#include "victron.h"
#include "esphome/core/log.h"
#include <algorithm>  // std::min

namespace esphome {
namespace victron {

static const char *const TAG = "victron";

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
  LOG_SENSOR("  ", "AC Out Voltage", ac_out_voltage_sensor_);
  LOG_SENSOR("  ", "AC Out Current", ac_out_current_sensor_);
  LOG_SENSOR("  ", "Load Current", load_current_sensor_);
  LOG_SENSOR("  ", "Day Number", day_number_sensor_);
  LOG_SENSOR("  ", "Charging Mode ID", charging_mode_id_sensor_);
  LOG_SENSOR("  ", "Error Code", error_code_sensor_);
  LOG_SENSOR("  ", "Warning Code", warning_code_sensor_);
  LOG_SENSOR("  ", "Tracking Mode ID", tracking_mode_id_sensor_);
  LOG_SENSOR("  ", "Device Mode ID", device_mode_id_sensor_);
  LOG_TEXT_SENSOR("  ", "Charging Mode", charging_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Error Text", error_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Warning Text", warning_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Tracking Mode", tracking_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Device Mode", device_mode_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Firmware Version", firmware_version_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Device Type", device_type_text_sensor_);
  LOG_BINARY_SENSOR("  ", "Load state", load_state_binary_sensor_);
  LOG_BINARY_SENSOR("  ", "Relay state", relay_state_binary_sensor_);

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
        // The checksum is used as end of frame indicator
        if (now - this->last_publish_ >= this->throttle_) {
          this->last_publish_ = now;
          this->publishing_ = true;
        } else {
          this->publishing_ = false;
        }
        continue;
      }
      if ((c == '\r') || (c == '\n')) {
        if (this->publishing_) {
          handle_value_();
        }
        state_ = 0;
      } else {
        value_.push_back(c);
      }
    }
  }
}

static const std::string charging_mode_text(int value) {
  switch (value) {
    case 0:
      return "Off";
    case 2:
      return "Fault";
    case 3:
      return "Bulk";
    case 4:
      return "Absorption";
    case 5:
      return "Float";
    case 7:
      return "Equalize (manual)";
    case 9:
      return "Inverting";
    case 245:
      return "Starting-up";
    case 247:
      return "Auto equalize / Recondition";
    case 252:
      return "External control";
    default:
      return "Unknown";
  }
}

static const std::string error_code_text(int value) {
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

static const std::string warning_code_text(int value) {
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

static const std::string tracking_mode_text(int value) {
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

static const std::string device_mode_text(int value) {
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

static const std::string device_type_text(int value) {
  switch (value) {
    case 0x203:
      return "BMV-700";
    case 0x204:
      return "BMV-702";
    case 0x205:
      return "BMV-700H";
    case 0xA389:
      return "SmartShunt";
    case 0xA381:
      return "BMV-712 Smart";
    case 0xA04C:
      return "BlueSolar MPPT 75/10";
    case 0x300:
      return "BlueSolar MPPT 70/15";
    case 0xA042:
      return "BlueSolar MPPT 75/15";
    case 0xA043:
      return "BlueSolar MPPT 100/15";
    case 0xA044:
      return "BlueSolar MPPT 100/30 rev1";
    case 0xA04A:
      return "BlueSolar MPPT 100/30 rev2";
    case 0xA041:
      return "BlueSolar MPPT 150/35 rev1";
    case 0xA04B:
      return "BlueSolar MPPT 150/35 rev2";
    case 0xA04D:
      return "BlueSolar MPPT 150/45";
    case 0xA040:
      return "BlueSolar MPPT 75/50";
    case 0xA045:
      return "BlueSolar MPPT 100/50 rev1";
    case 0xA049:
      return "BlueSolar MPPT 100/50 rev2";
    case 0xA04E:
      return "BlueSolar MPPT 150/60";
    case 0xA046:
      return "BlueSolar MPPT 150/70";
    case 0xA04F:
      return "BlueSolar MPPT 150/85";
    case 0xA047:
      return "BlueSolar MPPT 150/100";
    case 0xA050:
      return "SmartSolar MPPT 250/100";
    case 0xA051:
      return "SmartSolar MPPT 150/100";
    case 0xA052:
      return "SmartSolar MPPT 150/85";
    case 0xA053:
      return "SmartSolar MPPT 75/15";
    case 0xA054:
      return "SmartSolar MPPT 75/10";
    case 0xA055:
      return "SmartSolar MPPT 100/15";
    case 0xA056:
      return "SmartSolar MPPT 100/30";
    case 0xA057:
      return "SmartSolar MPPT 100/50";
    case 0xA058:
      return "SmartSolar MPPT 150/35";
    case 0xA059:
      return "SmartSolar MPPT 150/100 rev2";
    case 0xA05A:
      return "SmartSolar MPPT 150/85 rev2";
    case 0xA05B:
      return "SmartSolar MPPT 250/70";
    case 0xA05C:
      return "SmartSolar MPPT 250/85";
    case 0xA05D:
      return "SmartSolar MPPT 250/60";
    case 0xA05E:
      return "SmartSolar MPPT 250/45";
    case 0xA05F:
      return "SmartSolar MPPT 100/20";
    case 0xA060:
      return "SmartSolar MPPT 100/20 48V";
    case 0xA061:
      return "SmartSolar MPPT 150/45";
    case 0xA062:
      return "SmartSolar MPPT 150/60";
    case 0xA063:
      return "SmartSolar MPPT 150/70";
    case 0xA064:
      return "SmartSolar MPPT 250/85 rev2";
    case 0xA065:
      return "SmartSolar MPPT 250/100 rev2";
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
    default:
      return "Unknown";
  }
}

void VictronComponent::handle_value_() {
  int value;

  if (label_ == "H23") {
    this->publish_state_(max_power_yesterday_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H21") {
    this->publish_state_(max_power_today_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H19") {
    this->publish_state_(yield_total_sensor_, atoi(value_.c_str()) * 10);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H22") {
    this->publish_state_(yield_yesterday_sensor_, atoi(value_.c_str()) * 10);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "H20") {
    this->publish_state_(yield_today_sensor_, atoi(value_.c_str()) * 10);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "VPV") {
    this->publish_state_(panel_voltage_sensor_, atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "PPV") {
    this->publish_state_(panel_power_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "V") {
    this->publish_state_(battery_voltage_sensor_, atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "I") {
    this->publish_state_(battery_current_sensor_, atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "AC_OUT_V") {
    this->publish_state_(ac_out_voltage_sensor_, atoi(value_.c_str()) / 100.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "AC_OUT_I") {
    this->publish_state_(ac_out_current_sensor_, std::max(0.0, atoi(value_.c_str()) / 10.0));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "IL") {
    this->publish_state_(load_current_sensor_, atoi(value_.c_str()) / 1000.0);  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "HSDS") {
    this->publish_state_(day_number_sensor_, atoi(value_.c_str()));  // NOLINT(cert-err34-c)
    return;
  }

  if (label_ == "CS") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(charging_mode_id_sensor_, (float) value);
    this->publish_state_(charging_mode_text_sensor_, charging_mode_text(value));
    return;
  }

  if (label_ == "ERR") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(error_code_sensor_, value);
    this->publish_state_(error_text_sensor_, error_code_text(value));
    return;
  }

  if (label_ == "WARN") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(warning_code_sensor_, value);
    this->publish_state_(warning_text_sensor_, warning_code_text(value));
    return;
  }

  if (label_ == "MPPT") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(tracking_mode_id_sensor_, (float) value);
    this->publish_state_(tracking_mode_text_sensor_, tracking_mode_text(value));
    return;
  }

  if (label_ == "MODE") {
    value = atoi(value_.c_str());  // NOLINT(cert-err34-c)
    this->publish_state_(device_mode_id_sensor_, (float) value);
    this->publish_state_(device_mode_text_sensor_, device_mode_text(value));
    return;
  }

  if (label_ == "FW") {
    this->publish_state_once_(firmware_version_text_sensor_, value_.insert(value_.size() - 2, "."));
    return;
  }

  if (label_ == "PID") {
    this->publish_state_once_(device_type_text_sensor_, device_type_text(strtol(value_.c_str(), nullptr, 0)));
    return;
  }

  if (label_ == "LOAD") {
    this->publish_state_(load_state_binary_sensor_, value_ == "ON");
    return;
  }

  if (label_ == "RELAY") {
    this->publish_state_(relay_state_binary_sensor_, value_ == "ON");
    return;
  }

  ESP_LOGD(TAG, "Unhandled property: %s %s", label_.c_str(), value_.c_str());
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
