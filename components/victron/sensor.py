import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_EMPTY,
    ICON_FLASH,
    ICON_GAUGE,
    ICON_PERCENT,
    ICON_POWER,
    ICON_TIMELAPSE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_MINUTE,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_WATT,
    UNIT_WATT_HOURS,
)

from . import CONF_VICTRON_ID, VICTRON_COMPONENT_SCHEMA

DEPENDENCIES = ["victron"]

CODEOWNERS = ["@KinDR007"]

CONF_MAX_POWER_YESTERDAY = "max_power_yesterday"
CONF_MAX_POWER_TODAY = "max_power_today"
CONF_YIELD_TOTAL = "yield_total"
CONF_YIELD_YESTERDAY = "yield_yesterday"
CONF_YIELD_TODAY = "yield_today"
CONF_PANEL_VOLTAGE = "panel_voltage"
CONF_PANEL_POWER = "panel_power"
# CONF_BATTERY_VOLTAGE imported from const
CONF_BATTERY_VOLTAGE_2 = "battery_voltage_2"
CONF_BATTERY_VOLTAGE_3 = "battery_voltage_3"
CONF_AUXILIARY_BATTERY_VOLTAGE = "auxiliary_battery_voltage"
CONF_MIDPOINT_VOLTAGE_OF_THE_BATTERY_BANK = "midpoint_voltage_of_the_battery_bank"
CONF_MIDPOINT_DEVIATION_OF_THE_BATTERY_BANK = "midpoint_deviation_of_the_battery_bank"
CONF_BATTERY_CURRENT = "battery_current"
CONF_BATTERY_CURRENT_2 = "battery_current_2"
CONF_BATTERY_CURRENT_3 = "battery_current_3"
CONF_DAY_NUMBER = "day_number"
CONF_CHARGING_MODE_ID = "charging_mode_id"
CONF_ERROR_CODE = "error_code"
CONF_WARNING_CODE = "warning_code"
CONF_TRACKING_MODE_ID = "tracking_mode_id"
CONF_DEVICE_MODE_ID = "device_mode_id"
CONF_LOAD_CURRENT = "load_current"
CONF_AC_OUT_VOLTAGE = "ac_out_voltage"
CONF_AC_OUT_CURRENT = "ac_out_current"
CONF_AC_OUT_APPARENT_POWER = "ac_out_apparent_power"

CONF_BATTERY_TEMPERATURE = "battery_temperature"
CONF_INSTANTANEOUS_POWER = "instantaneous_power"
CONF_CONSUMED_AMP_HOURS = "consumed_amp_hours"
CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_TIME_TO_GO = "time_to_go"
CONF_DEPTH_OF_THE_DEEPEST_DISCHARGE = "depth_of_the_deepest_discharge"
CONF_DEPTH_OF_THE_LAST_DISCHARGE = "depth_of_the_last_discharge"
CONF_DEPTH_OF_THE_AVERAGE_DISCHARGE = "depth_of_the_average_discharge"
CONF_NUMBER_OF_CHARGE_CYCLES = "number_of_charge_cycles"
CONF_NUMBER_OF_FULL_DISCHARGES = "number_of_full_discharges"
CONF_CUMULATIVE_AMP_HOURS_DRAWN = "cumulative_amp_hours_drawn"
CONF_MIN_BATTERY_VOLTAGE = "min_battery_voltage"
CONF_MAX_BATTERY_VOLTAGE = "max_battery_voltage"
CONF_LAST_FULL_CHARGE = "last_full_charge"
CONF_NUMBER_OF_AUTOMATIC_SYNCHRONIZATIONS = "number_of_automatic_synchronizations"
CONF_NUMBER_OF_LOW_MAIN_VOLTAGE_ALARMS = "number_of_low_main_voltage_alarms"
CONF_NUMBER_OF_HIGH_MAIN_VOLTAGE_ALARMS = "number_of_high_main_voltage_alarms"
CONF_NUMBER_OF_LOW_AUXILIARY_VOLTAGE_ALARMS = "number_of_low_auxiliary_voltage_alarms"
CONF_NUMBER_OF_HIGH_AUXILIARY_VOLTAGE_ALARMS = "number_of_high_auxiliary_voltage_alarms"
CONF_MIN_AUXILIARY_BATTERY_VOLTAGE = "min_auxiliary_battery_voltage"
CONF_MAX_AUXILIARY_BATTERY_VOLTAGE = "max_auxiliary_battery_voltage"
CONF_AMOUNT_OF_DISCHARGED_ENERGY = "amount_of_discharged_energy"
CONF_AMOUNT_OF_CHARGED_ENERGY = "amount_of_charged_energy"
CONF_DC_MONITOR_MODE_ID = "dc_monitor_mode_id"
CONF_OFF_REASON_BITMASK = "off_reason_bitmask"
CONF_DC_INPUT_VOLTAGE = "dc_input_voltage"
CONF_DC_INPUT_CURRENT = "dc_input_current"
CONF_DC_INPUT_POWER = "dc_input_power"

UNIT_AMPERE_HOUR = "Ah"

SENSOR_DEFS = {
    CONF_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_MAX_POWER_YESTERDAY: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
    CONF_MAX_POWER_TODAY: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
    CONF_YIELD_TOTAL: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_YIELD_YESTERDAY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_YIELD_TODAY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_PANEL_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_PANEL_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
    CONF_BATTERY_VOLTAGE_2: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_BATTERY_VOLTAGE_3: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_AUXILIARY_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_MIDPOINT_VOLTAGE_OF_THE_BATTERY_BANK: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_MIDPOINT_DEVIATION_OF_THE_BATTERY_BANK: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": ICON_PERCENT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_BATTERY_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_BATTERY_CURRENT_2: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_BATTERY_CURRENT_3: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_AC_OUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_AC_OUT_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_AC_OUT_APPARENT_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
    CONF_DAY_NUMBER: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_CHARGING_MODE_ID: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_ERROR_CODE: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_WARNING_CODE: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_TRACKING_MODE_ID: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_DEVICE_MODE_ID: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_LOAD_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_BATTERY_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_INSTANTANEOUS_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
    CONF_CONSUMED_AMP_HOURS: {
        "unit_of_measurement": UNIT_AMPERE_HOUR,
        "icon": ICON_GAUGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_STATE_OF_CHARGE: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": ICON_PERCENT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_TIME_TO_GO: {
        "unit_of_measurement": UNIT_MINUTE,
        "icon": ICON_TIMELAPSE,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DEPTH_OF_THE_DEEPEST_DISCHARGE: {
        "unit_of_measurement": UNIT_AMPERE_HOUR,
        "icon": ICON_GAUGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_DEPTH_OF_THE_LAST_DISCHARGE: {
        "unit_of_measurement": UNIT_AMPERE_HOUR,
        "icon": ICON_GAUGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_DEPTH_OF_THE_AVERAGE_DISCHARGE: {
        "unit_of_measurement": UNIT_AMPERE_HOUR,
        "icon": ICON_GAUGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_CHARGE_CYCLES: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_FULL_DISCHARGES: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_CUMULATIVE_AMP_HOURS_DRAWN: {
        "unit_of_measurement": UNIT_AMPERE_HOUR,
        "icon": ICON_GAUGE,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_MIN_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_MAX_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_LAST_FULL_CHARGE: {
        "unit_of_measurement": UNIT_MINUTE,
        "icon": ICON_TIMELAPSE,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_AUTOMATIC_SYNCHRONIZATIONS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_LOW_MAIN_VOLTAGE_ALARMS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_HIGH_MAIN_VOLTAGE_ALARMS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_LOW_AUXILIARY_VOLTAGE_ALARMS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_NUMBER_OF_HIGH_AUXILIARY_VOLTAGE_ALARMS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_MIN_AUXILIARY_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_MAX_AUXILIARY_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_AMOUNT_OF_DISCHARGED_ENERGY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_AMOUNT_OF_CHARGED_ENERGY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_DC_MONITOR_MODE_ID: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_OFF_REASON_BITMASK: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": ICON_EMPTY,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
    },
    CONF_DC_INPUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "icon": ICON_FLASH,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
    },
    CONF_DC_INPUT_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": ICON_CURRENT_AC,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_CURRENT,
    },
    CONF_DC_INPUT_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": ICON_POWER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
    },
}

CONFIG_SCHEMA = VICTRON_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): sensor.sensor_schema(**kwargs)
        for key, kwargs in SENSOR_DEFS.items()
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_VICTRON_ID])
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = yield sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
