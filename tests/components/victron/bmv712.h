#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-bmv712.yaml (first complete frame including history)
// BMV-712 Smart (PID 0xA381), FW 04.08, model "712 Smart"
// Decoded key values:
//   battery_voltage: 26.464 V         auxiliary_battery_voltage: 0.034 V
//   battery_current: -5.647 A         instantaneous_power: -149 W
//   consumed_amp_hours: -6.671 Ah     state_of_charge: 93.7 %
//   time_to_go: 436 min               alarm_condition: "OFF"
//   relay_state: false                alarm_reason: "No error"
//   model_description: "712 Smart"    firmware_version: "04.08"
//   dc_monitor_mode_id: 0 ("Battery monitor (BMV)")
//   device_type: "BMV-712 Smart"
//   depth_deepest_discharge: -96.47 Ah  depth_last_discharge: -6.671 Ah
//   depth_average_discharge: -26.485 Ah number_of_charge_cycles: 29
//   number_of_full_discharges: 4      cumulative_amp_hours: -2316.635 Ah
//   min_battery_voltage: 0.027 V      max_battery_voltage: 28.776 V
//   last_full_charge: 118.9 min       number_of_auto_syncs: 31
//   low_main_voltage_alarms: 0        high_main_voltage_alarms: 0
//   min_aux_battery_voltage: 0.022 V  max_aux_battery_voltage: 0.038 V
//   amount_discharged_energy: 60660 Wh  amount_charged_energy: 61170 Wh
static const VeDirectKV BMV712_FRAME[] = {
    {"PID", "0xA381"}, {"V", "26464"},     {"VS", "34"},     {"I", "-5647"},   {"P", "-149"},    {"CE", "-6671"},
    {"SOC", "937"},    {"TTG", "436"},     {"Alarm", "OFF"}, {"Relay", "OFF"}, {"AR", "0"},      {"BMV", "712 Smart"},
    {"FW", "0408"},    {"MON", "0"},       {"H1", "-96470"}, {"H2", "-6671"},  {"H3", "-26485"}, {"H4", "29"},
    {"H5", "4"},       {"H6", "-2316635"}, {"H7", "27"},     {"H8", "28776"},  {"H9", "7134"},   {"H10", "31"},
    {"H11", "0"},      {"H12", "0"},       {"H15", "22"},    {"H16", "38"},    {"H17", "6066"},  {"H18", "6117"},
};

}  // namespace esphome::victron::testing
