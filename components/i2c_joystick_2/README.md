# I2C Joystick 2 Unit


The `i2c_joystick_2` component provides support for the M5Stack Unit Joystick 2, a two-axis analog joystick with a push button and RGB LED feedback. It communicates via I2C and exposes axis position, button state, and RGB LED control.

## Component/Hub

The I2C component is required to use this device.

```yaml
# Example configuration entry
i2c:
  sda: GPIO2
  scl: GPIO1

i2c_joystick_2:
  id: joystick2_device
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the I2C Joystick 2 component.
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x63`.
- All other options from [I2C Component](https://esphome.io/components/i2c#config-i2c).

## Sensor

The sensor platform exposes the X and Y axis positions of the joystick. Each axis can be configured independently with different output modes.

```yaml
sensor:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick X"
    axis: X
    mode: OFFSET_12BIT
    update_interval: 100ms

  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick Y"
    axis: Y
    mode: OFFSET_12BIT
    update_interval: 100ms
```

### Configuration variables

- **i2c_joystick_2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the I2C Joystick 2 component. Can be omitted when only one joystick is configured.
- **axis** (*Optional*, enum): The joystick axis to read. Defaults to `X`.
  - `X`: Horizontal axis
  - `Y`: Vertical axis
- **mode** (*Optional*, enum): The output mode for axis readings. Defaults to `OFFSET_12BIT`.
  - `ADC_16BIT`: Raw 16-bit ADC value (0–4095)
  - `ADC_8BIT`: Raw 8-bit ADC value (0–255)
  - `OFFSET_12BIT`: 12-bit signed offset from center (-2048 to +2047)
  - `OFFSET_8BIT`: 8-bit signed offset from center (-128 to +127)
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): Polling interval for axis readings. Defaults to `100ms`.
- All other options from [Sensor](https://esphome.io/components/sensor#config-sensor).

### Output Modes

| Mode | Range | Description |
|---|---|---|
| `ADC_16BIT` | 0 to 4095 | Raw 12-bit ADC value (stored in 16-bit register) |
| `ADC_8BIT` | 0 to 255 | Raw 8-bit ADC value |
| `OFFSET_12BIT` | -4095 to +4095 | Signed offset from center position (12-bit) |
| `OFFSET_8BIT` | -127 to +127 | Signed offset from center position (8-bit) |

> **Note**: The `OFFSET_*` modes automatically calibrate the center position and return signed values. Positive values indicate movement toward the right (X-axis) or up (Y-axis).

## Binary Sensor

The binary sensor platform exposes the joystick push button state.

```yaml
binary_sensor:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick Button"
    update_interval: 100ms
```

### Configuration variables

- **i2c_joystick_2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the I2C Joystick 2 component. Can be omitted when only one joystick is configured.
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): Polling interval for button state. Defaults to `100ms`.
- All other options from [Binary Sensor](https://esphome.io/components/binary_sensor#config-binary-sensor).

> **Note**: The button returns `true` when pressed and `false` when released. The hardware inverts the logic internally (0 = pressed, 1 = released).

## Output

The output platform provides individual RGB channel control for the joystick's built-in LED.

```yaml
output:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_R
    id: red_output

  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_G
    id: green_output

  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_B
    id: blue_output
```

### Configuration variables

- **i2c_joystick_2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the I2C Joystick 2 component. Can be omitted when only one joystick is configured.
- **channel** (*Required*, enum): The RGB channel to control.
  - `CHANNEL_R`: Red channel
  - `CHANNEL_G`: Green channel
  - `CHANNEL_B`: Blue channel
- All other options from [Float Output](https://esphome.io/components/output#float-output).

## Light

The RGB outputs can be combined to create a full-color light entity.

```yaml
light:
  - platform: rgb
    name: "Joystick LED"
    red: red_output
    green: green_output
    blue: blue_output
```

### Configuration variables

All options from [RGB Light](https://esphome.io/components/light/rgb) are supported.

## Full Configuration Example

```yaml
i2c:
  sda: GPIO2
  scl: GPIO1

i2c_joystick_2:
  id: joystick2_device

sensor:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick X"
    axis: X
    mode: OFFSET_12BIT
    update_interval: 100ms
    filters:
      - delta: 3.0

  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick Y"
    axis: Y
    mode: OFFSET_12BIT
    update_interval: 100ms
    filters:
      - delta: 3.0 

binary_sensor:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    name: "Joystick Button"
    update_interval: 100ms

output:
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_R
    id: red_output
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_G
    id: green_output
  - platform: i2c_joystick_2
    i2c_joystick_2_id: joystick2_device
    channel: CHANNEL_B
    id: blue_output

light:
  - platform: rgb
    name: "Joystick LED"
    id: joystick_led
    red: red_output
    green: green_output
    blue: blue_output
```

## Automation Example

```yaml
# Change LED color based on joystick position
sensor:
  - platform: i2c_joystick_2
    id: joy_x
    axis: X
    mode: OFFSET_8BIT
    on_value:
      then:
        - if:
            condition:
              lambda: 'return x > 50;'
            then:
              - light.turn_on:
                  id: joystick_led
                  red: 100%
                  green: 0%
                  blue: 0%
        - if:
            condition:
              lambda: 'return x < -50;'
            then:
              - light.turn_on:
                  id: joystick_led
                  red: 0%
                  green: 0%
                  blue: 100%

binary_sensor:
  - platform: i2c_joystick_2
    name: "Joystick Button"
    on_press:
      - light.toggle: joystick_led
```
