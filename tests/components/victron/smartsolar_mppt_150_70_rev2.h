#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-smartsolar-mppt-150-70-rev3.yaml
// SmartSolar MPPT 150|70 rev2 (PID 0xA06C), FWE 3.16-official, SER# HQ2247EP3FN
// Decoded key values:
//   battery_voltage: 52.70 V          battery_current: 7.60 A
//   panel_voltage: 103.87 V           panel_power: 412 W
//   tracking_mode_id: 2 ("Active")    charging_mode_id: 3 ("Bulk")
//   off_reason_bitmask: 0             off_reason: ""
//   error_code: 0 ("No error")        load_state: true
//   relay_state: false
//   yield_total: 11580 Wh             yield_today: 770 Wh
//   max_power_today: 479 W            yield_yesterday: 1350 Wh
//   max_power_yesterday: 525 W        day_number: 2
static const VeDirectKV SMARTSOLAR_MPPT_150_70_REV2_FRAME[] = {
    {"PID", "0xA06C"}, {"FWE", "316FF"}, {"SER#", "HQ2247EP3FN"}, {"V", "52700"},  {"I", "7600"},
    {"VPV", "103870"}, {"PPV", "412"},   {"MPPT", "2"},           {"CS", "3"},     {"OR", "0x00000000"},
    {"ERR", "0"},      {"LOAD", "ON"},   {"Relay", "OFF"},        {"H19", "1158"}, {"H20", "77"},
    {"H21", "479"},    {"H22", "135"},   {"H23", "525"},          {"HSDS", "2"},
};

}  // namespace esphome::victron::testing
