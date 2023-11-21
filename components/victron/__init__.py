import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_THROTTLE

AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor"]

DEPENDENCIES = ["uart"]

CODEOWNERS = ["@KinDR007"]

MULTI_CONF = True

CONF_ASYNC_UART = "async_uart"

victron_ns = cg.esphome_ns.namespace("victron")
VictronComponent = victron_ns.class_("VictronComponent", uart.UARTDevice, cg.Component)

CONF_VICTRON_ID = "victron_id"

CONFIG_SCHEMA = uart.UART_DEVICE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(VictronComponent),
        cv.Optional(CONF_THROTTLE, default="1s"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_ASYNC_UART, default=False): cv.boolean,
    }
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    cg.add(var.set_throttle(config[CONF_THROTTLE]))
    cg.add(var.set_async_uart(config[CONF_ASYNC_UART]))
