"""Component schema tests."""


class TestSensorDefs:
    def test_sensor_defs_completeness(self):
        from components.victron.sensor import SENSOR_DEFS

        assert len(SENSOR_DEFS) == 54

    def test_first_and_last_sensor(self):
        from components.victron.sensor import (
            CONF_BATTERY_VOLTAGE,
            CONF_DC_INPUT_POWER,
            SENSOR_DEFS,
        )

        keys = list(SENSOR_DEFS)
        assert keys[0] == CONF_BATTERY_VOLTAGE
        assert keys[-1] == CONF_DC_INPUT_POWER

    def test_known_sensors(self):
        from components.victron.sensor import (
            CONF_BATTERY_CURRENT,
            CONF_BATTERY_VOLTAGE,
            CONF_DC_INPUT_POWER,
            CONF_DC_INPUT_VOLTAGE,
            CONF_PANEL_VOLTAGE,
            CONF_STATE_OF_CHARGE,
            CONF_YIELD_TOTAL,
            SENSOR_DEFS,
        )

        assert CONF_BATTERY_VOLTAGE in SENSOR_DEFS
        assert CONF_PANEL_VOLTAGE in SENSOR_DEFS
        assert CONF_BATTERY_CURRENT in SENSOR_DEFS
        assert CONF_YIELD_TOTAL in SENSOR_DEFS
        assert CONF_STATE_OF_CHARGE in SENSOR_DEFS
        assert CONF_DC_INPUT_VOLTAGE in SENSOR_DEFS
        assert CONF_DC_INPUT_POWER in SENSOR_DEFS


class TestTextSensorConstants:
    def test_text_sensor_count(self):
        from components.victron.text_sensor import TEXT_SENSORS

        assert len(TEXT_SENSORS) == 15

    def test_first_and_last_text_sensor(self):
        from components.victron.text_sensor import (
            CONF_CHARGING_MODE,
            CONF_OFF_REASON,
            TEXT_SENSORS,
        )

        assert TEXT_SENSORS[0] == CONF_CHARGING_MODE
        assert TEXT_SENSORS[-1] == CONF_OFF_REASON

    def test_known_text_sensors(self):
        from components.victron.text_sensor import (
            CONF_CHARGING_MODE,
            CONF_ERROR,
            CONF_FIRMWARE_VERSION,
            CONF_OFF_REASON,
            CONF_TRACKING_MODE,
            TEXT_SENSORS,
        )

        assert CONF_CHARGING_MODE in TEXT_SENSORS
        assert CONF_ERROR in TEXT_SENSORS
        assert CONF_TRACKING_MODE in TEXT_SENSORS
        assert CONF_FIRMWARE_VERSION in TEXT_SENSORS
        assert CONF_OFF_REASON in TEXT_SENSORS


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        from components.victron.binary_sensor import BINARY_SENSOR_DEFS

        assert len(BINARY_SENSOR_DEFS) == 2

    def test_known_binary_sensors(self):
        from components.victron.binary_sensor import (
            BINARY_SENSOR_DEFS,
            CONF_LOAD_STATE,
            CONF_RELAY_STATE,
        )

        assert CONF_LOAD_STATE in BINARY_SENSOR_DEFS
        assert CONF_RELAY_STATE in BINARY_SENSOR_DEFS
