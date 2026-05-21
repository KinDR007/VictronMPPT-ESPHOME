"""Component schema tests."""


class TestSensorConstants:
    def test_sensor_count(self):
        from components.victron.sensor import SENSORS

        assert len(SENSORS) == 54

    def test_known_sensors(self):
        from components.victron.sensor import (
            CONF_BATTERY_VOLTAGE,
            CONF_PANEL_VOLTAGE,
            CONF_STATE_OF_CHARGE,
            SENSORS,
        )

        assert CONF_BATTERY_VOLTAGE in SENSORS
        assert CONF_PANEL_VOLTAGE in SENSORS
        assert CONF_STATE_OF_CHARGE in SENSORS


class TestTextSensorConstants:
    def test_text_sensor_count(self):
        from components.victron.text_sensor import TEXT_SENSORS

        assert len(TEXT_SENSORS) == 15

    def test_known_text_sensors(self):
        from components.victron.text_sensor import (
            CONF_CHARGING_MODE,
            CONF_ERROR,
            TEXT_SENSORS,
        )

        assert CONF_CHARGING_MODE in TEXT_SENSORS
        assert CONF_ERROR in TEXT_SENSORS


class TestBinarySensorConstants:
    def test_binary_sensor_count(self):
        from components.victron.binary_sensor import BINARY_SENSORS

        assert len(BINARY_SENSORS) == 2

    def test_known_binary_sensors(self):
        from components.victron.binary_sensor import (
            BINARY_SENSORS,
            CONF_LOAD_STATE,
            CONF_RELAY_STATE,
        )

        assert CONF_LOAD_STATE in BINARY_SENSORS
        assert CONF_RELAY_STATE in BINARY_SENSORS
