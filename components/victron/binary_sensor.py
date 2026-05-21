import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv

from . import CONF_VICTRON_ID, VICTRON_COMPONENT_SCHEMA

DEPENDENCIES = ["victron"]

CODEOWNERS = ["@KinDR007"]

CONF_LOAD_STATE = "load_state"
CONF_RELAY_STATE = "relay_state"

BINARY_SENSOR_DEFS = {
    CONF_LOAD_STATE: {},
    CONF_RELAY_STATE: {},
}

CONFIG_SCHEMA = VICTRON_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): binary_sensor.binary_sensor_schema(**kwargs)
        for key, kwargs in BINARY_SENSOR_DEFS.items()
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_VICTRON_ID])
    for key in BINARY_SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = yield binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
