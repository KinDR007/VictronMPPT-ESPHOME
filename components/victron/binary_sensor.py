import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv

from . import CONF_VICTRON_ID, VICTRON_COMPONENT_SCHEMA

DEPENDENCIES = ["victron"]

CODEOWNERS = ["@KinDR007"]

CONF_LOAD_STATE = "load_state"
CONF_RELAY_STATE = "relay_state"

BINARY_SENSORS = [
    CONF_LOAD_STATE,
    CONF_RELAY_STATE,
]

CONFIG_SCHEMA = VICTRON_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_LOAD_STATE): binary_sensor.binary_sensor_schema(),
        cv.Optional(CONF_RELAY_STATE): binary_sensor.binary_sensor_schema(),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_VICTRON_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = yield binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
