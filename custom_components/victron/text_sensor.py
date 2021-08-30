import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_ID, UNIT_VOLT, ICON_FLASH, UNIT_AMPERE, UNIT_WATT, \
    ICON_POWER, ICON_CURRENT_AC, CONF_TEMPERATURE, ICON_THERMOMETER, UNIT_CELSIUS, \
    UNIT_PERCENT, ICON_PERCENT, UNIT_EMPTY, ICON_EMPTY

DEPENDENCIES = ['uart']

esmart3_ns = cg.esphome_ns.namespace('esmart3')
ESmart3Component = esmart3_ns.class_('ESmart3Component', uart.UARTDevice, cg.PollingComponent)

CONF_CHARGE_MODE = "charge_mode"
CONF_INPUT_VOLTAGE = "input_voltage"
CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_CHARGING_CURRENT = "charging_current"
CONF_LOAD_VOLTAGE = "load_voltage"
CONF_LOAD_CURRENT = "load_current"
CONF_CHARGING_POWER = "charging_power"
CONF_LOAD_POWER = "load_power"
CONF_BATTERY_TEMP = "battery_temp"
CONF_INTERNAL_TEMP = "internal_temp"
CONF_BATTERY_LEVEL = "battery_level"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(ESmart3Component),
    cv.Optional(CONF_CHARGE_MODE): sensor.sensor_schema(UNIT_EMPTY, ICON_EMPTY, 0),
    cv.Optional(CONF_INPUT_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1),
    cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1),
    cv.Optional(CONF_CHARGING_CURRENT): sensor.sensor_schema(UNIT_AMPERE, ICON_CURRENT_AC, 1),
    cv.Optional(CONF_LOAD_VOLTAGE): sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1),
    cv.Optional(CONF_LOAD_CURRENT): sensor.sensor_schema(UNIT_AMPERE, ICON_CURRENT_AC, 1),
    cv.Optional(CONF_CHARGING_POWER): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 0),
    cv.Optional(CONF_LOAD_POWER): sensor.sensor_schema(UNIT_WATT, ICON_POWER, 0),
    cv.Optional(CONF_BATTERY_TEMP): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 0),
    cv.Optional(CONF_INTERNAL_TEMP): sensor.sensor_schema(UNIT_CELSIUS, ICON_THERMOMETER, 0),
    cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(UNIT_PERCENT, ICON_PERCENT, 0),
}).extend(cv.polling_component_schema('60s'))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_CHARGE_MODE in config:
        sens = yield sensor.new_sensor(config[CONF_CHARGE_MODE])
        cg.add(var.set_charge_mode_sensor(sens))

    if CONF_INPUT_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_INPUT_VOLTAGE])
        cg.add(var.set_input_voltage_sensor(sens))

    if CONF_BATTERY_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))

    if CONF_CHARGING_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_CHARGING_CURRENT])
        cg.add(var.set_charging_current_sensor(sens))

    if CONF_LOAD_VOLTAGE in config:
        sens = yield sensor.new_sensor(config[CONF_LOAD_VOLTAGE])
        cg.add(var.set_load_voltage_sensor(sens))

    if CONF_LOAD_CURRENT in config:
        sens = yield sensor.new_sensor(config[CONF_LOAD_CURRENT])
        cg.add(var.set_load_current_sensor(sens))

    if CONF_CHARGING_POWER in config:
        sens = yield sensor.new_sensor(config[CONF_CHARGING_POWER])
        cg.add(var.set_charging_power_sensor(sens))

    if CONF_LOAD_POWER in config:
        sens = yield sensor.new_sensor(config[CONF_LOAD_POWER])
        cg.add(var.set_load_power_sensor(sens))

    if CONF_BATTERY_TEMP in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_TEMP])
        cg.add(var.set_battery_temp_sensor(sens))

    if CONF_INTERNAL_TEMP in config:
        sens = yield sensor.new_sensor(config[CONF_INTERNAL_TEMP])
        cg.add(var.set_internal_temp_sensor(sens))

    if CONF_BATTERY_LEVEL in config:
        sens = yield sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))
