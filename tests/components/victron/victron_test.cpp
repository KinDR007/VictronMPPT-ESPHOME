#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "smartsolar_mppt.h"
#include "smartsolar_mppt_150_70_rev2.h"
#include "bmv.h"
#include "bmv712.h"
#include "bluesmart_ip22.h"
#include "bluesmart_ip65.h"
#include "phoenix_inverter.h"
#include "multi_rs_solar.h"
#include "orion_xs.h"

namespace esphome::victron::testing {

// ── SmartSolar MPPT 75|15 ─────────────────────────────────────────────────────

TEST(VictronSmartSolarMpptTest, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, panel_v, panel_p, load_i;
  binary_sensor::BinarySensor load;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_panel_voltage_sensor(&panel_v);
  v.set_panel_power_sensor(&panel_p);
  v.set_load_current_sensor(&load_i);
  v.set_load_state_binary_sensor(&load);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_NEAR(bat_v.state, 12.45f, 0.001f);
  EXPECT_NEAR(bat_i.state, 0.0f, 0.001f);
  EXPECT_NEAR(panel_v.state, 0.01f, 0.001f);
  EXPECT_FLOAT_EQ(panel_p.state, 0.0f);
  EXPECT_NEAR(load_i.state, 0.0f, 0.001f);
  EXPECT_TRUE(load.state);
}

TEST(VictronSmartSolarMpptTest, ChargingMode) {
  TestableVictron v;
  sensor::Sensor cs_id, mppt_id;
  text_sensor::TextSensor cs_text, mppt_text;
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);
  v.set_tracking_mode_id_sensor(&mppt_id);
  v.set_tracking_mode_text_sensor(&mppt_text);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_FLOAT_EQ(cs_id.state, 0.0f);
  EXPECT_EQ(cs_text.state, "Off");
  EXPECT_FLOAT_EQ(mppt_id.state, 0.0f);
  EXPECT_EQ(mppt_text.state, "Off");
}

TEST(VictronSmartSolarMpptTest, OffReason) {
  TestableVictron v;
  sensor::Sensor bitmask;
  text_sensor::TextSensor reason;
  v.set_off_reason_bitmask_sensor(&bitmask);
  v.set_off_reason_text_sensor(&reason);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_FLOAT_EQ(bitmask.state, 1.0f);
  EXPECT_EQ(reason.state, "No input power");
}

TEST(VictronSmartSolarMpptTest, ErrorCode) {
  TestableVictron v;
  sensor::Sensor err_id;
  text_sensor::TextSensor err_text;
  v.set_error_code_sensor(&err_id);
  v.set_error_text_sensor(&err_text);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_FLOAT_EQ(err_id.state, 0.0f);
  EXPECT_EQ(err_text.state, "No error");
}

TEST(VictronSmartSolarMpptTest, YieldHistory) {
  TestableVictron v;
  sensor::Sensor h19, h20, h21, h22, h23, hsds;
  v.set_yield_total_sensor(&h19);
  v.set_yield_today_sensor(&h20);
  v.set_max_power_today_sensor(&h21);
  v.set_yield_yesterday_sensor(&h22);
  v.set_max_power_yesterday_sensor(&h23);
  v.set_day_number_sensor(&hsds);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_NEAR(h19.state, 30.0f, 0.1f);
  EXPECT_NEAR(h20.state, 0.0f, 0.1f);
  EXPECT_FLOAT_EQ(h21.state, 24.0f);
  EXPECT_NEAR(h22.state, 10.0f, 0.1f);
  EXPECT_FLOAT_EQ(h23.state, 15.0f);
  EXPECT_FLOAT_EQ(hsds.state, 3.0f);
}

TEST(VictronSmartSolarMpptTest, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fw, serial;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_text_sensor(&fw);
  v.set_serial_number_text_sensor(&serial);

  process_frame(v, SMARTSOLAR_MPPT_FRAME);

  EXPECT_EQ(device_type.state, "SmartSolar MPPT 75|15");
  EXPECT_EQ(fw.state, "1.56");
  EXPECT_EQ(serial.state, "HQ1942K7LJ8");
}

// ── SmartSolar MPPT 150|70 rev2 ───────────────────────────────────────────────

TEST(VictronSmartSolarMppt15070Rev2Test, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, panel_v, panel_p;
  binary_sensor::BinarySensor load, relay;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_panel_voltage_sensor(&panel_v);
  v.set_panel_power_sensor(&panel_p);
  v.set_load_state_binary_sensor(&load);
  v.set_relay_state_binary_sensor(&relay);

  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);

  EXPECT_NEAR(bat_v.state, 52.70f, 0.001f);
  EXPECT_NEAR(bat_i.state, 7.60f, 0.001f);
  EXPECT_NEAR(panel_v.state, 103.87f, 0.01f);
  EXPECT_FLOAT_EQ(panel_p.state, 412.0f);
  EXPECT_TRUE(load.state);
  EXPECT_FALSE(relay.state);
}

TEST(VictronSmartSolarMppt15070Rev2Test, ChargingMode) {
  TestableVictron v;
  sensor::Sensor cs_id, mppt_id;
  text_sensor::TextSensor cs_text, mppt_text;
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);
  v.set_tracking_mode_id_sensor(&mppt_id);
  v.set_tracking_mode_text_sensor(&mppt_text);

  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);

  EXPECT_FLOAT_EQ(cs_id.state, 3.0f);
  EXPECT_EQ(cs_text.state, "Bulk");
  EXPECT_FLOAT_EQ(mppt_id.state, 2.0f);
  EXPECT_EQ(mppt_text.state, "Active");
}

TEST(VictronSmartSolarMppt15070Rev2Test, OffReasonZero) {
  TestableVictron v;
  sensor::Sensor bitmask;
  text_sensor::TextSensor reason;
  v.set_off_reason_bitmask_sensor(&bitmask);
  v.set_off_reason_text_sensor(&reason);

  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);

  EXPECT_FLOAT_EQ(bitmask.state, 0.0f);
  EXPECT_EQ(reason.state, "");
}

TEST(VictronSmartSolarMppt15070Rev2Test, YieldHistory) {
  TestableVictron v;
  sensor::Sensor h19, h20, h21, h22, h23, hsds;
  v.set_yield_total_sensor(&h19);
  v.set_yield_today_sensor(&h20);
  v.set_max_power_today_sensor(&h21);
  v.set_yield_yesterday_sensor(&h22);
  v.set_max_power_yesterday_sensor(&h23);
  v.set_day_number_sensor(&hsds);

  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);

  EXPECT_NEAR(h19.state, 11580.0f, 1.0f);
  EXPECT_NEAR(h20.state, 770.0f, 1.0f);
  EXPECT_FLOAT_EQ(h21.state, 479.0f);
  EXPECT_NEAR(h22.state, 1350.0f, 1.0f);
  EXPECT_FLOAT_EQ(h23.state, 525.0f);
  EXPECT_FLOAT_EQ(hsds.state, 2.0f);
}

TEST(VictronSmartSolarMppt15070Rev2Test, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fwe, serial;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_24bit_text_sensor(&fwe);
  v.set_serial_number_text_sensor(&serial);

  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);

  EXPECT_EQ(device_type.state, "SmartSolar MPPT 150|70 rev2");
  EXPECT_EQ(fwe.state, "3.16-official");
  EXPECT_EQ(serial.state, "HQ2247EP3FN");
}

// ── BMV-702 ───────────────────────────────────────────────────────────────────

TEST(VictronBmvTest, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, aux_v, mid_v, mid_dev, bat_i, bat_t, pow, ce, soc, ttg;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_auxiliary_battery_voltage_sensor(&aux_v);
  v.set_midpoint_voltage_of_the_battery_bank_sensor(&mid_v);
  v.set_midpoint_deviation_of_the_battery_bank_sensor(&mid_dev);
  v.set_battery_current_sensor(&bat_i);
  v.set_battery_temperature_sensor(&bat_t);
  v.set_instantaneous_power_sensor(&pow);
  v.set_consumed_amp_hours_sensor(&ce);
  v.set_state_of_charge_sensor(&soc);
  v.set_time_to_go_sensor(&ttg);

  process_frame(v, BMV_FRAME);

  EXPECT_NEAR(bat_v.state, 12.80f, 0.001f);
  EXPECT_NEAR(aux_v.state, 12.80f, 0.001f);
  EXPECT_NEAR(mid_v.state, 1.28f, 0.001f);
  EXPECT_NEAR(mid_dev.state, 12.0f, 0.01f);
  EXPECT_NEAR(bat_i.state, 15.0f, 0.001f);
  EXPECT_FLOAT_EQ(bat_t.state, 25.0f);
  EXPECT_FLOAT_EQ(pow.state, 130.0f);
  EXPECT_NEAR(ce.state, 13.5f, 0.001f);
  EXPECT_NEAR(soc.state, 87.6f, 0.01f);
  EXPECT_FLOAT_EQ(ttg.state, 45.0f);
}

TEST(VictronBmvTest, AlarmAndRelay) {
  TestableVictron v;
  text_sensor::TextSensor alarm, ar;
  binary_sensor::BinarySensor relay;
  v.set_alarm_condition_active_text_sensor(&alarm);
  v.set_alarm_reason_text_sensor(&ar);
  v.set_relay_state_binary_sensor(&relay);

  process_frame(v, BMV_FRAME);

  EXPECT_EQ(alarm.state, "OFF");
  EXPECT_FALSE(relay.state);
  EXPECT_EQ(ar.state, "Unknown");
}

TEST(VictronBmvTest, HistoryCounters) {
  TestableVictron v;
  sensor::Sensor h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h15, h16, h17, h18;
  v.set_depth_of_the_deepest_discharge_sensor(&h1);
  v.set_depth_of_the_last_discharge_sensor(&h2);
  v.set_depth_of_the_average_discharge_sensor(&h3);
  v.set_number_of_charge_cycles_sensor(&h4);
  v.set_number_of_full_discharges_sensor(&h5);
  v.set_cumulative_amp_hours_drawn_sensor(&h6);
  v.set_min_battery_voltage_sensor(&h7);
  v.set_max_battery_voltage_sensor(&h8);
  v.set_last_full_charge_sensor(&h9);
  v.set_number_of_automatic_synchronizations_sensor(&h10);
  v.set_number_of_low_main_voltage_alarms_sensor(&h11);
  v.set_number_of_high_main_voltage_alarms_sensor(&h12);
  v.set_min_auxiliary_battery_voltage_sensor(&h15);
  v.set_max_auxiliary_battery_voltage_sensor(&h16);
  v.set_amount_of_discharged_energy_sensor(&h17);
  v.set_amount_of_charged_energy_sensor(&h18);

  process_frame(v, BMV_FRAME);

  EXPECT_NEAR(h1.state, 55.0f, 0.001f);
  EXPECT_NEAR(h2.state, 15.0f, 0.001f);
  EXPECT_NEAR(h3.state, 13.0f, 0.001f);
  EXPECT_FLOAT_EQ(h4.state, 230.0f);
  EXPECT_FLOAT_EQ(h5.state, 12.0f);
  EXPECT_NEAR(h6.state, 234.0f, 0.001f);
  EXPECT_NEAR(h7.state, 11.0f, 0.001f);
  EXPECT_NEAR(h8.state, 14.8f, 0.001f);
  EXPECT_NEAR(h9.state, 120.0f, 0.1f);
  EXPECT_FLOAT_EQ(h10.state, 45.0f);
  EXPECT_FLOAT_EQ(h11.state, 5.0f);
  EXPECT_FLOAT_EQ(h12.state, 0.0f);
  EXPECT_NEAR(h15.state, 11.5f, 0.001f);
  EXPECT_NEAR(h16.state, 14.8f, 0.001f);
  EXPECT_NEAR(h17.state, 340.0f, 0.1f);
  EXPECT_NEAR(h18.state, 450.0f, 0.1f);
}

TEST(VictronBmvTest, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, model;
  v.set_device_type_text_sensor(&device_type);
  v.set_model_description_text_sensor(&model);

  process_frame(v, BMV_FRAME);

  EXPECT_EQ(device_type.state, "BMV-702");
  EXPECT_EQ(model.state, "702");
}

TEST(VictronBmvTest, DcMonitorMode) {
  TestableVictron v;
  sensor::Sensor mon_id;
  text_sensor::TextSensor mon_text;
  v.set_dc_monitor_mode_id_sensor(&mon_id);
  v.set_dc_monitor_mode_text_sensor(&mon_text);

  process_frame(v, BMV_WITH_DC_MONITOR_FRAME);

  EXPECT_FLOAT_EQ(mon_id.state, -1.0f);
  EXPECT_EQ(mon_text.state, "Generic source");
}

// ── BMV-712 Smart ─────────────────────────────────────────────────────────────

TEST(VictronBmv712Test, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, aux_v, bat_i, pow, ce, soc, ttg;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_auxiliary_battery_voltage_sensor(&aux_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_instantaneous_power_sensor(&pow);
  v.set_consumed_amp_hours_sensor(&ce);
  v.set_state_of_charge_sensor(&soc);
  v.set_time_to_go_sensor(&ttg);

  process_frame(v, BMV712_FRAME);

  EXPECT_NEAR(bat_v.state, 26.464f, 0.001f);
  EXPECT_NEAR(aux_v.state, 0.034f, 0.001f);
  EXPECT_NEAR(bat_i.state, -5.647f, 0.001f);
  EXPECT_FLOAT_EQ(pow.state, -149.0f);
  EXPECT_NEAR(ce.state, -6.671f, 0.001f);
  EXPECT_NEAR(soc.state, 93.7f, 0.01f);
  EXPECT_FLOAT_EQ(ttg.state, 436.0f);
}

TEST(VictronBmv712Test, AlarmAndRelay) {
  TestableVictron v;
  text_sensor::TextSensor alarm, ar;
  binary_sensor::BinarySensor relay;
  v.set_alarm_condition_active_text_sensor(&alarm);
  v.set_alarm_reason_text_sensor(&ar);
  v.set_relay_state_binary_sensor(&relay);

  process_frame(v, BMV712_FRAME);

  EXPECT_EQ(alarm.state, "OFF");
  EXPECT_FALSE(relay.state);
  EXPECT_EQ(ar.state, "No error");
}

TEST(VictronBmv712Test, DcMonitorMode) {
  TestableVictron v;
  sensor::Sensor mon_id;
  text_sensor::TextSensor mon_text;
  v.set_dc_monitor_mode_id_sensor(&mon_id);
  v.set_dc_monitor_mode_text_sensor(&mon_text);

  process_frame(v, BMV712_FRAME);

  EXPECT_FLOAT_EQ(mon_id.state, 0.0f);
  EXPECT_EQ(mon_text.state, "Battery monitor (BMV)");
}

TEST(VictronBmv712Test, HistoryCounters) {
  TestableVictron v;
  sensor::Sensor h1, h2, h3, h4, h5, h6, h7, h8, h9, h10, h11, h12, h15, h16, h17, h18;
  v.set_depth_of_the_deepest_discharge_sensor(&h1);
  v.set_depth_of_the_last_discharge_sensor(&h2);
  v.set_depth_of_the_average_discharge_sensor(&h3);
  v.set_number_of_charge_cycles_sensor(&h4);
  v.set_number_of_full_discharges_sensor(&h5);
  v.set_cumulative_amp_hours_drawn_sensor(&h6);
  v.set_min_battery_voltage_sensor(&h7);
  v.set_max_battery_voltage_sensor(&h8);
  v.set_last_full_charge_sensor(&h9);
  v.set_number_of_automatic_synchronizations_sensor(&h10);
  v.set_number_of_low_main_voltage_alarms_sensor(&h11);
  v.set_number_of_high_main_voltage_alarms_sensor(&h12);
  v.set_min_auxiliary_battery_voltage_sensor(&h15);
  v.set_max_auxiliary_battery_voltage_sensor(&h16);
  v.set_amount_of_discharged_energy_sensor(&h17);
  v.set_amount_of_charged_energy_sensor(&h18);

  process_frame(v, BMV712_FRAME);

  EXPECT_NEAR(h1.state, -96.47f, 0.01f);
  EXPECT_NEAR(h2.state, -6.671f, 0.001f);
  EXPECT_NEAR(h3.state, -26.485f, 0.001f);
  EXPECT_FLOAT_EQ(h4.state, 29.0f);
  EXPECT_FLOAT_EQ(h5.state, 4.0f);
  EXPECT_NEAR(h6.state, -2316.635f, 0.001f);
  EXPECT_NEAR(h7.state, 0.027f, 0.001f);
  EXPECT_NEAR(h8.state, 28.776f, 0.001f);
  EXPECT_NEAR(h9.state, 118.9f, 0.1f);
  EXPECT_FLOAT_EQ(h10.state, 31.0f);
  EXPECT_FLOAT_EQ(h11.state, 0.0f);
  EXPECT_FLOAT_EQ(h12.state, 0.0f);
  EXPECT_NEAR(h15.state, 0.022f, 0.001f);
  EXPECT_NEAR(h16.state, 0.038f, 0.001f);
  EXPECT_NEAR(h17.state, 60660.0f, 1.0f);
  EXPECT_NEAR(h18.state, 61170.0f, 1.0f);
}

TEST(VictronBmv712Test, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fw, model;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_text_sensor(&fw);
  v.set_model_description_text_sensor(&model);

  process_frame(v, BMV712_FRAME);

  EXPECT_EQ(device_type.state, "BMV-712 Smart");
  EXPECT_EQ(fw.state, "04.08");
  EXPECT_EQ(model.state, "712 Smart");
}

// ── Blue Smart IP22 Charger 24|8 ─────────────────────────────────────────────

TEST(VictronBlueSmartIp22Test, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, bat_t, err_id;
  text_sensor::TextSensor err_text, cs_text;
  sensor::Sensor cs_id;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_battery_temperature_sensor(&bat_t);
  v.set_error_code_sensor(&err_id);
  v.set_error_text_sensor(&err_text);
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);

  process_frame(v, BLUESMART_IP22_FRAME);

  EXPECT_NEAR(bat_v.state, 26.87f, 0.001f);
  EXPECT_NEAR(bat_i.state, 2.0f, 0.001f);
  EXPECT_TRUE(std::isnan(bat_t.state));
  EXPECT_FLOAT_EQ(err_id.state, 0.0f);
  EXPECT_EQ(err_text.state, "No error");
  EXPECT_FLOAT_EQ(cs_id.state, 3.0f);
  EXPECT_EQ(cs_text.state, "Bulk");
}

TEST(VictronBlueSmartIp22Test, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fwe, serial, hwrev;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_24bit_text_sensor(&fwe);
  v.set_serial_number_text_sensor(&serial);
  v.set_hardware_revision_text_sensor(&hwrev);

  process_frame(v, BLUESMART_IP22_FRAME);

  EXPECT_EQ(device_type.state, "Blue Smart IP22 Charger 24|8");
  EXPECT_EQ(fwe.state, "03.42-official");
  EXPECT_EQ(serial.state, "HQ2249TNFVM");
  EXPECT_EQ(hwrev.state, "57825");
}

// ── Blue Smart IP65 Charger 12|25 ────────────────────────────────────────────

TEST(VictronBlueSmartIp65Test, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, err_id, cs_id;
  text_sensor::TextSensor err_text, cs_text;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_error_code_sensor(&err_id);
  v.set_error_text_sensor(&err_text);
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);

  process_frame(v, BLUESMART_IP65_FRAME);

  EXPECT_NEAR(bat_v.state, 14.2f, 0.001f);
  EXPECT_NEAR(bat_i.state, 1.0f, 0.001f);
  EXPECT_FLOAT_EQ(err_id.state, 0.0f);
  EXPECT_EQ(err_text.state, "No error");
  EXPECT_FLOAT_EQ(cs_id.state, 4.0f);
  EXPECT_EQ(cs_text.state, "Absorption");
}

TEST(VictronBlueSmartIp65Test, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fwe, serial, hwrev;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_24bit_text_sensor(&fwe);
  v.set_serial_number_text_sensor(&serial);
  v.set_hardware_revision_text_sensor(&hwrev);

  process_frame(v, BLUESMART_IP65_FRAME);

  EXPECT_EQ(device_type.state, "Blue Smart IP65 Charger 12|25");
  EXPECT_EQ(fwe.state, "03.25-official");
  EXPECT_EQ(serial.state, "HQ2232GGMEH");
  EXPECT_EQ(hwrev.state, "136663");
}

// ── Phoenix Inverter 12V 250VA 230V ──────────────────────────────────────────

TEST(VictronPhoenixInverterTest, AcOutputAndMode) {
  TestableVictron v;
  sensor::Sensor ac_v, ac_i, cs_id, mode_id, warn_id;
  text_sensor::TextSensor cs_text, mode_text, warn_text, ar;
  v.set_ac_out_voltage_sensor(&ac_v);
  v.set_ac_out_current_sensor(&ac_i);
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);
  v.set_device_mode_id_sensor(&mode_id);
  v.set_device_mode_text_sensor(&mode_text);
  v.set_warning_code_sensor(&warn_id);
  v.set_warning_text_sensor(&warn_text);
  v.set_alarm_reason_text_sensor(&ar);

  process_frame(v, PHOENIX_INVERTER_FRAME);

  EXPECT_NEAR(ac_v.state, 230.0f, 0.01f);
  EXPECT_NEAR(ac_i.state, 5.0f, 0.01f);
  EXPECT_FLOAT_EQ(cs_id.state, 5.0f);
  EXPECT_EQ(cs_text.state, "Float");
  EXPECT_FLOAT_EQ(mode_id.state, 2.0f);
  EXPECT_EQ(mode_text.state, "On");
  EXPECT_FLOAT_EQ(warn_id.state, 1.0f);
  EXPECT_EQ(warn_text.state, "Low Voltage");
  EXPECT_EQ(ar.state, "Unknown");
}

TEST(VictronPhoenixInverterTest, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, serial;
  v.set_device_type_text_sensor(&device_type);
  v.set_serial_number_text_sensor(&serial);

  process_frame(v, PHOENIX_INVERTER_FRAME);

  EXPECT_EQ(device_type.state, "Phoenix Inverter 12V 250VA 230V");
  EXPECT_EQ(serial.state, "HQ141112345");
}

// ── Multi RS Solar 48V 6000VA 230V ───────────────────────────────────────────

TEST(VictronMultiRsSolarTest, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, panel_v, panel_p, ac_v, ac_i;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_panel_voltage_sensor(&panel_v);
  v.set_panel_power_sensor(&panel_p);
  v.set_ac_out_voltage_sensor(&ac_v);
  v.set_ac_out_current_sensor(&ac_i);

  process_frame(v, MULTI_RS_SOLAR_FRAME);

  EXPECT_NEAR(bat_v.state, 56.65f, 0.001f);
  EXPECT_NEAR(bat_i.state, -3.2f, 0.001f);
  EXPECT_NEAR(panel_v.state, 190.09f, 0.01f);
  EXPECT_FLOAT_EQ(panel_p.state, 0.0f);
  EXPECT_NEAR(ac_v.state, 230.01f, 0.01f);
  EXPECT_NEAR(ac_i.state, 1.0f, 0.01f);
}

TEST(VictronMultiRsSolarTest, ChargingModeAndError) {
  TestableVictron v;
  sensor::Sensor cs_id, err_id;
  text_sensor::TextSensor cs_text, err_text;
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);
  v.set_error_code_sensor(&err_id);
  v.set_error_text_sensor(&err_text);

  process_frame(v, MULTI_RS_SOLAR_FRAME);

  EXPECT_FLOAT_EQ(cs_id.state, 6.0f);
  EXPECT_EQ(cs_text.state, "Storage");
  EXPECT_FLOAT_EQ(err_id.state, 0.0f);
  EXPECT_EQ(err_text.state, "No error");
}

TEST(VictronMultiRsSolarTest, YieldHistory) {
  TestableVictron v;
  sensor::Sensor h19, h20, h21, h22, h23, hsds;
  v.set_yield_total_sensor(&h19);
  v.set_yield_today_sensor(&h20);
  v.set_max_power_today_sensor(&h21);
  v.set_yield_yesterday_sensor(&h22);
  v.set_max_power_yesterday_sensor(&h23);
  v.set_day_number_sensor(&hsds);

  process_frame(v, MULTI_RS_SOLAR_FRAME);

  EXPECT_NEAR(h19.state, 25510.0f, 1.0f);
  EXPECT_NEAR(h20.state, 2170.0f, 1.0f);
  EXPECT_FLOAT_EQ(h21.state, 1289.0f);
  EXPECT_NEAR(h22.state, 2640.0f, 1.0f);
  EXPECT_FLOAT_EQ(h23.state, 1404.0f);
  EXPECT_FLOAT_EQ(hsds.state, 5.0f);
}

TEST(VictronMultiRsSolarTest, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fwe, serial;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_24bit_text_sensor(&fwe);
  v.set_serial_number_text_sensor(&serial);

  process_frame(v, MULTI_RS_SOLAR_FRAME);

  EXPECT_EQ(device_type.state, "Multi RS Solar 48V 6000VA 230V");
  EXPECT_EQ(fwe.state, "1.09-official");
  EXPECT_EQ(serial.state, "HQ2206WVU9D");
}

// ── Orion XS 12V/12V-50A ─────────────────────────────────────────────────────

TEST(VictronOrionXsTest, VoltagesAndCurrents) {
  TestableVictron v;
  sensor::Sensor bat_v, bat_i, pow, dc_in_v, dc_in_i, dc_in_p;
  v.set_battery_voltage_sensor(&bat_v);
  v.set_battery_current_sensor(&bat_i);
  v.set_instantaneous_power_sensor(&pow);
  v.set_dc_input_voltage_sensor(&dc_in_v);
  v.set_dc_input_current_sensor(&dc_in_i);
  v.set_dc_input_power_sensor(&dc_in_p);

  process_frame(v, ORION_XS_FRAME);

  EXPECT_NEAR(bat_v.state, 13.22f, 0.001f);
  EXPECT_NEAR(bat_i.state, 0.0f, 0.001f);
  EXPECT_FLOAT_EQ(pow.state, 0.0f);
  EXPECT_NEAR(dc_in_v.state, 12.62f, 0.01f);
  EXPECT_NEAR(dc_in_i.state, 0.0f, 0.001f);
  EXPECT_FLOAT_EQ(dc_in_p.state, 0.0f);
}

TEST(VictronOrionXsTest, ChargingModeAndError) {
  TestableVictron v;
  sensor::Sensor cs_id, err_id;
  text_sensor::TextSensor cs_text, err_text;
  v.set_charging_mode_id_sensor(&cs_id);
  v.set_charging_mode_text_sensor(&cs_text);
  v.set_error_code_sensor(&err_id);
  v.set_error_text_sensor(&err_text);

  process_frame(v, ORION_XS_FRAME);

  EXPECT_FLOAT_EQ(cs_id.state, 0.0f);
  EXPECT_EQ(cs_text.state, "Off");
  EXPECT_FLOAT_EQ(err_id.state, 0.0f);
  EXPECT_EQ(err_text.state, "No error");
}

TEST(VictronOrionXsTest, OffReasonMultipleBits) {
  TestableVictron v;
  sensor::Sensor bitmask;
  text_sensor::TextSensor reason;
  v.set_off_reason_bitmask_sensor(&bitmask);
  v.set_off_reason_text_sensor(&reason);

  process_frame(v, ORION_XS_FRAME);

  EXPECT_FLOAT_EQ(bitmask.state, 9.0f);
  EXPECT_EQ(reason.state, "No input power;Remote input");
}

TEST(VictronOrionXsTest, DeviceInfo) {
  TestableVictron v;
  text_sensor::TextSensor device_type, fwe, serial;
  v.set_device_type_text_sensor(&device_type);
  v.set_firmware_version_24bit_text_sensor(&fwe);
  v.set_serial_number_text_sensor(&serial);

  process_frame(v, ORION_XS_FRAME);

  EXPECT_EQ(device_type.state, "Orion XS 12V/12V-50A");
  EXPECT_EQ(fwe.state, "1.12-official");
  EXPECT_EQ(serial.state, "HQ2433EWGZZ");
}

// ── Edge cases ────────────────────────────────────────────────────────────────

TEST(VictronEdgeCaseTest, TemperatureNotAvailable) {
  TestableVictron v;
  sensor::Sensor bat_t;
  v.set_battery_temperature_sensor(&bat_t);

  v.label_ = "T";
  v.value_ = "---";
  v.handle_value_();

  EXPECT_TRUE(std::isnan(bat_t.state));
}

TEST(VictronEdgeCaseTest, FirmwareVersionOncePublished) {
  TestableVictron v;
  text_sensor::TextSensor fw;
  v.set_firmware_version_text_sensor(&fw);

  v.label_ = "FW";
  v.value_ = "156";
  v.handle_value_();
  EXPECT_EQ(fw.state, "1.56");

  v.value_ = "157";
  v.handle_value_();
  EXPECT_EQ(fw.state, "1.56");
}

TEST(VictronEdgeCaseTest, AcOutCurrentNegativeClampedToZero) {
  TestableVictron v;
  sensor::Sensor ac_i;
  v.set_ac_out_current_sensor(&ac_i);

  v.label_ = "AC_OUT_I";
  v.value_ = "-50";
  v.handle_value_();

  EXPECT_FLOAT_EQ(ac_i.state, 0.0f);
}

TEST(VictronEdgeCaseTest, NullSensorsDoNotCrash) {
  TestableVictron v;
  process_frame(v, SMARTSOLAR_MPPT_FRAME);
  process_frame(v, SMARTSOLAR_MPPT_150_70_REV2_FRAME);
  process_frame(v, BMV_FRAME);
  process_frame(v, BMV712_FRAME);
  process_frame(v, BLUESMART_IP22_FRAME);
  process_frame(v, BLUESMART_IP65_FRAME);
  process_frame(v, PHOENIX_INVERTER_FRAME);
  process_frame(v, MULTI_RS_SOLAR_FRAME);
  process_frame(v, ORION_XS_FRAME);
}

}  // namespace esphome::victron::testing
