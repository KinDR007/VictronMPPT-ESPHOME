#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-multi-rs-solar.yaml
// Multi RS Solar 48V 6000VA 230V (PID 0xA442), FWE 1.09-official, SER# HQ2206WVU9D
// Decoded key values:
//   battery_voltage: 56.65 V          battery_current: -3.2 A
//   panel_voltage: 190.09 V           panel_power: 0 W
//   charging_mode_id: 6 ("Storage")   error_code: 0 ("No error")
//   ac_out_current: 1.0 A (max(0, 10 / 10.0))
//   ac_out_voltage: 230.01 V (23001 / 100.0)
//   yield_total: 25510 Wh             yield_today: 2170 Wh
//   max_power_today: 1289 W           yield_yesterday: 2640 Wh
//   max_power_yesterday: 1404 W       day_number: 5
//   device_type: "Multi RS Solar 48V 6000VA 230V"
//   firmware_version_24bit: "1.09-official"
//   serial_number: "HQ2206WVU9D"
static const VeDirectKV MULTI_RS_SOLAR_FRAME[] = {
    {"PID", "0xA442"},  {"SER#", "HQ2206WVU9D"}, {"FWE", "109FF"}, {"CS", "6"},    {"ERR", "0"},
    {"AC_OUT_I", "10"}, {"AC_OUT_V", "23001"},   {"H19", "2551"},  {"H20", "217"}, {"H21", "1289"},
    {"H22", "264"},     {"H23", "1404"},         {"HSDS", "5"},    {"I", "-3200"}, {"PPV", "0"},
    {"V", "56650"},     {"VPV", "190090"},
};

}  // namespace esphome::victron::testing
