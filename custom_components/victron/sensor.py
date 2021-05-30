import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, uart
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, \
    UNIT_WATT_HOURS, ICON_POWER, ICON_CURRENT_AC, ICON_COUNTER, UNIT_EMPTY, ICON_EMPTY, \
    DEVICE_CLASS_POWER, DEVICE_CLASS_CURRENT, DEVICE_CLASS_VOLTAGE, DEVICE_CLASS_EMPTY
from . import victron_ns, VictronComponent, CONF_VICTRON_ID

# sensors
CONF_MAX_POWER_YESTERDAY = "max_power_yesterday"
CONF_MAX_POWER_TODAY = "max_power_today"
CONF_YIELD_TOTAL = "yield_total"
CONF_YIELD_YESTERDAY = "yield_yesterday"
CONF_YIELD_TODAY = "yield_today"
CONF_PANEL_VOLTAGE = "panel_voltage"
CONF_PANEL_POWER = "panel_power"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_BATTERY_CURRENT = "battery_current"
CONF_DAY_NUMBER = "day_number"
CONF_CHARGER_STATUS = "charger_status"
CONF_ERROR_CODE = "error_code"
CONF_TRACKER_OPERATION = "tracker_operation"

# text sensors
CONF_CHARGER_TEXT = "charger_text"
CONF_ERROR_TEXT = "error_text"
CONF_TRACKER_TEXT = "tracker_text"
CONF_FW_VERSION = "fw_version"
CONF_PID = "pid"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_VICTRON_ID): cv.use_id(VictronComponent),
    cv.Optional(CONF_MAX_POWER_YESTERDAY): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_MAX_POWER_TODAY): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_YIELD_TOTAL): sensor.sensor_schema(UNIT_WATT_HOURS, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_YIELD_YESTERDAY): sensor.sensor_schema(UNIT_WATT_HOURS, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_YIELD_TODAY): sensor.sensor_schema(UNIT_WATT_HOURS, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_PANEL_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 3, DEVICE_CLASS_VOLTAGE),
    cv.Optional(CONF_PANEL_POWER): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 0, DEVICE_CLASS_POWER),
    cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 3, DEVICE_CLASS_VOLTAGE),
    cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(UNIT_AMPERE, ICON_CURRENT_AC, 3, DEVICE_CLASS_CURRENT),
    cv.Optional(CONF_DAY_NUMBER): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_CHARGER_STATUS): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_ERROR_CODE): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_TRACKER_OPERATION): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0, DEVICE_CLASS_EMPTY),
    cv.Optional(CONF_CHARGER_TEXT): text_sensor.TEXT_SENSOR_SCHEMA.extend({cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}),
    cv.Optional(CONF_ERROR_TEXT): text_sensor.TEXT_SENSOR_SCHEMA.extend({cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}),
    cv.Optional(CONF_TRACKER_TEXT): text_sensor.TEXT_SENSOR_SCHEMA.extend({cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}),
    cv.Optional(CONF_FW_VERSION): text_sensor.TEXT_SENSOR_SCHEMA.extend({cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}),
    cv.Optional(CONF_PID): text_sensor.TEXT_SENSOR_SCHEMA.extend({cv.GenerateID(): cv.declare_id(text_sensor.TextSensor)}),
})

def to_code(config):
    victron = yield cg.get_variable(config[CONF_VICTRON_ID])

    if CONF_MAX_POWER_YESTERDAY in config:
        sens = yield sensor.new_sensor(config[CONF_MAX_POWER_YESTERDAY])
        cg.add(victron.set_max_power_yesterday_sensor(sens))

    if CONF_MAX_POWER_TODAY in config:
        sens = yield sensor.new_sensor(config[CONF_MAX_POWER_TODAY])
        cg.add(victron.set_max_power_today_sensor(sens))

    if CONF_YIELD_TOTAL in config:
        sens = yield sensor.new_sensor(config[CONF_YIELD_TOTAL])
        cg.add(victron.set_yield_total_sensor(sens))

    if CONF_YIELD_YESTERDAY in config:
        sens = yield sensor.new_sensor(config[CONF_YIELD_YESTERDAY])
        cg.add(victron.set_yield_yesterday_sensor(sens))

    if CONF_YIELD_TODAY in config:
        sens = yield sensor.new_sensor(config[CONF_YIELD_TODAY])
        cg.add(victron.set_yield_today_sensor(sens))

    if CONF_PANEL_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_PANEL_VOLTAGE])
        cg.add(victron.set_panel_voltage_sensor(sens))

    if CONF_PANEL_POWER in config:
        sens = yield sensor.new_sensor(config[CONF_PANEL_POWER])
        cg.add(victron.set_panel_power_sensor(sens))

    if CONF_BATTERY_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(victron.set_battery_voltage_sensor(sens))

    if CONF_BATTERY_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_CURRENT])
        cg.add(victron.set_battery_current_sensor(sens))

    if CONF_DAY_NUMBER in config:
        sens = yield sensor.new_sensor(config[CONF_DAY_NUMBER])
        cg.add(victron.set_day_number_sensor(sens))

    if CONF_CHARGER_STATUS in config:
        sens = yield sensor.new_sensor(config[CONF_CHARGER_STATUS])
        cg.add(victron.set_charger_status_sensor(sens))

    if CONF_ERROR_CODE in config:
        sens = yield sensor.new_sensor(config[CONF_ERROR_CODE])
        cg.add(victron.set_error_code_sensor(sens))

    if CONF_TRACKER_OPERATION in config:
        sens = yield sensor.new_sensor(config[CONF_TRACKER_OPERATION])
        cg.add(victron.set_tracker_operation_sensor(sens))

    if CONF_CHARGER_TEXT in config:
        conf = config[CONF_CHARGER_TEXT]
        sens = cg.new_Pvariable(conf[CONF_ID])
        yield text_sensor.register_text_sensor(sens, conf)
        cg.add(victron.set_charger_text_sensor(sens))

    if CONF_ERROR_TEXT in config:
        conf = config[CONF_ERROR_TEXT]
        sens = cg.new_Pvariable(conf[CONF_ID])
        yield text_sensor.register_text_sensor(sens, conf)
        cg.add(victron.set_error_text_sensor(sens))

    if CONF_TRACKER_TEXT in config:
        conf = config[CONF_TRACKER_TEXT]
        sens = cg.new_Pvariable(conf[CONF_ID])
        yield text_sensor.register_text_sensor(sens, conf)
        cg.add(victron.set_tracker_text_sensor(sens))

    if CONF_FW_VERSION in config:
        conf = config[CONF_FW_VERSION]
        sens = cg.new_Pvariable(conf[CONF_ID])
        yield text_sensor.register_text_sensor(sens, conf)
        cg.add(victron.set_fw_version_sensor(sens))

    if CONF_PID in config:
        conf = config[CONF_PID]
        sens = cg.new_Pvariable(conf[CONF_ID])
        yield text_sensor.register_text_sensor(sens, conf)
        cg.add(victron.set_pid_sensor(sens))

