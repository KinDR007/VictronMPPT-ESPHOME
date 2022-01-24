# VictronMPPT-ESPHOME

ESPHome component to monitor a Victron MPPT Charger via ve.direct / UART TTL

## Supported devices

All Victron MPPT Chargers providing a ve.direct port.

## Tested devices

  * Victron SmartSolar MPPT 75/15
  * Victron SmartSolar MPPT 100/20
  * Victron SmartSolar MPPT 150/35
  * Victron SmartSolar MPPT 250/70
  * Victron Phoenix Inverter 12/500

## Requirements

* [ESPHome 2021.10 or higher](https://github.com/esphome/esphome/releases).
* Generic ESP32 or ESP8266 board

## Schematics

```
                UART-TTL
┌────────────────┐                ┌──────────────────┐
│           GND o│<-------------->│o GND             │
│ Victron    TX o│--------------->│o D7   ESP32/     │
│ Charger    RX o│                │       ESP8266    │<-- GND
│            5V o│                │                  │<-- 3.3V
└────────────────┘                └──────────────────┘

# UART-TTL jack (JST-PH 2.0mm pinch)
┌─── ─────── ────┐
│                │
│ O   O   O   O  │
│GND  RX  TX VCC │
└────────────────┘
```

If you are unsure about to pin order please measure the voltage between GND and VCC (5V). If you measure a positive voltage you know the position of VCC and GND!

### JST-PH jack

| Pin     | Purpose      | ESP pin        |
| :-----: | :----------- | :------------- |
|  **1**  | **GND**      | GND            |
|    2    | RX           |                |
|  **3**  | **TX**       | D7 (RX)        |
|    4    | 5V           |                |

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://KinDR007/VictronMPPT-ESPHOME@main

uart:
  id: uart0
  tx_pin: D8  # Not used! The communication is read-only
  rx_pin: D7
  baud_rate: 19200
  rx_buffer_size: 256

victron:
  id: victron0
  uart_id: uart0

sensor:
  - platform: victron
    victron_id: victron0
    panel_voltage:
      name: "Panel voltage"
    battery_voltage:
      name: "Battery voltage"
    battery_current:
      name: "Battery current"
```

or just use the `esp8266-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/KinDR007/VictronMPPT-ESPHOME.git
cd VictronMPPT-ESPHOME

# Create a secret.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD

wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp8266-example.yaml

```

The `uart_id` and `victron_id` is optional if you use a single UART / victron device. All sensors are optional.

The available numeric sensors are:
- `max_power_yesterday`
- `max_power_today`
- `yield_total`
- `yield_yesterday`
- `yield_today`
- `panel_voltage`
- `panel_power`
- `battery_current`
- `day_number`
- `charging_mode_id`
- `error_code`
- `tracking_mode_id`
- `load_current`

The available text sensors are:
- `charging_mode`
- `error`
- `tracking_mode`
- `firmware_version`
- `device_type`

Binary sensors:

- `load_state`
- `relay_state`

Big thanks for help to ssieb for the support!
