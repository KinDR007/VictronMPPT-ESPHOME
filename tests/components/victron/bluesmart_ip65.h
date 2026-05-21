#pragma once
#include "common.h"

namespace esphome::victron::testing {

// Source: tests/fake-bluesmart-ip65-0xa30a.yaml (first frame)
// Blue Smart IP65 Charger 12|25 (PID 0xA30A), FWE 03.25-official, SER# HQ2232GGMEH
// Decoded key values:
//   battery_voltage: 14.2 V           battery_current: 1.0 A
//   error_code: 0 ("No error")        charging_mode_id: 4 ("Absorption")
//   hardware_revision: "136663"
//   device_type: "Blue Smart IP65 Charger 12|25"
//   firmware_version_24bit: "03.25-official"
//   serial_number: "HQ2232GGMEH"
static const VeDirectKV BLUESMART_IP65_FRAME[] = {
    {"PID", "0xA30A"}, {"FWE", "0325FF"}, {"SER#", "HQ2232GGMEH"}, {"V", "14200"}, {"I", "1000"},
    {"ERR", "0"},      {"CS", "4"},       {"HC#", "136663"},
};

}  // namespace esphome::victron::testing
