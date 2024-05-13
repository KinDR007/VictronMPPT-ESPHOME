#include "victron.h"
#include "esphome/core/log.h"
#include <algorithm>  // std::min
#include "esphome/core/helpers.h"

namespace esphome {
namespace victron {

static const char *const TAG = "victron";

static const uint8_t OFF_REASONS_SIZE = 16;
static const char *const OFF_REASONS[OFF_REASONS_SIZE] = {
    "No input power",                       // 0000 0000 0000 0001
    "Switched off (power switch)",          // 0000 0000 0000 0010
    "Switched off (device mode register)",  // 0000 0000 0000 0100
    "Remote input",                         // 0000 0000 0000 1000
    "Protection active",                    // 0000 0000 0001 0000
    "Paygo",                                // 0000 0000 0010 0000
    "BMS",                                  // 0000 0000 0100 0000
    "Engine shutdown detection",            // 0000 0000 1000 0000
    "Analysing input voltage",              // 0000 0001 0000 0000
    "Unknown: Bit 10",                      // 0000 0010 0000 0000
    "Unknown: Bit 11",                      // 0000 0100 0000 0000
    "Unknown: Bit 12",                      // 0000 1000 0000 0000
    "Unknown: Bit 13",                      // 0001 0000 0000 0000
    "Unknown: Bit 14",                      // 0010 0000 0000 0000
    "Unknown: Bit 15",                      // 0100 0000 0000 0000
    "Unknown: Bit 16",                      // 1000 0000 0000 0000
};

void VictronComponent::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "Victron:");
  LOG_BINARY_SENSOR("  ", "Load state", load_state_binary_sensor_);
  LOG_BINARY_SENSOR("  ", "Relay state", relay_state_binary_sensor_);
  LOG_SENSOR("  ", "Max Power Yesterday", max_power_yesterday_sensor_);
  LOG_SENSOR("  ", "Max Power Today", max_power_today_sensor_);
  LOG_SENSOR("  ", "Yield Total", yield_total_sensor_);
  LOG_SENSOR("  ", "Yield Yesterday", yield_yesterday_sensor_);
  LOG_SENSOR("  ", "Yield Today", yield_today_sensor_);
  LOG_SENSOR("  ", "Panel Voltage", panel_voltage_sensor_);
  LOG_SENSOR("  ", "Panel Power", panel_power_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Voltage 2", battery_voltage_2_sensor_);
  LOG_SENSOR("  ", "Battery Voltage 3", battery_voltage_3_sensor_);
  LOG_SENSOR("  ", "Battery Current", battery_current_sensor_);
  LOG_SENSOR("  ", "Battery Current", battery_current_2_sensor_);
  LOG_SENSOR("  ", "Battery Current", battery_current_3_sensor_);
  LOG_SENSOR("  ", "AC Out Voltage", ac_out_voltage_sensor_);
  LOG_SENSOR("  ", "AC Out Current", ac_out_current_sensor_);
  LOG_SENSOR("  ", "Load Current", load_current_sensor_);
  LOG_SENSOR("  ", "Day Number", day_number_sensor_);
  LOG_SENSOR("  ", "Charging Mode ID", charging_mode_id_sensor_);
  LOG_SENSOR("  ", "Error Code", error_code_sensor_);
  LOG_SENSOR("  ", "Warning Code", warning_code_sensor_);
  LOG_SENSOR("  ", "Tracking Mode ID", tracking_mode_id_sensor_);
  LOG_SENSOR("  ", "Device Mode ID", device_mode_id_sensor_);
  LOG_SENSOR("  ", "Off Reason Bitmask", off_reason_bitmask_sensor_);
  LOG_TEXT_SENSOR("  ", "Charging Mode", charging_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Error Text", error_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Warning Text", warning_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Tracking Mode", tracking_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Device Mode", device_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Firmware Version", firmware_version_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Firmware Version 24bit", firmware_version_24bit_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Device Type", device_type_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Off Reason", off_reason_text_sensor_);

  LOG_SENSOR("  ", "Battery Temperature ", battery_temperature_sensor_);
  LOG_SENSOR("  ", "Instantaneous Power", instantaneous_power_sensor_);
  LOG_SENSOR("  ", "Consumed Amp Hours", consumed_amp_hours_sensor_);
  LOG_SENSOR("  ", "State Of Charge", state_of_charge_sensor_);
  LOG_SENSOR("  ", "Time To Go", time_to_go_sensor_);
  LOG_SENSOR("  ", "Depth Of The Deepest Discharge", depth_of_the_deepest_discharge_sensor_);
  LOG_SENSOR("  ", "Depth Of The Last Discharge", depth_of_the_last_discharge_sensor_);
  LOG_SENSOR("  ", "Number Of Charge Cycles", number_of_charge_cycles_sensor_);
  LOG_SENSOR("  ", "Number Of Full Discharges", number_of_full_discharges_sensor_);
  LOG_SENSOR("  ", "Min Battery Voltage", min_battery_voltage_sensor_);
  LOG_SENSOR("  ", "Max Battery Voltage", max_battery_voltage_sensor_);
  LOG_SENSOR("  ", "Last Full Charge", last_full_charge_sensor_);
  LOG_SENSOR("  ", "Amount Of Discharged Energy", amount_of_discharged_energy_sensor_);
  LOG_SENSOR("  ", "Amount Of Charged Energy", amount_of_charged_energy_sensor_);
  LOG_TEXT_SENSOR("  ", "Alarm Condition Active", alarm_condition_active_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Alarm Reason", alarm_reason_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Model Description", model_description_text_sensor_);

  check_uart_settings(19200);
}

void VictronComponent::loop() {
  if (async_uart_) {
    async_loop();
  } else {
    blocking_loop();
  }
}

void VictronComponent::blocking_loop() {
  const uint32_t now = millis();
  if ((state_ > 0) && (now - last_transmission_ >= 200)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago");
    state_ = 0;
  }

  if (!available())
    return;

  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (state_ == 0) {
      if (c == '\r' || c == '\n') {
        continue;
      }
      label_.clear();
      value_.clear();
      state_ = 1;
    }
    if (state_ == 1) {
      // Start of a ve.direct hex frame
      if (c == ':') {
        state_ = 3;
        continue;
      }
      if (c == '\t') {
        state_ = 2;
      } else {
        label_.push_back(c);
      }
      continue;
    }
    if (state_ == 2) {
      if (label_ == "Checksum") {
        state_ = 0;
        // The checksum is used as end of frame indicator
        if (now - this->last_publish_ >= this->throttle_) {
          this->last_publish_ = now;
          this->publishing_ = true;
        } else {
          this->publishing_ = false;
        }
        continue;
      }
      if (c == '\r' || c == '\n') {
        if (this->publishing_) {
          handle_value_(label_, value_);
        }
        state_ = 0;
      } else {
        value_.push_back(c);
      }
    }
    // Discard ve.direct hex frame
    if (state_ == 3) {
      if (c == '\r' || c == '\n') {
        state_ = 0;
      }
    }
  }
}

void VictronComponent::async_loop() {
  // publish one value at a time to yield to esphome between
  // each value and avoid blocking too long
  if (publishing_ && recv_buffer_.size() > 0) {
    std::pair<std::string, std::string> p = recv_buffer_.back();
    handle_value_(p.first, p.second);
    recv_buffer_.pop_back();
    if (recv_buffer_.size() == 0) {
      publishing_ = false;
    }
    return;
  }
  // reset publishing in case buffer is empty while publishing
  publishing_ = false;
  const uint32_t now = millis();
  if ((state_ > 0) && (now - last_transmission_ >= 200)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago");
    state_ = 0;
  }
  if (!available())
    return;

  last_transmission_ = now;
  uint8_t c;
  read_byte(&c);
  // checksum is calculated as the sum of all bytes in a frame
  // the final checksum should be a multiple of 256 (0 in 8 bit value)
  checksum_ += c;
  if (state_ == 0) {
    if (c == '\r' || c == '\n') {
      return;
    }
    // reset label/value
    label_.clear();
    value_.clear();
    state_ = 1;
    if (begin_frame_ == 0) {
      begin_frame_ = now;
    }
  }
  // read label
  if (state_ == 1) {
    // Start of a ve.direct hex frame
    if (c == ':') {
      state_ = 3;
      return;
    }
    if (c == '\t') {
      // end of label received, start reading value
      state_ = 2;
    } else {
      // update label
      label_.push_back(c);
    }
    return;
  }
  // read value
  if (state_ == 2) {
    // The checksum is used as end of frame indicator
    if (label_ == "Checksum") {
      state_ = 0;
      if (begin_frame_ - this->last_publish_ >= this->throttle_) {
        // check that checksum value is accurate
        if (checksum_ != 0) {
          // invalid checksum, drop frame
          ESP_LOGW(TAG, "Received invalid checksum, dropping frame: recv %d, calc %d", c, checksum_);
          checksum_ = 0;
          for (std::pair<std::string, std::string> element : recv_buffer_) {
            ESP_LOGD(TAG, ">> %s: %s", element.first.c_str(), element.second.c_str());
          }
          // clear buffer with invalid data
          recv_buffer_.clear();
          return;
        }
        this->last_publish_ = begin_frame_;
        // full buffer received, with valid checksum
        // set state to publishing to publish the values in the buffer
        publishing_ = true;
      } else {
        // frame is throttled, clear buffer and skip publishing
        ESP_LOGD(TAG, "recv throttled, drop frame");
        recv_buffer_.clear();
      }
      // reset checksum and frame
      checksum_ = 0;
      begin_frame_ = now;
      return;
    }
    if (c == '\r' || c == '\n') {
      // end of value received, add label/value to buffer
      recv_buffer_.insert(recv_buffer_.begin(), std::make_pair(label_, value_));
      state_ = 0;
    } else {
      // update value
      value_.push_back(c);
    }
  }
  // Discard ve.direct hex frame
  if (state_ == 3) {
    if (c == '\r' || c == '\n') {
      state_ = 0;
      checksum_ = 0;
      recv_buffer_.clear();
    }
  }
}

static std::string charging_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 1:
      return "Low power";
    case 2:
      return "Fault";
    case 3:
      return "Bulk";
    case 4:
      return "Absorption";
    case 5:
      return "Float";
    case 6:
      return "Storage";
    case 7:
      return "Equalize (manual)";
    case 9:
      return "Inverting";
    case 11:
      return "Power supply";
    case 245:
      return "Starting-up";
    case 246:
      return "Repeated absorption";
    case 247:
      return "Auto equalize / Recondition";
    case 248:
      return "BatterySafe";
    case 252:
      return "External control";
    default:
      return "Unknown";
  }
}

static std::string error_code_text(int value) {
  switch (value) {
    case 0:
      return "No error";
    case 2:
      return "Battery voltage too high";
    case 17:
      return "Charger temperature too high";
    case 18:
      return "Charger over current";
    case 19:
      return "Charger current reversed";
    case 20:
      return "Bulk time limit exceeded";
    case 21:
      return "Current sensor issue";
    case 26:
      return "Terminals overheated";
    case 28:
      return "Converter issue";
    case 33:
      return "Input voltage too high (solar panel)";
    case 34:
      return "Input current too high (solar panel)";
    case 38:
      return "Input shutdown (excessive battery voltage)";
    case 39:
      return "Input shutdown (due to current flow during off mode)";
    case 65:
      return "Lost communication with one of devices";
    case 66:
      return "Synchronised charging device configuration issue";
    case 67:
      return "BMS connection lost";
    case 68:
      return "Network misconfigured";
    case 116:
      return "Factory calibration data lost";
    case 117:
      return "Invalid/incompatible firmware";
    case 119:
      return "User settings invalid";
    default:
      return "Unknown";
  }
}

static std::string warning_code_text(int value) {
  switch (value) {
    case 0:
      return "No warning";
    case 1:
      return "Low Voltage";
    case 2:
      return "High Voltage";
    case 4:
      return "Low SOC";
    case 8:
      return "Low Starter Voltage";
    case 16:
      return "High Starter Voltage";
    case 32:
      return "Low Temperature";
    case 64:
      return "High Temperature";
    case 128:
      return "Mid Voltage";
    case 256:
      return "Overload";
    case 512:
      return "DC-ripple";
    case 1024:
      return "Low V AC out";
    case 2048:
      return "High V AC out";
    default:
      return "Multiple warnings";
  }
}

static std::string tracking_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 1:
      return "Limited";
    case 2:
      return "Active";
    default:
      return "Unknown";
  }
}

static std::string device_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 2:
      return "On";
    case 4:
      return "Off";
    case 5:
      return "Eco";
    default:
      return "Unknown";
  }
}

static std::string dc_monitor_mode_text(int value) {
  switch (value) {
    case -9:
      return "Solar charger";
    case -8:
      return "Wind turbine";
    case -7:
      return "Shaft generator";
    case -6:
      return "Alternator";
    case -5:
      return "Fuel cell";
    case -4:
      return "Water generator";
    case -3:
      return "DC/DC charger";
    case -2:
      return "AC charger";
    case -1:
      return "Generic source";
    case 0:
      return "Battery monitor (BMV)";
    case 1:
      return "Generic load";
    case 2:
      return "Electric drive";
    case 3:
      return "Fridge";
    case 4:
      return "Water pump";
    case 5:
      return "Bilge pump";
    case 6:
      return "DC system";
    case 7:
      return "Inverter";
    case 8:
      return "Water heater";
    default:
      return "Unknown";
  }
}

static std::string device_type_text(int value) {
  switch (value) {
    case 0x203:
      return "BMV-700";
    case 0x204:
      return "BMV-702";
    case 0x205:
      return "BMV-700H";
    case 0x0300:
      return "BlueSolar MPPT 70|15";
    case 0xA040:
      return "BlueSolar MPPT 75|50";
    case 0xA041:
      return "BlueSolar MPPT 150|35";
    case 0xA042:
      return "BlueSolar MPPT 75|15";
    case 0xA043:
      return "BlueSolar MPPT 100|15";
    case 0xA044:
      return "BlueSolar MPPT 100|30";
    case 0xA045:
      return "BlueSolar MPPT 100|50";
    case 0xA046:
      return "BlueSolar MPPT 150|70";
    case 0xA047:
      return "BlueSolar MPPT 150|100";
    case 0xA049:
      return "BlueSolar MPPT 100|50 rev2";
    case 0xA04A:
      return "BlueSolar MPPT 100|30 rev2";
    case 0xA04B:
      return "BlueSolar MPPT 150|35 rev2";
    case 0xA04C:
      return "BlueSolar MPPT 75|10";
    case 0xA04D:
      return "BlueSolar MPPT 150|45";
    case 0xA04E:
      return "BlueSolar MPPT 150|60";
    case 0xA04F:
      return "BlueSolar MPPT 150|85";
    case 0xA050:
      return "SmartSolar MPPT 250|100";
    case 0xA051:
      return "SmartSolar MPPT 150|100";
    case 0xA052:
      return "SmartSolar MPPT 150|85";
    case 0xA053:
      return "SmartSolar MPPT 75|15";
    case 0xA054:
      return "SmartSolar MPPT 75|10";
    case 0xA055:
      return "SmartSolar MPPT 100|15";
    case 0xA056:
      return "SmartSolar MPPT 100|30";
    case 0xA073:
      return "SmartSolar MPPT 150|45 rev3";
    case 0xA057:
      return "SmartSolar MPPT 100|50";
    case 0xA058:
      return "SmartSolar MPPT 150|35";
    case 0xA059:
      return "SmartSolar MPPT 150|100 rev2";
    case 0xA05A:
      return "SmartSolar MPPT 150|85 rev2";
    case 0xA05B:
      return "SmartSolar MPPT 250|70";
    case 0xA05C:
      return "SmartSolar MPPT 250|85";
    case 0xA05D:
      return "SmartSolar MPPT 250|60";
    case 0xA05E:
      return "SmartSolar MPPT 250|45";
    case 0xA05F:
      return "SmartSolar MPPT 100|20";
    case 0xA060:
      return "SmartSolar MPPT 100|20 48V";
    case 0xA061:
      return "SmartSolar MPPT 150|45";
    case 0xA062:
      return "SmartSolar MPPT 150|60";
    case 0xA063:
      return "SmartSolar MPPT 150|70";
    case 0xA064:
      return "SmartSolar MPPT 250|85 rev2";
    case 0xA065:
      return "SmartSolar MPPT 250|100 rev2";
    case 0xA066:
      return "BlueSolar MPPT 100|20";
    case 0xA067:
      return "BlueSolar MPPT 100|20 48V";
    case 0xA068:
      return "SmartSolar MPPT 250|60 rev2";
    case 0xA069:
      return "SmartSolar MPPT 250|70 rev2";
    case 0xA06A:
      return "SmartSolar MPPT 150|45 rev2";
    case 0xA06B:
      return "SmartSolar MPPT 150|60 rev2";
    case 0xA06C:
      return "SmartSolar MPPT 150|70 rev2";
    case 0xA06D:
      return "SmartSolar MPPT 150|85 rev3";
    case 0xA06E:
      return "SmartSolar MPPT 150|100 rev3";
    case 0xA06F:
      return "BlueSolar MPPT 150|45 rev2";
    case 0xA070:
      return "BlueSolar MPPT 150|60 rev2";
    case 0xA071:
      return "BlueSolar MPPT 150|70 rev2";
    case 0xA102:
      return "SmartSolar MPPT VE.Can 150/70";
    case 0xA103:
      return "SmartSolar MPPT VE.Can 150/45";
    case 0xA104:
      return "SmartSolar MPPT VE.Can 150/60";
    case 0xA105:
      return "SmartSolar MPPT VE.Can 150/85";
    case 0xA106:
      return "SmartSolar MPPT VE.Can 150/100";
    case 0xA107:
      return "SmartSolar MPPT VE.Can 250/45";
    case 0xA108:
      return "SmartSolar MPPT VE.Can 250/60";
    case 0xA109:
      return "SmartSolar MPPT VE.Can 250/70";
    case 0xA10A:
      return "SmartSolar MPPT VE.Can 250/85";
    case 0xA10B:
      return "SmartSolar MPPT VE.Can 250/100";
    case 0xA10C:
      return "SmartSolar MPPT VE.Can 150/70 rev2";
    case 0xA10D:
      return "SmartSolar MPPT VE.Can 150/85 rev2";
    case 0xA10E:
      return "SmartSolar MPPT VE.Can 150/100 rev2";
    case 0xA10F:
      return "BlueSolar MPPT VE.Can 150/100";
    case 0xA112:
      return "BlueSolar MPPT VE.Can 250/70";
    case 0xA113:
      return "BlueSolar MPPT VE.Can 250/100";
    case 0xA114:
      return "SmartSolar MPPT VE.Can 250/70 rev2";
    case 0xA115:
      return "SmartSolar MPPT VE.Can 250/100 rev2";
    case 0xA116:
      return "SmartSolar MPPT VE.Can 250/85 rev2";
    case 0xA201:
      return "Phoenix Inverter 12V 250VA 230V";
    case 0xA202:
      return "Phoenix Inverter 24V 250VA 230V";
    case 0xA204:
      return "Phoenix Inverter 48V 250VA 230V";
    case 0xA211:
      return "Phoenix Inverter 12V 375VA 230V";
    case 0xA212:
      return "Phoenix Inverter 24V 375VA 230V";
    case 0xA214:
      return "Phoenix Inverter 48V 375VA 230V";
    case 0xA221:
      return "Phoenix Inverter 12V 500VA 230V";
    case 0xA222:
      return "Phoenix Inverter 24V 500VA 230V";
    case 0xA224:
      return "Phoenix Inverter 48V 500VA 230V";
    case 0xA231:
      return "Phoenix Inverter 12V 250VA 230V";
    case 0xA232:
      return "Phoenix Inverter 24V 250VA 230V";
    case 0xA234:
      return "Phoenix Inverter 48V 250VA 230V";
    case 0xA239:
      return "Phoenix Inverter 12V 250VA 120V";
    case 0xA23A:
      return "Phoenix Inverter 24V 250VA 120V";
    case 0xA23C:
      return "Phoenix Inverter 48V 250VA 120V";
    case 0xA241:
      return "Phoenix Inverter 12V 375VA 230V";
    case 0xA242:
      return "Phoenix Inverter 24V 375VA 230V";
    case 0xA244:
      return "Phoenix Inverter 48V 375VA 230V";
    case 0xA249:
      return "Phoenix Inverter 12V 375VA 120V";
    case 0xA24A:
      return "Phoenix Inverter 24V 375VA 120V";
    case 0xA24C:
      return "Phoenix Inverter 48V 375VA 120V";
    case 0xA251:
      return "Phoenix Inverter 12V 500VA 230V";
    case 0xA252:
      return "Phoenix Inverter 24V 500VA 230V";
    case 0xA254:
      return "Phoenix Inverter 48V 500VA 230V";
    case 0xA259:
      return "Phoenix Inverter 12V 500VA 120V";
    case 0xA25A:
      return "Phoenix Inverter 24V 500VA 120V";
    case 0xA25C:
      return "Phoenix Inverter 48V 500VA 120V";
    case 0xA261:
      return "Phoenix Inverter 12V 800VA 230V";
    case 0xA262:
      return "Phoenix Inverter 24V 800VA 230V";
    case 0xA264:
      return "Phoenix Inverter 48V 800VA 230V";
    case 0xA269:
      return "Phoenix Inverter 12V 800VA 120V";
    case 0xA26A:
      return "Phoenix Inverter 24V 800VA 120V";
    case 0xA26C:
      return "Phoenix Inverter 48V 800VA 120V";
    case 0xA271:
      return "Phoenix Inverter 12V 1200VA 230V";
    case 0xA272:
      return "Phoenix Inverter 24V 1200VA 230V";
    case 0xA274:
      return "Phoenix Inverter 48V 1200VA 230V";
    case 0xA279:
      return "Phoenix Inverter 12V 1200VA 120V";
    case 0xA27A:
      return "Phoenix Inverter 24V 1200VA 120V";
    case 0xA27C:
      return "Phoenix Inverter 48V 1200VA 120V";
    case 0xA281:
      return "Phoenix Inverter 12V 1600VA 230V";
    case 0xA282:
      return "Phoenix Inverter 24V 1600VA 230V";
    case 0xA284:
      return "Phoenix Inverter 48V 1600VA 230V";
    case 0xA291:
      return "Phoenix Inverter 12V 2000VA 230V";
    case 0xA292:
      return "Phoenix Inverter 24V 2000VA 230V";
    case 0xA294:
      return "Phoenix Inverter 48V 2000VA 230V";
    case 0xA2A1:
      return "Phoenix Inverter 12V 3000VA 230V";
    case 0xA2A2:
      return "Phoenix Inverter 24V 3000VA 230V";
    case 0xA2A4:
      return "Phoenix Inverter 48V 3000VA 230V";
    case 0xA340:
      return "Phoenix Smart IP43 Charger 12|50 (1+1)";
    case 0xA341:
      return "Phoenix Smart IP43 Charger 12|50 (3)";
    case 0xA342:
      return "Phoenix Smart IP43 Charger 24|25 (1+1)";
    case 0xA343:
      return "Phoenix Smart IP43 Charger 24|25 (3)";
    case 0xA344:
      return "Phoenix Smart IP43 Charger 12|30 (1+1)";
    case 0xA345:
      return "Phoenix Smart IP43 Charger 12|30 (3)";
    case 0xA346:
      return "Phoenix Smart IP43 Charger 24|16 (1+1)";
    case 0xA347:
      return "Phoenix Smart IP43 Charger 24|16 (3)";
    case 0xA381:
      return "BMV-712 Smart";
    case 0xA382:
      return "BMV-710H Smart";
    case 0xA383:
      return "BMV-712 Smart Rev2";
    case 0xA389:
      return "SmartShunt 500A/50mV";
    case 0xA38A:
      return "SmartShunt 1000A/50mV";
    case 0xA38B:
      return "SmartShunt 2000A/50mV";
    case 0xA442:
      return "Multi RS Solar 48V 6000VA 230V";
    default:
      return "Unknown";
  }
}

static std::string off_reason_text(uint32_t mask) {
  bool first = true;
  std::string value_list = "";

  if (mask) {
    for (uint8_t i = 0; i < OFF_REASONS_SIZE; i++) {
      if (mask & (1 << i)) {
        if (first) {
          first = false;
        } else {
          value_list.append(";");
        }
        value_list.append(OFF_REASONS[i]);
      }
    }
  }

  return value_list;
}

void VictronComponent::handle_value_(std::string l, std::string v) {
  int value;

  if (l == "V") {
    this->publish_state_(battery_voltage_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "V2") {
    // mV to V
    this->publish_state_(battery_voltage_2_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "V3") {
    // mV to V
    this->publish_state_(battery_voltage_3_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "VS") {
    // mV to V
    this->publish_state_(auxiliary_battery_voltage_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "VM") {
    // mV to V
    this->publish_state_(midpoint_voltage_of_the_battery_bank_sensor_,
                         atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "DM") {
    // Per mill to %
    this->publish_state_(midpoint_deviation_of_the_battery_bank_sensor_,
                         atoi(v.c_str()) * 0.10f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "VPV") {
    // mV to V
    this->publish_state_(panel_voltage_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "PPV") {
    this->publish_state_(panel_power_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "I") {
    // mA to A
    this->publish_state_(battery_current_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "I2") {
    // mA to A
    this->publish_state_(battery_current_2_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "I3") {
    // mA to A
    this->publish_state_(battery_current_3_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "IL") {
    this->publish_state_(load_current_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "LOAD") {
    this->publish_state_(load_state_binary_sensor_, v == "ON" || v == "On");
    return;
  }

  if (l == "T") {
    if (v == "---") {
      this->publish_state_(battery_temperature_sensor_, NAN);
      return;
    }

    this->publish_state_(battery_temperature_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "P") {
    this->publish_state_(instantaneous_power_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "CE") {
    // mAh -> Ah
    this->publish_state_(consumed_amp_hours_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "SOC") {
    // Per mill to %
    this->publish_state_(state_of_charge_sensor_, atoi(v.c_str()) * 0.10f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "TTG") {
    this->publish_state_(time_to_go_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "Alarm") {
    this->publish_state_(alarm_condition_active_text_sensor_, v);
    return;
  }

  if (l == "Relay") {
    this->publish_state_(relay_state_binary_sensor_, v == "ON" || v == "On");
    return;
  }

  if (l == "AR") {
    this->publish_state_(alarm_reason_text_sensor_, error_code_text(atoi(v.c_str())));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "OR") {
    auto off_reason_bitmask = parse_hex<uint32_t>(v.substr(2, v.size() - 2));
    if (off_reason_bitmask) {
      this->publish_state_(off_reason_bitmask_sensor_, *off_reason_bitmask);
      this->publish_state_(off_reason_text_sensor_, off_reason_text(*off_reason_bitmask));
    }
    return;
  }

  if (l == "H1") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_deepest_discharge_sensor_,
                         atoi(v.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H2") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_last_discharge_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H3") {
    // mAh -> Ah
    this->publish_state_(depth_of_the_average_discharge_sensor_,
                         atoi(v.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H4") {
    this->publish_state_(number_of_charge_cycles_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H5") {
    this->publish_state_(number_of_full_discharges_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H6") {
    if (v == "---") {
      this->publish_state_(cumulative_amp_hours_drawn_sensor_, NAN);
      return;
    }

    // mAh -> Ah
    this->publish_state_(cumulative_amp_hours_drawn_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H7") {
    // mV to V
    this->publish_state_(min_battery_voltage_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H8") {
    // mV to V
    this->publish_state_(max_battery_voltage_sensor_, atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H9") {
    if (v == "---") {
      this->publish_state_(last_full_charge_sensor_, NAN);
      return;
    }

    // sec -> min
    this->publish_state_(last_full_charge_sensor_, (float) atoi(v.c_str()) / 60.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H10") {
    if (v == "---") {
      this->publish_state_(number_of_automatic_synchronizations_sensor_, NAN);
      return;
    }

    this->publish_state_(number_of_automatic_synchronizations_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H11") {
    this->publish_state_(number_of_low_main_voltage_alarms_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H12") {
    this->publish_state_(number_of_high_main_voltage_alarms_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H13") {
    this->publish_state_(number_of_low_auxiliary_voltage_alarms_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H14") {
    this->publish_state_(number_of_high_auxiliary_voltage_alarms_sensor_,
                         atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H15") {
    // mV to V
    this->publish_state_(min_auxiliary_battery_voltage_sensor_,
                         atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H16") {
    // mV to V
    this->publish_state_(max_auxiliary_battery_voltage_sensor_,
                         atoi(v.c_str()) / 1000.0f);  // NOLINT(cert-err34-c)
    return;
  }

  // "H17"    0.01 kWh   Amount of discharged energy (BMV) / Amount of produced energy (DC monitor)
  if (l == "H17") {
    // Wh
    this->publish_state_(amount_of_discharged_energy_sensor_, atoi(v.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  // "H18"    0.01 kWh   Amount of charged energy (BMV) / Amount of consumed energy (DC monitor)
  if (l == "H18") {
    // Wh
    this->publish_state_(amount_of_charged_energy_sensor_, atoi(v.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H19") {
    this->publish_state_(yield_total_sensor_, atoi(v.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H20") {
    this->publish_state_(yield_today_sensor_, atoi(v.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H21") {
    this->publish_state_(max_power_today_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H22") {
    this->publish_state_(yield_yesterday_sensor_, atoi(v.c_str()) * 10.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "H23") {
    this->publish_state_(max_power_yesterday_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "ERR") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(error_code_sensor_, value);
    this->publish_state_(error_text_sensor_, error_code_text(value));
    return;
  }

  if (l == "CS") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(charging_mode_id_sensor_, (float) value);
    this->publish_state_(charging_mode_text_sensor_, charging_mode_text(value));
    return;
  }

  // "BMV"               Model description (deprecated)
  if (l == "BMV") {
    this->publish_state_(model_description_text_sensor_, v);
    return;
  }

  if (l == "FW") {
    this->publish_state_once_(firmware_version_text_sensor_, v.insert(v.size() - 2, "."));
    return;
  }

  if (l == "FWE") {
    if (this->firmware_version_24bit_text_sensor_ == nullptr || this->firmware_version_24bit_text_sensor_->has_state())
      return;

    if (v.size() > 4) {
      std::string release_type = v.substr(v.size() - 2, 2);
      std::string version_number = v.substr(0, v.size() - 2);
      version_number = version_number.insert(version_number.size() - 2, ".");
      release_type = (release_type == "FF") ? "-official" : "-beta-" + release_type;

      this->publish_state_once_(firmware_version_24bit_text_sensor_, version_number + release_type);
      return;
    }

    this->publish_state_once_(firmware_version_24bit_text_sensor_, v);
    return;
  }

  if (l == "PID") {
    this->publish_state_once_(device_type_text_sensor_, device_type_text(strtol(v.c_str(), nullptr, 0)));
    return;
  }

  if (l == "SER#") {
    this->publish_state_once_(serial_number_text_sensor_, v);
    return;
  }

  if (l == "HSDS") {
    this->publish_state_(day_number_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "MODE") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(device_mode_id_sensor_, (float) value);
    this->publish_state_(device_mode_text_sensor_, device_mode_text(value));
    return;
  }

  if (l == "AC_OUT_V") {
    this->publish_state_(ac_out_voltage_sensor_, atoi(v.c_str()) / 100.0f);  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "AC_OUT_I") {
    this->publish_state_(ac_out_current_sensor_, std::max(0.0f, atoi(v.c_str()) / 10.0f));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "AC_OUT_S") {
    this->publish_state_(ac_out_apparent_power_sensor_, atoi(v.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (l == "WARN") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(warning_code_sensor_, value);
    this->publish_state_(warning_text_sensor_, warning_code_text(value));
    return;
  }

  if (l == "MPPT") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(tracking_mode_id_sensor_, (float) value);
    this->publish_state_(tracking_mode_text_sensor_, tracking_mode_text(value));
    return;
  }

  if (l == "MON") {
    value = atoi(v.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(dc_monitor_mode_id_sensor_, (float) value);
    this->publish_state_(dc_monitor_mode_text_sensor_, dc_monitor_mode_text(value));
    return;
  }

  ESP_LOGD(TAG, "Unhandled property: %s %s", l.c_str(), v.c_str());
}

void VictronComponent::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void VictronComponent::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void VictronComponent::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

void VictronComponent::publish_state_once_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  if (text_sensor->has_state())
    return;

  text_sensor->publish_state(state);
}

}  // namespace victron
}  // namespace esphome
