#Victron solar charge controller

A configured uart component is required.

Example:
```yaml
victron:
  uart_id: the_uart

sensor:
  - platform: victron
    panel_voltage:
      id: pv
    battery_voltage:
      id: bv
    battery_current:
      id: bc
```

The `uart_id` is optional.

All sensors are optional.

The available numeric sensors are
for MPPT:
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
- `tracking_mode_id`
- `load_current`

for inverters:
- `ac_out_voltage`
- `ac_out_current`
- `battery_voltage`
- `warning_code`
- `charger_status` (= State of operation)
- `device_mode_id`


The available text sensors are
for MPPT:
- `device_type`
- `charging_mode`
- `error`
- `tracking_mode`
- `firmware_version`

for inverters:
- `device_type`
- `device_mode`
- `warning`
- `firmware_version`


Example for multiple devices:
```yaml
victron:
  - id: victron_mppt
    uart_id: the_uart_of_mppt
  - id: victron_inverter
    uart_id: the_uart_of_inverter

text_sensor:
  - platform: victron
    victron_id: victron_mppt
    device_type:
      name: "MPPT device type"
  - platform: victron
    victron_id: victron_inverter
    device_type:
      name: "Inverter device type"
```

Binary sensors:

```yaml
binary_sensor:
  - platform: victron
    victron_id: victron_inverter
    load_state:
      name: "Load state"
    relay_state:
      name: "Relay state"
```
