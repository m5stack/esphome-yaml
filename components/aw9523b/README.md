# AW9523B 16-Channel GPIO Expander with LED Driver

The AW9523B is a 16-channel GPIO expander with integrated LED driver functionality. It communicates via I2C and provides configurable digital I/O pins with LED current control capability. Port 0 (P0) supports both open-drain and push-pull output modes.

For detailed specifications, refer to the [AW9523B Datasheet](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/products/core/CoreS3/AW9523B-EN.pdf).

## Component/Hub

The I2C component is required to use this device.

```yaml
# Example configuration entry
aw9523b:
  id: aw9523b_hub
  reset: true
  p0_drive_mode: OPEN_DRAIN
  led_max_current: 37mA
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the AW9523B component.
- **reset** (*Optional*, boolean): Perform software reset on initialization. Defaults to `true`.
- **p0_drive_mode** (*Optional*, enum): Output drive mode for Port 0 (pins 0-7). Defaults to `OPEN_DRAIN`.
  - `OPEN_DRAIN`: Open-drain output mode
  - `PUSH_PULL`: Push-pull output mode
- **led_max_current** (*Optional*, enum): Global maximum current limit for LED mode. Defaults to `37mA`.
  - `37mA`: Maximum current (Imax × 1)
  - `27.75mA`: 75% of maximum current (Imax × 0.75)
  - `18.5mA`: 50% of maximum current (Imax × 0.5)
  - `9.25mA`: 25% of maximum current (Imax × 0.25)
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x58`.
- All other options from [I2C Component](https://esphome.io/components/i2c#config-i2c).

## GPIO Pin

Individual GPIO pins can be configured for digital input or output.

```yaml
# Example GPIO pin configuration
binary_sensor:
  - platform: gpio
    name: "AW9523B Input"
    pin:
      aw9523b_id: aw9523b_hub
      number: 0
      mode:
        input: true
      inverted: false

switch:
  - platform: gpio
    name: "AW9523B Output"
    pin:
      aw9523b_id: aw9523b_hub
      number: 8
      mode:
        output: true
```

### Pin Configuration variables

- **aw9523b_id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the AW9523B component.
- **number** (*Required*, int): The pin number. Range: 0 to 15.
  - Pins 0-7: Port 0 (P0_0 to P0_7)
  - Pins 8-15: Port 1 (P1_0 to P1_7)
- **mode** (*Required*): Pin mode configuration.
  - **input** (*Optional*, boolean): Set pin as input.
  - **output** (*Optional*, boolean): Set pin as output.
- **inverted** (*Optional*, boolean): Invert the pin logic. Defaults to `false`.

## Output

The AW9523B output platform provides LED current control for individual pins. When configured as an output, the pin operates in LED mode with adjustable brightness.

```yaml
output:
  - platform: aw9523b
    id: led_output_0
    aw9523b_id: aw9523b_hub
    pin: 0
  - platform: aw9523b
    id: led_output_1
    aw9523b_id: aw9523b_hub
    pin: 1
  - platform: aw9523b
    id: led_output_2
    aw9523b_id: aw9523b_hub
    pin: 2
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this output component.
- **aw9523b_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the AW9523B component. If you have only one AW9523B component, this can be omitted.
- **pin** (*Required*, int): The pin number to control. Range: 0 to 15.
- All other options from [Float Output](https://esphome.io/components/output#float-output).

## Light

The AW9523B outputs can be used to control various types of lights including monochromatic, RGB, and RGBW configurations.

### Monochromatic Light

```yaml
light:
  - platform: monochromatic
    name: "AW9523B LED 1"
    output: led_output_0
```

### RGB Light

```yaml
output:
  - platform: aw9523b
    id: red_output
    aw9523b_id: aw9523b_hub
    pin: 0
  - platform: aw9523b
    id: green_output
    aw9523b_id: aw9523b_hub
    pin: 1
  - platform: aw9523b
    id: blue_output
    aw9523b_id: aw9523b_hub
    pin: 2

light:
  - platform: rgb
    name: "AW9523B RGB Light"
    red: red_output
    green: green_output
    blue: blue_output
```

### RGBW Light

```yaml
output:
  - platform: aw9523b
    id: red_output
    aw9523b_id: aw9523b_hub
    pin: 0
  - platform: aw9523b
    id: green_output
    aw9523b_id: aw9523b_hub
    pin: 1
  - platform: aw9523b
    id: blue_output
    aw9523b_id: aw9523b_hub
    pin: 2
  - platform: aw9523b
    id: white_output
    aw9523b_id: aw9523b_hub
    pin: 3

light:
  - platform: rgbw
    name: "AW9523B RGBW Light"
    red: red_output
    green: green_output
    blue: blue_output
    white: white_output
```

### Light Configuration Variables

All options from [Light](https://esphome.io/components/light#config-light) are supported.

## Full Configuration Example

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22

aw9523b:
  id: aw9523b_hub
  reset: true
  p0_drive_mode: PUSH_PULL
  led_max_current: 37mA

# GPIO pins as binary sensor and switch
binary_sensor:
  - platform: gpio
    name: "AW9523B Button 1"
    pin:
      aw9523b_id: aw9523b_hub
      number: 8
      mode:
        input: true
      inverted: true

  - platform: gpio
    name: "AW9523B Button 2"
    pin:
      aw9523b_id: aw9523b_hub
      number: 9
      mode:
        input: true
      inverted: true

switch:
  - platform: gpio
    name: "AW9523B Relay 1"
    pin:
      aw9523b_id: aw9523b_hub
      number: 10
      mode:
        output: true

  - platform: gpio
    name: "AW9523B Relay 2"
    pin:
      aw9523b_id: aw9523b_hub
      number: 11
      mode:
        output: true

# LED outputs
output:
  - platform: aw9523b
    id: red_led
    aw9523b_id: aw9523b_hub
    pin: 0
  - platform: aw9523b
    id: green_led
    aw9523b_id: aw9523b_hub
    pin: 1
  - platform: aw9523b
    id: blue_led
    aw9523b_id: aw9523b_hub
    pin: 2
  - platform: aw9523b
    id: white_led
    aw9523b_id: aw9523b_hub
    pin: 3

# RGB Light
light:
  - platform: rgb
    name: "AW9523B RGB Status Light"
    red: red_led
    green: green_led
    blue: blue_led

# Monochromatic Light
  - platform: monochromatic
    name: "AW9523B White Light"
    output: white_led
```