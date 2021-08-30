#include "victron.h"
#include "esphome/core/log.h"

namespace esphome {
namespace victron {

static const char *TAG = "victron";

void VictronComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Victron:");
  LOG_SENSOR("  ", "Max Power Yesterday", max_power_yesterday_sensor_);
  LOG_SENSOR("  ", "Max Power Today", max_power_today_sensor_);
  LOG_SENSOR("  ", "Yield Total", yield_total_sensor_);
  LOG_SENSOR("  ", "Yield Yesterday", yield_yesterday_sensor_);
  LOG_SENSOR("  ", "Yield Today", yield_today_sensor_);
  LOG_SENSOR("  ", "Panel Voltage", panel_voltage_sensor_);
  LOG_SENSOR("  ", "Panel Power", panel_power_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", battery_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Current", battery_current_sensor_);
  LOG_SENSOR("  ", "Load Current", load_current_sensor_);
  LOG_SENSOR("  ", "Day Number", day_number_sensor_);
  LOG_SENSOR("  ", "Charger Status", charger_status_sensor_);
  LOG_SENSOR("  ", "Error Code", error_code_sensor_);
  LOG_TEXT_SENSOR("  ", "Charger Text", charger_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Error Text", error_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Tracker Operation", tracker_operation_sensor_);
  LOG_TEXT_SENSOR("  ", "FW Version", fw_version_sensor_);
  LOG_TEXT_SENSOR("  ", "PID", pid_sensor_);
  check_uart_settings(19200);
}

void VictronComponent::loop() {
  const uint32_t now = millis();
  if ((state_ > 0) && (now - last_transmission_ >= 200)) {
    // last transmission too long ago. Reset RX index.
    ESP_LOGW(TAG, "Last transmission too long ago.");
    state_ = 0;
  }

  if (!available())
    return;

  last_transmission_ = now;
  while (available()) {
    uint8_t c;
    read_byte(&c);
    if (state_ == 0) {
      if ((c == '\r') || (c == '\n'))
        continue;
      label_.clear();
      value_.clear();
      state_ = 1;
    }
    if (state_ == 1) {
      if (c == '\t')
        state_ = 2;
      else
        label_.push_back(c);
      continue;
    }
    if (state_ == 2) {
      if (label_ == "Checksum") {
        state_ = 0;
        continue;
      }
      if ((c == '\r') || (c == '\n')) {
        handle_value_();
        state_ = 0;
      } else {
        value_.push_back(c);
      }
    }
  }
}

static const __FlashStringHelper* charger_status_text(int value) {
  switch (value) {
    case 0: return F("Off");
    case 2: return F("Fault");
    case 3: return F("Bulk");
    case 4: return F("Absorption");
    case 5: return F("Float");
    case 7: return F("Equalize (manual)");
    case 245: return F("Starting-up");
    case 247: return F("Auto equalize / Recondition");
    case 252: return F("External control");
    default: return F("Unknown");
  }
}

static const __FlashStringHelper* error_code_text(int value) {
  switch (value) {
    case 0: return F("No error");
    case 2: return F("Battery voltage too high");
    case 17: return F("Charger temperature too high");
    case 18: return F("Charger over current");
    case 19: return F("Charger current reversed");
    case 20: return F("Bulk time limit exceeded");
    case 21: return F("Current sensor issue");
    case 26: return F("Terminals overheated");
    case 28: return F("Converter issue");
    case 33: return F("Input voltage too high (solar panel)");
    case 34: return F("Input current too high (solar panel)");
    case 38: return F("Input shutdown (excessive battery voltage)");
    case 39: return F("Input shutdown (due to current flow during off mode)");
    case 65: return F("Lost communication with one of devices");
    case 66: return F("Synchronised charging device configuration issue");
    case 67: return F("BMS connection lost");
    case 68: return F("Network misconfigured");
    case 116: return F("Factory calibration data lost");
    case 117: return F("Invalid/incompatible firmware");
    case 119: return F("User settings invalid");
    default: return F("Unknown");
  }
}

static const std::string tracker_op_text(int value) {
  switch (value) {
    case 0: return "Off";
    case 1: return "Limited";
    case 2: return "Active";
    default: return "Unknown";
  }
}

static const __FlashStringHelper* pid_text(int value) {
  switch (value) {
    case 0x203: return F("BMV-700");
    case 0x204: return F("BMV-702");
    case 0x205: return F("BMV-700H");
    case 0xA389: return F("SmartShunt");
    case 0xA381: return F("BMV-712 Smart");
    case 0xA04C: return F("BlueSolar MPPT 75/10");
    case 0x300: return F("BlueSolar MPPT 70/15");
    case 0xA042: return F("BlueSolar MPPT 75/15");
    case 0xA043: return F("BlueSolar MPPT 100/15");
    case 0xA044: return F("BlueSolar MPPT 100/30 rev1");
    case 0xA04A: return F("BlueSolar MPPT 100/30 rev2");
    case 0xA041: return F("BlueSolar MPPT 150/35 rev1");
    case 0xA04B: return F("BlueSolar MPPT 150/35 rev2");
    case 0xA04D: return F("BlueSolar MPPT 150/45");
    case 0xA040: return F("BlueSolar MPPT 75/50");
    case 0xA045: return F("BlueSolar MPPT 100/50 rev1");
    case 0xA049: return F("BlueSolar MPPT 100/50 rev2");
    case 0xA04E: return F("BlueSolar MPPT 150/60");
    case 0xA046: return F("BlueSolar MPPT 150/70");
    case 0xA04F: return F("BlueSolar MPPT 150/85");
    case 0xA047: return F("BlueSolar MPPT 150/100");
    case 0xA050: return F("SmartSolar MPPT 250/100");
    case 0xA051: return F("SmartSolar MPPT 150/100");
    case 0xA052: return F("SmartSolar MPPT 150/85");
    case 0xA053: return F("SmartSolar MPPT 75/15");
    case 0xA054: return F("SmartSolar MPPT 75/10");
    case 0xA055: return F("SmartSolar MPPT 100/15");
    case 0xA056: return F("SmartSolar MPPT 100/30");
    case 0xA057: return F("SmartSolar MPPT 100/50");
    case 0xA058: return F("SmartSolar MPPT 150/35");
    case 0xA059: return F("SmartSolar MPPT 150/100 rev2");
    case 0xA05A: return F("SmartSolar MPPT 150/85 rev2");
    case 0xA05B: return F("SmartSolar MPPT 250/70");
    case 0xA05C: return F("SmartSolar MPPT 250/85");
    case 0xA05D: return F("SmartSolar MPPT 250/60");
    case 0xA05E: return F("SmartSolar MPPT 250/45");
    case 0xA05F: return F("SmartSolar MPPT 100/20");
    case 0xA060: return F("SmartSolar MPPT 100/20 48V");
    case 0xA061: return F("SmartSolar MPPT 150/45");
    case 0xA062: return F("SmartSolar MPPT 150/60");
    case 0xA063: return F("SmartSolar MPPT 150/70");
    case 0xA064: return F("SmartSolar MPPT 250/85 rev2");
    case 0xA065: return F("SmartSolar MPPT 250/100 rev2");
    case 0xA201: return F("Phoenix Inverter 12V 250VA 230V");
    case 0xA202: return F("Phoenix Inverter 24V 250VA 230V");
    case 0xA204: return F("Phoenix Inverter 48V 250VA 230V");
    case 0xA211: return F("Phoenix Inverter 12V 375VA 230V");
    case 0xA212: return F("Phoenix Inverter 24V 375VA 230V");
    case 0xA214: return F("Phoenix Inverter 48V 375VA 230V");
    case 0xA221: return F("Phoenix Inverter 12V 500VA 230V");
    case 0xA222: return F("Phoenix Inverter 24V 500VA 230V");
    case 0xA224: return F("Phoenix Inverter 48V 500VA 230V");
    case 0xA231: return F("Phoenix Inverter 12V 250VA 230V");
    case 0xA232: return F("Phoenix Inverter 24V 250VA 230V");
    case 0xA234: return F("Phoenix Inverter 48V 250VA 230V");
    case 0xA239: return F("Phoenix Inverter 12V 250VA 120V");
    case 0xA23A: return F("Phoenix Inverter 24V 250VA 120V");
    case 0xA23C: return F("Phoenix Inverter 48V 250VA 120V");
    case 0xA241: return F("Phoenix Inverter 12V 375VA 230V");
    case 0xA242: return F("Phoenix Inverter 24V 375VA 230V");
    case 0xA244: return F("Phoenix Inverter 48V 375VA 230V");
    case 0xA249: return F("Phoenix Inverter 12V 375VA 120V");
    case 0xA24A: return F("Phoenix Inverter 24V 375VA 120V");
    case 0xA24C: return F("Phoenix Inverter 48V 375VA 120V");
    case 0xA251: return F("Phoenix Inverter 12V 500VA 230V");
    case 0xA252: return F("Phoenix Inverter 24V 500VA 230V");
    case 0xA254: return F("Phoenix Inverter 48V 500VA 230V");
    case 0xA259: return F("Phoenix Inverter 12V 500VA 120V");
    case 0xA25A: return F("Phoenix Inverter 24V 500VA 120V");
    case 0xA25C: return F("Phoenix Inverter 48V 500VA 120V");
    case 0xA261: return F("Phoenix Inverter 12V 800VA 230V");
    case 0xA262: return F("Phoenix Inverter 24V 800VA 230V");
    case 0xA264: return F("Phoenix Inverter 48V 800VA 230V");
    case 0xA269: return F("Phoenix Inverter 12V 800VA 120V");
    case 0xA26A: return F("Phoenix Inverter 24V 800VA 120V");
    case 0xA26C: return F("Phoenix Inverter 48V 800VA 120V");
    case 0xA271: return F("Phoenix Inverter 12V 1200VA 230V");
    case 0xA272: return F("Phoenix Inverter 24V 1200VA 230V");
    case 0xA274: return F("Phoenix Inverter 48V 1200VA 230V");
    case 0xA279: return F("Phoenix Inverter 12V 1200VA 120V");
    case 0xA27A: return F("Phoenix Inverter 24V 1200VA 120V");
    case 0xA27C: return F("Phoenix Inverter 48V 1200VA 120V");
    default: return nullptr;
  }
}

static std::string flash_to_string(const __FlashStringHelper *flash) {
  std::string result;
  const char *fptr = (PGM_P) flash;
  result.reserve(strlen_P(fptr));
  char c;
  while ((c = pgm_read_byte(fptr++)) != 0)
    result.push_back(c);
  return result;
}

void VictronComponent::handle_value_() {
  int value;

  if (label_ == "H23") {
    if (max_power_yesterday_sensor_ != nullptr)
      max_power_yesterday_sensor_->publish_state(atoi(value_.c_str()));
  } else if (label_ == "H21") {
    if (max_power_today_sensor_ != nullptr)
      max_power_today_sensor_->publish_state(atoi(value_.c_str()));
  } else if (label_ == "H19") {
    if (yield_total_sensor_ != nullptr)
      yield_total_sensor_->publish_state(atoi(value_.c_str()) * 10);
  } else if (label_ == "H22") {
    if (yield_yesterday_sensor_ != nullptr)
      yield_yesterday_sensor_->publish_state(atoi(value_.c_str()) * 10);
  } else if (label_ == "H20") {
    if (yield_today_sensor_ != nullptr)
      yield_today_sensor_->publish_state(atoi(value_.c_str()) * 10);
  } else if (label_ == "VPV") {
    if (panel_voltage_sensor_ != nullptr)
      panel_voltage_sensor_->publish_state(atoi(value_.c_str()) / 1000.0);
  } else if (label_ == "PPV") {
    if (panel_power_sensor_ != nullptr)
      panel_power_sensor_->publish_state(atoi(value_.c_str()));
  } else if (label_ == "V") {
    if (battery_voltage_sensor_ != nullptr)
      battery_voltage_sensor_->publish_state(atoi(value_.c_str()) / 1000.0);
  } else if (label_ == "I") {
    if (battery_current_sensor_ != nullptr)
      battery_current_sensor_->publish_state(atoi(value_.c_str()) / 1000.0);      
  } else if (label_ == "IL") {
    if (load_current_sensor_ != nullptr)
      load_current_sensor_->publish_state(atoi(value_.c_str()) / 1000.0); 
  } else if (label_ == "HSDS") {
    if (day_number_sensor_ != nullptr)
      day_number_sensor_->publish_state(atoi(value_.c_str()));
  } else if (label_ == "CS") {
    value = atoi(value_.c_str());
    if (charger_status_sensor_ != nullptr)
      charger_status_sensor_->publish_state(value);
    if (charger_text_sensor_ != nullptr)
      charger_text_sensor_->publish_state(flash_to_string(charger_status_text(value)));
  } else if (label_ == "ERR") {
    value = atoi(value_.c_str());
    if (error_code_sensor_ != nullptr)
      error_code_sensor_->publish_state(value);
    if (error_text_sensor_ != nullptr)
      error_text_sensor_->publish_state(flash_to_string(error_code_text(value)));
  } else if (label_ == "MPPT") {
    value = atoi(value_.c_str());
    if (tracker_operation_sensor_ != nullptr)
      tracker_operation_sensor_->publish_state(value);
    if (tracker_text_sensor_ != nullptr)
      tracker_text_sensor_->publish_state(tracker_op_text(value));
  } else if (label_ == "FW") {
    if ((fw_version_sensor_ != nullptr) && !fw_version_sensor_->has_state())
      fw_version_sensor_->publish_state(value_.insert(value_.size() - 2, "."));
  } else if (label_ == "PID") {
    //value = atoi(value_.c_str());

    //ESP_LOGD(TAG, "received PID: '%s'", value_.c_str()); 
    value = strtol(value_.c_str(), NULL, 0);
    //ESP_LOGD(TAG, "received PID: '%04x'", value); 
    if ((pid_sensor_ != nullptr) && !pid_sensor_->has_state()) {
      const __FlashStringHelper *flash = pid_text(value);
      if (flash != nullptr) {
        pid_sensor_->publish_state(flash_to_string(flash));
      }// else {
       // char s[30];
       // snprintf(s, 30, "Unknown device (%04x)", value);
        //pid_sensor_->publish_state(s);
      //}
    }
  }
}

}  // namespace victron
}  // namespace esphome
