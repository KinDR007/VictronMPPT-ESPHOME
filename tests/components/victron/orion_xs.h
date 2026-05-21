#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-orion-xs-0xa3f0.yaml (first frame)
// Orion XS 12V/12V-50A (PID 0xA3F0), FWE 1.12-official, SER# HQ2433EWGZZ
// Decoded key values:
//   battery_voltage: 13.22 V          battery_current: 0.0 A
//   instantaneous_power: 0 W
//   dc_input_voltage: 12.62 V (1262 / 100.0)
//   dc_input_current: 0.0 A (0 / 10.0)
//   dc_input_power: 0 W
//   charging_mode_id: 0 ("Off")       error_code: 0 ("No error")
//   off_reason_bitmask: 9 (bits 0+3)
//   off_reason: "No input power;Remote input"
//   device_type: "Orion XS 12V/12V-50A"
//   firmware_version_24bit: "1.12-official"
//   serial_number: "HQ2433EWGZZ"
static const VeDirectKV ORION_XS_FRAME[] = {
    {"PID", "0xA3F0"},   {"FWE", "112FF"}, {"SER#", "HQ2433EWGZZ"}, {"V", "13220"}, {"I", "0"},           {"P", "0"},
    {"DC_IN_V", "1262"}, {"DC_IN_I", "0"}, {"DC_IN_P", "0"},        {"CS", "0"},    {"OR", "0x00000009"}, {"ERR", "0"},
};

}  // namespace esphome::victron::testing
