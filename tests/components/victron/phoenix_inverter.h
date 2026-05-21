#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-phoenix-inverter.yaml
// Phoenix Inverter 12V 250VA 230V (PID 0xA201), SER# HQ141112345
// Decoded key values:
//   ac_out_current: 5.0 A (max(0, 50 / 10.0))
//   ac_out_voltage: 230.0 V (23000 / 100.0)
//   alarm_reason: error_code_text(1) = "Unknown"
//   charging_mode_id: 5 ("Float")
//   device_mode_id: 2 ("On")
//   warning_code: 1 ("Low Voltage")
//   device_type: "Phoenix Inverter 12V 250VA 230V"
//   serial_number: "HQ141112345"
static const VeDirectKV PHOENIX_INVERTER_FRAME[] = {
    {"AC_OUT_I", "50"}, {"AC_OUT_V", "23000"},   {"AR", "1"},   {"CS", "5"}, {"MODE", "2"},
    {"PID", "0xA201"},  {"SER#", "HQ141112345"}, {"WARN", "1"},
};

}  // namespace esphome::victron::testing
