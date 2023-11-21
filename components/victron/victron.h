#pragma once

#include <vector>
#include <utility>
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace victron {

class VictronComponent : public uart::UARTDevice, public Component {
 public:
  void set_throttle(uint32_t throttle) { this->throttle_ = throttle; }
  void set_async_uart(bool async_uart) { this->async_uart_ = async_uart; }
  void set_load_state_binary_sensor(binary_sensor::BinarySensor *load_state_binary_sensor) {
    load_state_binary_sensor_ = load_state_binary_sensor;
  }
  void set_relay_state_binary_sensor(binary_sensor::BinarySensor *relay_state_binary_sensor) {
    relay_state_binary_sensor_ = relay_state_binary_sensor;
  }
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
  void set_battery_voltage_2_sensor(sensor::Sensor *battery_voltage_2_sensor) {
    battery_voltage_2_sensor_ = battery_voltage_2_sensor;
  }
  void set_battery_voltage_3_sensor(sensor::Sensor *battery_voltage_3_sensor) {
    battery_voltage_3_sensor_ = battery_voltage_3_sensor;
  }
  void set_auxiliary_battery_voltage_sensor(sensor::Sensor *auxiliary_battery_voltage_sensor) {
    auxiliary_battery_voltage_sensor_ = auxiliary_battery_voltage_sensor;
  }
  void set_midpoint_voltage_of_the_battery_bank_sensor(sensor::Sensor *midpoint_voltage_of_the_battery_bank_sensor) {
    midpoint_voltage_of_the_battery_bank_sensor_ = midpoint_voltage_of_the_battery_bank_sensor;
  }
  void set_midpoint_deviation_of_the_battery_bank_sensor(
      sensor::Sensor *midpoint_deviation_of_the_battery_bank_sensor) {
    midpoint_deviation_of_the_battery_bank_sensor_ = midpoint_deviation_of_the_battery_bank_sensor;
  }
  void set_battery_current_sensor(sensor::Sensor *battery_current_sensor) {
    battery_current_sensor_ = battery_current_sensor;
  }
  void set_battery_current_2_sensor(sensor::Sensor *battery_current_2_sensor) {
    battery_current_2_sensor_ = battery_current_2_sensor;
  }
  void set_battery_current_3_sensor(sensor::Sensor *battery_current_3_sensor) {
    battery_current_3_sensor_ = battery_current_3_sensor;
  }
  void set_ac_out_voltage_sensor(sensor::Sensor *ac_out_voltage_sensor) {
    ac_out_voltage_sensor_ = ac_out_voltage_sensor;
  }
  void set_ac_out_current_sensor(sensor::Sensor *ac_out_current_sensor) {
    ac_out_current_sensor_ = ac_out_current_sensor;
  }
  void set_ac_out_apparent_power_sensor(sensor::Sensor *ac_out_apparent_power_sensor) {
    ac_out_apparent_power_sensor_ = ac_out_apparent_power_sensor;
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
  void set_dc_monitor_mode_id_sensor(sensor::Sensor *dc_monitor_mode_id_sensor) {
    dc_monitor_mode_id_sensor_ = dc_monitor_mode_id_sensor;
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
  void set_firmware_version_24bit_text_sensor(text_sensor::TextSensor *firmware_version_24bit_text_sensor) {
    firmware_version_24bit_text_sensor_ = firmware_version_24bit_text_sensor;
  }
  void set_device_type_text_sensor(text_sensor::TextSensor *device_type_text_sensor) {
    device_type_text_sensor_ = device_type_text_sensor;
  }
  void set_serial_number_text_sensor(text_sensor::TextSensor *serial_number_text_sensor) {
    serial_number_text_sensor_ = serial_number_text_sensor;
  }
  void set_dc_monitor_mode_text_sensor(text_sensor::TextSensor *dc_monitor_mode_text_sensor) {
    dc_monitor_mode_text_sensor_ = dc_monitor_mode_text_sensor;
  }

  void set_battery_temperature_sensor(sensor::Sensor *battery_temperature_sensor) {
    battery_temperature_sensor_ = battery_temperature_sensor;
  }
  void set_instantaneous_power_sensor(sensor::Sensor *instantaneous_power_sensor) {
    instantaneous_power_sensor_ = instantaneous_power_sensor;
  }
  void set_consumed_amp_hours_sensor(sensor::Sensor *consumed_amp_hours_sensor) {
    consumed_amp_hours_sensor_ = consumed_amp_hours_sensor;
  }
  void set_state_of_charge_sensor(sensor::Sensor *state_of_charge_sensor) {
    state_of_charge_sensor_ = state_of_charge_sensor;
  }
  void set_time_to_go_sensor(sensor::Sensor *time_to_go_sensor) { time_to_go_sensor_ = time_to_go_sensor; }
  void set_depth_of_the_deepest_discharge_sensor(sensor::Sensor *depth_of_the_deepest_discharge_sensor) {
    depth_of_the_deepest_discharge_sensor_ = depth_of_the_deepest_discharge_sensor;
  }
  void set_depth_of_the_last_discharge_sensor(sensor::Sensor *depth_of_the_last_discharge_sensor) {
    depth_of_the_last_discharge_sensor_ = depth_of_the_last_discharge_sensor;
  }
  void set_depth_of_the_average_discharge_sensor(sensor::Sensor *depth_of_the_average_discharge_sensor) {
    depth_of_the_average_discharge_sensor_ = depth_of_the_average_discharge_sensor;
  }
  void set_number_of_charge_cycles_sensor(sensor::Sensor *number_of_charge_cycles_sensor) {
    number_of_charge_cycles_sensor_ = number_of_charge_cycles_sensor;
  }
  void set_number_of_full_discharges_sensor(sensor::Sensor *number_of_full_discharges_sensor) {
    number_of_full_discharges_sensor_ = number_of_full_discharges_sensor;
  }
  void set_cumulative_amp_hours_drawn_sensor(sensor::Sensor *cumulative_amp_hours_drawn_sensor) {
    cumulative_amp_hours_drawn_sensor_ = cumulative_amp_hours_drawn_sensor;
  }
  void set_min_battery_voltage_sensor(sensor::Sensor *min_battery_voltage_sensor) {
    min_battery_voltage_sensor_ = min_battery_voltage_sensor;
  }
  void set_max_battery_voltage_sensor(sensor::Sensor *max_battery_voltage_sensor) {
    max_battery_voltage_sensor_ = max_battery_voltage_sensor;
  }
  void set_last_full_charge_sensor(sensor::Sensor *last_full_charge_sensor) {
    last_full_charge_sensor_ = last_full_charge_sensor;
  }
  void set_number_of_automatic_synchronizations_sensor(sensor::Sensor *number_of_automatic_synchronizations_sensor) {
    number_of_automatic_synchronizations_sensor_ = number_of_automatic_synchronizations_sensor;
  }
  void set_number_of_low_main_voltage_alarms_sensor(sensor::Sensor *number_of_low_main_voltage_alarms_sensor) {
    number_of_low_main_voltage_alarms_sensor_ = number_of_low_main_voltage_alarms_sensor;
  }
  void set_number_of_high_main_voltage_alarms_sensor(sensor::Sensor *number_of_high_main_voltage_alarms_sensor) {
    number_of_high_main_voltage_alarms_sensor_ = number_of_high_main_voltage_alarms_sensor;
  }
  void set_number_of_low_auxiliary_voltage_alarms_sensor(
      sensor::Sensor *number_of_low_auxiliary_voltage_alarms_sensor) {
    number_of_low_auxiliary_voltage_alarms_sensor_ = number_of_low_auxiliary_voltage_alarms_sensor;
  }
  void set_number_of_high_auxiliary_voltage_alarms_sensor(
      sensor::Sensor *number_of_high_auxiliary_voltage_alarms_sensor) {
    number_of_high_auxiliary_voltage_alarms_sensor_ = number_of_high_auxiliary_voltage_alarms_sensor;
  }
  void set_min_auxiliary_battery_voltage_sensor(sensor::Sensor *min_auxiliary_battery_voltage_sensor) {
    min_auxiliary_battery_voltage_sensor_ = min_auxiliary_battery_voltage_sensor;
  }
  void set_max_auxiliary_battery_voltage_sensor(sensor::Sensor *max_auxiliary_battery_voltage_sensor) {
    max_auxiliary_battery_voltage_sensor_ = max_auxiliary_battery_voltage_sensor;
  }
  void set_amount_of_discharged_energy_sensor(sensor::Sensor *amount_of_discharged_energy_sensor) {
    amount_of_discharged_energy_sensor_ = amount_of_discharged_energy_sensor;
  }
  void set_amount_of_charged_energy_sensor(sensor::Sensor *amount_of_charged_energy_sensor) {
    amount_of_charged_energy_sensor_ = amount_of_charged_energy_sensor;
  }
  void set_off_reason_bitmask_sensor(sensor::Sensor *off_reason_bitmask_sensor) {
    off_reason_bitmask_sensor_ = off_reason_bitmask_sensor;
  }

  void set_alarm_condition_active_text_sensor(text_sensor::TextSensor *alarm_condition_active_text_sensor) {
    alarm_condition_active_text_sensor_ = alarm_condition_active_text_sensor;
  }
  void set_alarm_reason_text_sensor(text_sensor::TextSensor *alarm_reason_text_sensor) {
    alarm_reason_text_sensor_ = alarm_reason_text_sensor;
  }
  void set_model_description_text_sensor(text_sensor::TextSensor *model_description_text_sensor) {
    model_description_text_sensor_ = model_description_text_sensor;
  }
  void set_off_reason_text_sensor(text_sensor::TextSensor *off_reason_text_sensor) {
    off_reason_text_sensor_ = off_reason_text_sensor;
  }

  void dump_config() override;
  void loop() override;
  void async_loop();
  void blocking_loop();

  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void handle_value_(std::string l, std::string v);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void publish_state_once_(text_sensor::TextSensor *text_sensor, const std::string &state);

  binary_sensor::BinarySensor *load_state_binary_sensor_;
  binary_sensor::BinarySensor *relay_state_binary_sensor_;

  sensor::Sensor *max_power_yesterday_sensor_{nullptr};
  sensor::Sensor *max_power_today_sensor_{nullptr};
  sensor::Sensor *yield_total_sensor_{nullptr};
  sensor::Sensor *yield_yesterday_sensor_{nullptr};
  sensor::Sensor *yield_today_sensor_{nullptr};
  sensor::Sensor *panel_voltage_sensor_{nullptr};
  sensor::Sensor *panel_power_sensor_{nullptr};
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_voltage_2_sensor_{nullptr};
  sensor::Sensor *battery_voltage_3_sensor_{nullptr};
  sensor::Sensor *auxiliary_battery_voltage_sensor_{nullptr};
  sensor::Sensor *midpoint_voltage_of_the_battery_bank_sensor_{nullptr};
  sensor::Sensor *midpoint_deviation_of_the_battery_bank_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
  sensor::Sensor *battery_current_2_sensor_{nullptr};
  sensor::Sensor *battery_current_3_sensor_{nullptr};
  sensor::Sensor *ac_out_voltage_sensor_{nullptr};
  sensor::Sensor *ac_out_current_sensor_{nullptr};
  sensor::Sensor *ac_out_apparent_power_sensor_{nullptr};
  sensor::Sensor *load_current_sensor_{nullptr};
  sensor::Sensor *day_number_sensor_{nullptr};
  sensor::Sensor *device_mode_sensor_{nullptr};
  sensor::Sensor *charging_mode_id_sensor_{nullptr};
  sensor::Sensor *error_code_sensor_{nullptr};
  sensor::Sensor *warning_code_sensor_{nullptr};
  sensor::Sensor *tracking_mode_id_sensor_{nullptr};
  sensor::Sensor *device_mode_id_sensor_{nullptr};
  sensor::Sensor *dc_monitor_mode_id_sensor_{nullptr};
  sensor::Sensor *off_reason_bitmask_sensor_{nullptr};
  text_sensor::TextSensor *charging_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *error_text_sensor_{nullptr};
  text_sensor::TextSensor *warning_text_sensor_{nullptr};
  text_sensor::TextSensor *tracking_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *device_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *firmware_version_text_sensor_{nullptr};
  text_sensor::TextSensor *firmware_version_24bit_text_sensor_{nullptr};
  text_sensor::TextSensor *device_type_text_sensor_{nullptr};
  text_sensor::TextSensor *serial_number_text_sensor_{nullptr};
  text_sensor::TextSensor *dc_monitor_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *off_reason_text_sensor_{nullptr};

  sensor::Sensor *battery_temperature_sensor_{nullptr};
  sensor::Sensor *instantaneous_power_sensor_{nullptr};
  sensor::Sensor *consumed_amp_hours_sensor_{nullptr};
  sensor::Sensor *state_of_charge_sensor_{nullptr};
  sensor::Sensor *time_to_go_sensor_{nullptr};
  sensor::Sensor *depth_of_the_deepest_discharge_sensor_{nullptr};
  sensor::Sensor *depth_of_the_last_discharge_sensor_{nullptr};
  sensor::Sensor *depth_of_the_average_discharge_sensor_{nullptr};
  sensor::Sensor *number_of_charge_cycles_sensor_{nullptr};
  sensor::Sensor *number_of_full_discharges_sensor_{nullptr};
  sensor::Sensor *cumulative_amp_hours_drawn_sensor_{nullptr};
  sensor::Sensor *min_battery_voltage_sensor_{nullptr};
  sensor::Sensor *max_battery_voltage_sensor_{nullptr};
  sensor::Sensor *last_full_charge_sensor_{nullptr};
  sensor::Sensor *number_of_automatic_synchronizations_sensor_{nullptr};
  sensor::Sensor *number_of_low_main_voltage_alarms_sensor_{nullptr};
  sensor::Sensor *number_of_high_main_voltage_alarms_sensor_{nullptr};
  sensor::Sensor *number_of_low_auxiliary_voltage_alarms_sensor_{nullptr};
  sensor::Sensor *number_of_high_auxiliary_voltage_alarms_sensor_{nullptr};
  sensor::Sensor *min_auxiliary_battery_voltage_sensor_{nullptr};
  sensor::Sensor *max_auxiliary_battery_voltage_sensor_{nullptr};
  sensor::Sensor *amount_of_discharged_energy_sensor_{nullptr};
  sensor::Sensor *amount_of_charged_energy_sensor_{nullptr};
  text_sensor::TextSensor *alarm_condition_active_text_sensor_{nullptr};
  text_sensor::TextSensor *alarm_reason_text_sensor_{nullptr};
  text_sensor::TextSensor *model_description_text_sensor_{nullptr};

  int state_{0};
  bool async_uart_{false};
  bool publishing_{false};
  std::string label_;
  std::string value_;
  uint32_t begin_frame_{0};
  uint32_t last_transmission_{0};
  uint32_t last_publish_{0};
  uint32_t throttle_{0};
  uint8_t checksum_{0};
  std::vector<std::pair<std::string, std::string>> recv_buffer_{};
};

}  // namespace victron
}  // namespace esphome
