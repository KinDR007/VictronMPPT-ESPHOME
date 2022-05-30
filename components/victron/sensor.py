import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_EMPTY,
    ICON_FLASH,
    ICON_PERCENT,
    ICON_POWER,
    ICON_TIMELAPSE,
    STATE_CLASS_MEASUREMENT,
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
CONF_BATTERY_CURRENT = "battery_current"
CONF_DAY_NUMBER = "day_number"
CONF_CHARGING_MODE_ID = "charging_mode_id"
CONF_ERROR_CODE = "error_code"
CONF_WARNING_CODE = "warning_code"
CONF_TRACKING_MODE_ID = "tracking_mode_id"
CONF_DEVICE_MODE_ID = "device_mode_id"
CONF_LOAD_CURRENT = "load_current"
CONF_AC_OUT_VOLTAGE = "ac_out_voltage"
CONF_AC_OUT_CURRENT = "ac_out_current"

CONF_BATTERY_TEMPERATURE = "battery_temperature"
CONF_INSTANTANEOUS_POWER = "instantaneous_power"
CONF_CONSUMED_AMP_HOURS = "consumed_amp_hours"
CONF_STATE_OF_CHARGE = "state_of_charge"
CONF_TIME_TO_GO = "time_to_go"
CONF_DEPTH_OF_THE_DEEPEST_DISCHARGE = "depth_of_the_deepest_discharge"
CONF_DEPTH_OF_THE_LAST_DISCHARGE = "depth_of_the_last_discharge"
CONF_NUMBER_OF_CHARGE_CYCLES = "number_of_charge_cycles"
CONF_NUMBER_OF_FULL_DISCHARGES = "number_of_full_discharges"
CONF_MIN_BATTERY_VOLTAGE = "min_battery_voltage"
CONF_MAX_BATTERY_VOLTAGE = "max_battery_voltage"
CONF_LAST_FULL_CHARGE = "last_full_charge"
CONF_AMOUNT_OF_DISCHARGED_ENERGY = "amount_of_discharged_energy"
CONF_AMOUNT_OF_CHARGED_ENERGY = "amount_of_charged_energy"

SENSORS = [
    CONF_BATTERY_VOLTAGE,
    CONF_AC_OUT_VOLTAGE,
    CONF_MAX_POWER_YESTERDAY,
    CONF_MAX_POWER_TODAY,
    CONF_YIELD_TOTAL,
    CONF_YIELD_YESTERDAY,
    CONF_YIELD_TODAY,
    CONF_PANEL_VOLTAGE,
    CONF_PANEL_POWER,
    CONF_BATTERY_CURRENT,
    CONF_AC_OUT_CURRENT,
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
    CONF_NUMBER_OF_CHARGE_CYCLES,
    CONF_NUMBER_OF_FULL_DISCHARGES,
    CONF_MIN_BATTERY_VOLTAGE,
    CONF_MAX_BATTERY_VOLTAGE,
    CONF_LAST_FULL_CHARGE,
    CONF_AMOUNT_OF_DISCHARGED_ENERGY,
    CONF_AMOUNT_OF_CHARGED_ENERGY,
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
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_YIELD_YESTERDAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_YIELD_TODAY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
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
        cv.Optional(CONF_AC_OUT_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
        ),
        cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_AC_OUT_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
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
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_POWER,
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
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
        ),
        cv.Optional(CONF_DEPTH_OF_THE_LAST_DISCHARGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_AMPERE,
            icon=ICON_CURRENT_AC,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
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
        cv.Optional(CONF_AMOUNT_OF_DISCHARGED_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_AMOUNT_OF_CHARGED_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT_HOURS,
            icon=ICON_POWER,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
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
