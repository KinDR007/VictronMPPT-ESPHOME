#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-smartsolar-mppt.yaml
// SmartSolar MPPT 75|15 (PID 0xA053), FW 1.56, SER# HQ1942K7LJ8
// Decoded key values:
//   battery_voltage: 12.45 V          battery_current: 0.0 A
//   panel_voltage: 0.01 V             panel_power: 0 W
//   charging_mode_id: 0 ("Off")       tracking_mode_id: 0 ("Off")
//   off_reason_bitmask: 1             off_reason: "No input power"
//   error_code: 0 ("No error")        load_state: true
//   load_current: 0.0 A
//   yield_total: 30 Wh                yield_today: 0 Wh
//   max_power_today: 24 W             yield_yesterday: 10 Wh
//   max_power_yesterday: 15 W         day_number: 3
static const VeDirectKV SMARTSOLAR_MPPT_FRAME[] = {
    {"PID", "0xA053"},    {"FW", "156"}, {"SER#", "HQ1942K7LJ8"},
    {"V", "12450"},       {"I", "0"},    {"VPV", "10"},
    {"PPV", "0"},         {"CS", "0"},   {"MPPT", "0"},
    {"OR", "0x00000001"}, {"ERR", "0"},  {"LOAD", "ON"},
    {"IL", "0"},          {"H19", "3"},  {"H20", "0"},
    {"H21", "24"},        {"H22", "1"},  {"H23", "15"},
    {"HSDS", "3"},
};

}  // namespace esphome::victron::testing
