# VictronMPPT-ESPHOME

ESPHome component to monitor a Victron MPPT Charger via ve.direct / UART TTL

## Supported devices

All Victron MPPT Chargers providing a ve.direct port.

## Tested devices

  * Victron SmartSolar MPPT 100/20
  * Victron SmartSolar MPPT 150/35

## Requirements

* [ESPHome 1.18.0 or higher](https://github.com/esphome/esphome/releases).
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

or just use the `esp8266-example.yaml` or `esp8266-example-advanced.yaml` as proof of concept. The `uart_id` and `victron_id` is optional if you use a single UART / victron device. All sensors are optional.

The available numeric sensors are:
- `max_power_yesterday`
- `max_power_today`
- `yield_total`
- `yield_yesterday`
- `yield_today`
- `panel_voltage`
- `panel_power`
- `battery_voltage`
- `battery_current`
- `day_number`
- `charger_status`
- `error_code`
- `tracker_operation`

The available text sensors are:
- `charger_text`
- `error_text`
- `tracker_text`
- `fw_version`
- `pid`

Big thanks for help to ssieb for the support!
