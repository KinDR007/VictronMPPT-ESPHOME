#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace victron {

class VictronComponent : public uart::UARTDevice, public Component {
 public:
  void set_max_power_yesterday_sensor(sensor::Sensor *max_power_yesterday_sensor) {
    max_power_yesterday_sensor_ = max_power_yesterday_sensor;
  }
  void set_max_power_today_sensor(sensor::Sensor *max_power_today_sensor) {
    max_power_today_sensor_ = max_power_today_sensor;
  }
  void set_yield_total_sensor(sensor::Sensor *yield_total_sensor) { yield_total_sensor_ = yield_total_sensor; }
  void set_yield_yesterday_sensor(sensor::Sensor *yield_yesterday_sensor) {
    yield_yesterday_sensor_ = yield_yesterday_sensor;
  }
  void set_yield_today_sensor(sensor::Sensor *yield_today_sensor) { yield_today_sensor_ = yield_today_sensor; }
  void set_panel_voltage_sensor(sensor::Sensor *panel_voltage_sensor) { panel_voltage_sensor_ = panel_voltage_sensor; }
  void set_panel_power_sensor(sensor::Sensor *panel_power_sensor) { panel_power_sensor_ = panel_power_sensor; }
  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) {
    battery_voltage_sensor_ = battery_voltage_sensor;
  }
  void set_battery_current_sensor(sensor::Sensor *battery_current_sensor) {
    battery_current_sensor_ = battery_current_sensor;
  }
  void set_ac_out_voltage_sensor(sensor::Sensor *ac_out_voltage_sensor) {
    ac_out_voltage_sensor_ = ac_out_voltage_sensor;
  }
  void set_ac_out_current_sensor(sensor::Sensor *ac_out_current_sensor) {
    ac_out_current_sensor_ = ac_out_current_sensor;
  }
  void set_load_current_sensor(sensor::Sensor *load_current_sensor) { load_current_sensor_ = load_current_sensor; }
  void set_day_number_sensor(sensor::Sensor *day_number_sensor) { day_number_sensor_ = day_number_sensor; }
  void set_charging_mode_id_sensor(sensor::Sensor *charging_mode_id_sensor) {
    charging_mode_id_sensor_ = charging_mode_id_sensor;
  }
  void set_error_code_sensor(sensor::Sensor *error_code_sensor) { error_code_sensor_ = error_code_sensor; }
  void set_warning_code_sensor(sensor::Sensor *warning_code_sensor) { warning_code_sensor_ = warning_code_sensor; }
  void set_tracking_mode_id_sensor(sensor::Sensor *tracking_mode_id_sensor) {
    tracking_mode_id_sensor_ = tracking_mode_id_sensor;
  }
  void set_device_mode_id_sensor(sensor::Sensor *device_mode_id_sensor) {
    device_mode_id_sensor_ = device_mode_id_sensor;
  }

  void set_charging_mode_text_sensor(text_sensor::TextSensor *charging_mode_text_sensor) {
    charging_mode_text_sensor_ = charging_mode_text_sensor;
  }
  void set_error_text_sensor(text_sensor::TextSensor *error_text_sensor) { error_text_sensor_ = error_text_sensor; }
  void set_warning_text_sensor(text_sensor::TextSensor *warning_text_sensor) {
    warning_text_sensor_ = warning_text_sensor;
  }
  void set_tracking_mode_text_sensor(text_sensor::TextSensor *tracking_mode_text_sensor) {
    tracking_mode_text_sensor_ = tracking_mode_text_sensor;
  }
  void set_device_mode_text_sensor(text_sensor::TextSensor *device_mode_text_sensor) {
    device_mode_text_sensor_ = device_mode_text_sensor;
  }
  void set_firmware_version_text_sensor(text_sensor::TextSensor *firmware_version_text_sensor) {
    firmware_version_text_sensor_ = firmware_version_text_sensor;
  }
  void set_device_type_text_sensor(text_sensor::TextSensor *device_type_text_sensor) {
    device_type_text_sensor_ = device_type_text_sensor;
  }
  void set_load_state_text_sensor(text_sensor::TextSensor *load_state_text_sensor) {
    load_state_text_sensor_ = load_state_text_sensor;
  }
  void set_relay_state_text_sensor(text_sensor::TextSensor *relay_state_text_sensor) {
    relay_state_text_sensor_ = relay_state_text_sensor;
  }  

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void handle_value_();

  sensor::Sensor *max_power_yesterday_sensor_{nullptr};
  sensor::Sensor *max_power_today_sensor_{nullptr};
  sensor::Sensor *yield_total_sensor_{nullptr};
  sensor::Sensor *yield_yesterday_sensor_{nullptr};
  sensor::Sensor *yield_today_sensor_{nullptr};
  sensor::Sensor *panel_voltage_sensor_{nullptr};
  sensor::Sensor *panel_power_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
  sensor::Sensor *ac_out_voltage_sensor_{nullptr};
  sensor::Sensor *ac_out_current_sensor_{nullptr};
  sensor::Sensor *load_current_sensor_{nullptr};
  sensor::Sensor *day_number_sensor_{nullptr};
  sensor::Sensor *device_mode_sensor_{nullptr};
  sensor::Sensor *charging_mode_id_sensor_{nullptr};
  sensor::Sensor *error_code_sensor_{nullptr};
  sensor::Sensor *warning_code_sensor_{nullptr};
  sensor::Sensor *tracking_mode_id_sensor_{nullptr};
  sensor::Sensor *device_mode_id_sensor_{nullptr};

  text_sensor::TextSensor *charging_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *error_text_sensor_{nullptr};
  text_sensor::TextSensor *warning_text_sensor_{nullptr};
  text_sensor::TextSensor *tracking_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *device_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *firmware_version_text_sensor_{nullptr};
  text_sensor::TextSensor *device_type_text_sensor_{nullptr};
  text_sensor::TextSensor *load_state_text_sensor_{nullptr};
  text_sensor::TextSensor *relay_state_text_sensor_{nullptr};

  int state_{0};
  std::string label_;
  std::string value_;
  uint32_t last_transmission_{0};
};

}  // namespace victron
}  // namespace esphome
