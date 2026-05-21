#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-bmv700.yaml
// BMV-702 (PID 0x204), FW 1.19, model "702"
// Decoded key values:
//   battery_voltage: 12.80 V          auxiliary_battery_voltage: 12.80 V
//   midpoint_voltage: 1.28 V          midpoint_deviation: 12.0 %
//   battery_current: 15.0 A           battery_temperature: 25 °C
//   instantaneous_power: 130 W        consumed_amp_hours: 13.5 Ah
//   state_of_charge: 87.6 %           time_to_go: 45 min
//   alarm_condition: "OFF"            relay_state: false
//   alarm_reason: error_code_text(1) = "Unknown"
//   depth_deepest_discharge: 55.0 Ah  depth_last_discharge: 15.0 Ah
//   depth_average_discharge: 13.0 Ah  number_of_charge_cycles: 230
//   number_of_full_discharges: 12     cumulative_amp_hours: 234.0 Ah
//   min_battery_voltage: 11.0 V       max_battery_voltage: 14.8 V
//   last_full_charge: 120.0 min       number_of_auto_syncs: 45
//   low_main_voltage_alarms: 5        high_main_voltage_alarms: 0
//   min_aux_battery_voltage: 11.5 V   max_aux_battery_voltage: 14.8 V
//   amount_discharged_energy: 340 Wh  amount_charged_energy: 450 Wh
//   model_description: "702"          device_type: "BMV-702"
static const VeDirectKV BMV_FRAME[] = {
    {"V", "12800"},   {"VS", "12800"},  {"VM", "1280"},   {"DM", "120"},   {"I", "15000"},   {"T", "25"},
    {"P", "130"},     {"CE", "13500"},  {"SOC", "876"},   {"TTG", "45"},   {"Alarm", "OFF"}, {"Relay", "OFF"},
    {"AR", "1"},      {"H1", "55000"},  {"H2", "15000"},  {"H3", "13000"}, {"H4", "230"},    {"H5", "12"},
    {"H6", "234000"}, {"H7", "11000"},  {"H8", "14800"},  {"H9", "7200"},  {"H10", "45"},    {"H11", "5"},
    {"H12", "0"},     {"H15", "11500"}, {"H16", "14800"}, {"H17", "34"},   {"H18", "45"},    {"BMV", "702"},
    {"PID", "0x204"},
};

// Source: tests/fake-bmv710.yaml
// BMV-702 with DC monitor mode field (PID 0x204)
// Additional field vs BMV_FRAME:
//   dc_monitor_mode_id: -1 ("Generic source")
static const VeDirectKV BMV_WITH_DC_MONITOR_FRAME[] = {
    {"V", "12800"},   {"VS", "12800"},  {"VM", "1280"},   {"DM", "120"},   {"I", "15000"},   {"T", "25"},
    {"P", "130"},     {"CE", "13500"},  {"SOC", "876"},   {"TTG", "45"},   {"Alarm", "OFF"}, {"Relay", "OFF"},
    {"AR", "1"},      {"H1", "55000"},  {"H2", "15000"},  {"H3", "13000"}, {"H4", "230"},    {"H5", "12"},
    {"H6", "234000"}, {"H7", "11000"},  {"H8", "14800"},  {"H9", "7200"},  {"H10", "45"},    {"H11", "5"},
    {"H12", "0"},     {"H15", "11500"}, {"H16", "14800"}, {"H17", "34"},   {"H18", "45"},    {"BMV", "702"},
    {"PID", "0x204"}, {"MON", "-1"},
};

}  // namespace esphome::victron::testing
