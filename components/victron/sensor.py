import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_ENERGY,
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

from . import CONF_VICTRON_ID, VictronComponent

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

UNIT_AMPERE_HOUR = "Ah"

SENSORS = [
    CONF_BATTERY_VOLTAGE,
    CONF_MAX_POWER_YESTERDAY,
    CONF_MAX_POWER_TODAY,
    CONF_YIELD_TOTAL,
    CONF_YIELD_YESTERDAY,
    CONF_YIELD_TODAY,
    CONF_PANEL_VOLTAGE,
    CONF_PANEL_POWER,
    CONF_BATTERY_VOLTAGE_2,
    CONF_BATTERY_VOLTAGE_3,
    CONF_AUXILIARY_BATTERY_VOLTAGE,
    CONF_MIDPOINT_VOLTAGE_OF_THE_BATTERY_BANK,
    CONF_MIDPOINT_DEVIATION_OF_THE_BATTERY_BANK,
    CONF_BATTERY_CURRENT,
    CONF_BATTERY_CURRENT_2,
    CONF_BATTERY_CURRENT_3,
    CONF_AC_OUT_VOLTAGE,
    CONF_AC_OUT_CURRENT,
    CONF_AC_OUT_APPARENT_POWER,
    CONF_DAY_NUMBER,
    CONF_CHARGING_MODE_ID,
    CONF_ERROR_CODE,
    CONF_WARNING_CODE,
    CONF_TRACKING_MODE_ID,
    CONF_DEVICE_MODE_ID,
    CONF_LOAD_CURRENT,
    #
    CONF_BATTERY_TEMPERATURE,
    CONF_INSTANTANEOUS_POWER,
    CONF_CONSUMED_AMP_HOURS,
    CONF_STATE_OF_CHARGE,
    CONF_TIME_TO_GO,
    CONF_DEPTH_OF_THE_DEEPEST_DISCHARGE,
    CONF_DEPTH_OF_THE_LAST_DISCHARGE,
    CONF_DEPTH_OF_THE_AVERAGE_DISCHARGE,
    CONF_NUMBER_OF_CHARGE_CYCLES,
    CONF_NUMBER_OF_FULL_DISCHARGES,
    CONF_CUMULATIVE_AMP_HOURS_DRAWN,
    CONF_MIN_BATTERY_VOLTAGE,
    CONF_MAX_BATTERY_VOLTAGE,
    CONF_LAST_FULL_CHARGE,
    CONF_NUMBER_OF_AUTOMATIC_SYNCHRONIZATIONS,
    CONF_NUMBER_OF_LOW_MAIN_VOLTAGE_ALARMS,
    CONF_NUMBER_OF_HIGH_MAIN_VOLTAGE_ALARMS,
    CONF_NUMBER_OF_LOW_AUXILIARY_VOLTAGE_ALARMS,
    CONF_NUMBER_OF_HIGH_AUXILIARY_VOLTAGE_ALARMS,
    CONF_MIN_AUXILIARY_BATTERY_VOLTAGE,
    CONF_MAX_AUXILIARY_BATTERY_VOLTAGE,
    CONF_AMOUNT_OF_DISCHARGED_ENERGY,
    CONF_AMOUNT_OF_CHARGED_ENERGY,
    CONF_DC_MONITOR_MODE_ID,
    CONF_OFF_REASON_BITMASK,
]


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_VICTRON_ID): cv.use_id(VictronComponent),
        cv.Optional(CONF_MAX_POWER_YESTERDAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_MAX_POWER_TODAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_YIELD_TOTAL): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_YIELD_YESTERDAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_YIELD_TODAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_PANEL_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_PANEL_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE_2): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE_3): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_AUXILIARY_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_MIDPOINT_VOLTAGE_OF_THE_BATTERY_BANK): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_MIDPOINT_DEVIATION_OF_THE_BATTERY_BANK): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_BATTERY_CURRENT_2): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_BATTERY_CURRENT_3): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_AC_OUT_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_AC_OUT_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_AC_OUT_APPARENT_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_DAY_NUMBER): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_CHARGING_MODE_ID): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_ERROR_CODE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_WARNING_CODE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_TRACKING_MODE_ID): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_DEVICE_MODE_ID): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_LOAD_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_BATTERY_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_INSTANTANEOUS_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_CONSUMED_AMP_HOURS): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            icon=ICON_GAUGE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_STATE_OF_CHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_TIME_TO_GO): sensor.sensor_schema(
            unit_of_measurement=UNIT_MINUTE,
            icon=ICON_TIMELAPSE,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_DEPTH_OF_THE_DEEPEST_DISCHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            icon=ICON_GAUGE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_DEPTH_OF_THE_LAST_DISCHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            icon=ICON_GAUGE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_DEPTH_OF_THE_AVERAGE_DISCHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            icon=ICON_GAUGE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_CHARGE_CYCLES): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_FULL_DISCHARGES): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_CUMULATIVE_AMP_HOURS_DRAWN): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE_HOUR,
            icon=ICON_GAUGE,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_MIN_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_MAX_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_LAST_FULL_CHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_MINUTE,
            icon=ICON_TIMELAPSE,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_AUTOMATIC_SYNCHRONIZATIONS): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_LOW_MAIN_VOLTAGE_ALARMS): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_HIGH_MAIN_VOLTAGE_ALARMS): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_LOW_AUXILIARY_VOLTAGE_ALARMS): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_NUMBER_OF_HIGH_AUXILIARY_VOLTAGE_ALARMS): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_MIN_AUXILIARY_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_MAX_AUXILIARY_BATTERY_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_AMOUNT_OF_DISCHARGED_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_AMOUNT_OF_CHARGED_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL_INCREASING,
        ),
        cv.Optional(CONF_DC_MONITOR_MODE_ID): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
        cv.Optional(CONF_OFF_REASON_BITMASK): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
        ),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_VICTRON_ID])
    for key in SENSORS:
        if key in config:
            conf = config[key]
            sens = yield sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
