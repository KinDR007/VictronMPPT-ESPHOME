import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_EMPTY

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
        cv.Optional(CONF_LOAD_STATE): binary_sensor.binary_sensor_schema(
            binary_sensor.BinarySensor,
            icon=ICON_EMPTY,
        ),
        cv.Optional(CONF_RELAY_STATE): binary_sensor.binary_sensor_schema(
            binary_sensor.BinarySensor,
            icon=ICON_EMPTY,
        ),
    }
)


def to_code(config):
    hub = yield cg.get_variable(config[CONF_VICTRON_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            yield binary_sensor.register_binary_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))
