#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-bluesmart-ip22-0xa332.yaml
// Blue Smart IP22 Charger 24|8 (PID 0xA332), FWE 03.42-official, SER# HQ2249TNFVM
// Decoded key values:
//   battery_voltage: 26.87 V          battery_current: 2.0 A
//   battery_temperature: NAN          error_code: 0 ("No error")
//   charging_mode_id: 3 ("Bulk")      hardware_revision: "57825"
//   device_type: "Blue Smart IP22 Charger 24|8"
//   firmware_version_24bit: "03.42-official"
//   serial_number: "HQ2249TNFVM"
static const VeDirectKV BLUESMART_IP22_FRAME[] = {
    {"PID", "0xA332"}, {"FWE", "0342FF"}, {"SER#", "HQ2249TNFVM"}, {"V", "26870"}, {"I", "2000"}, {"T", "---"},
    {"ERR", "0"},      {"CS", "3"},       {"HC#", "57825"},
};

}  // namespace esphome::victron::testing
