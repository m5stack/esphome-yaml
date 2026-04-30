# Unit EXT.IO2 /Stamp IO


The `extio2` component provides support for the M5Stack Unit EXT.IO2 / Stamp IO, an 8-channel I2C GPIO expander. Each pin can be independently configured as a digital input, digital output, ADC input, PWM output, servo output, or RGB LED output. All sub-components share a single I2C hub.

## Dependencies

- [`i2c`](https://esphome.io/components/i2c)

## Component/Hub

```yaml
i2c:
  sda: GPIO2
  scl: GPIO1

extio2:
  id: extio2_hub
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the EXT.IO2 hub. Required when multiple EXT.IO2 units are used.
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x45`.
- **reset** (*Optional*, boolean): Reset all pins to digital input mode during setup. Defaults to `true`.
- **pwm_freq** (*Optional*, enum): Global PWM frequency applied to all PWM channels. Defaults to `1kHz`.
  - `2kHz`
  - `1kHz`
  - `500Hz`
  - `250Hz`
  - `125Hz`
- All other options from [I2C Device](https://esphome.io/components/i2c#config-i2c).

## GPIO Pin

EXT.IO2 pins can be used anywhere ESPHome accepts a standard GPIO pin reference, such as `switch`, `binary_sensor`, or `output` components.

```yaml
# Digital output — switch
switch:
  - platform: gpio
    name: "EXT.IO2 GPIO 0"
    pin:
      extio2_id: extio2_hub
      number: 0
      mode:
        output: true

# Digital input — binary sensor
binary_sensor:
  - platform: gpio
    name: "EXT.IO2 GPIO 1"
    pin:
      extio2_id: extio2_hub
      number: 1
      mode:
        input: true
        pullup: true
```

### Pin configuration variables

- **extio2_id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the EXT.IO2 hub.
- **number** (*Required*, int): The pin number. Range: `0`–`7`.
- **inverted** (*Optional*, boolean): Invert the pin logic. Defaults to `false`.
- **mode** (*Optional*): Pin mode
  - **input** (*boolean*): Configure as digital input.
  - **output** (*boolean*): Configure as digital output.

The internal firmware only support input / output

## Sensor (ADC)

The sensor platform reads analog values from any of the 8 ADC-capable pins.

```yaml
sensor:
  - platform: extio2
    extio2_id: extio2_hub
    name: "ADC Sensor 0"
    channel: ADC_0
    resolution: 12BIT
    update_interval: 5s
```

### Configuration variables

- **extio2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the EXT.IO2 hub.
- **channel** (*Required*, enum): The ADC channel to read. `ADC_0` through `ADC_7`.
- **resolution** (*Optional*, enum): ADC resolution. Defaults to `8BIT`.
  - `8BIT`: 8-bit resolution, range 0–255.
  - `12BIT`: 12-bit resolution, range 0–4095.
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): Polling interval. Defaults to `60s`.
- All other options from [Sensor](https://esphome.io/components/sensor#config-sensor).

## Output (PWM)

The output platform drives any pin as a PWM output. The duty cycle is expressed as a float from `0.0` to `1.0`, mapped internally to 0–100%.

```yaml
output:
  - platform: extio2
    extio2_id: extio2_hub
    id: pwm_ch0
    channel: PWM_0

light:
  - platform: monochromatic
    name: "PWM Light 0"
    output: pwm_ch0
    effects:
      - pulse:
```

### Configuration variables

- **extio2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the EXT.IO2 hub.
- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of this output.
- **channel** (*Required*, enum): The PWM channel to use. `PWM_0` through `PWM_7`.
- All other options from [Float Output](https://esphome.io/components/output#float-output).

> **Note**: The PWM frequency is set globally on the hub via `pwm_freq`. All PWM channels share the same frequency.

## Number (Servo)

The number platform controls servo motors. Two control modes are available: angle (degrees) and pulse width (microseconds).

### Angle mode

Commands the servo to a position in degrees (0–180°).

```yaml
number:
  - platform: extio2
    extio2_id: extio2_hub
    name: "Servo 0"
    type: angle
    channel: SERVO_0
    min_value: 0
    max_value: 180
    step: 3
```

### Pulse mode

Commands the servo using a raw pulse width in microseconds.

```yaml
number:
  - platform: extio2
    extio2_id: extio2_hub
    name: "Servo 1"
    type: pulse
    channel: SERVO_1
    min_value: 500
    max_value: 2500
    step: 100
```

### Configuration variables

- **extio2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the EXT.IO2 hub.
- **type** (*Required*, enum): Servo control mode.
  - `angle`: Control by angle in degrees.
  - `pulse`: Control by pulse width in microseconds.
- **channel** (*Required*, enum): The servo channel to use. `SERVO_0` through `SERVO_7`.
- **min_value** (*Optional*, float): Minimum value. Defaults to `0.0` (angle) or `500.0` (pulse).
- **max_value** (*Optional*, float): Maximum value. Defaults to `180.0` (angle) or `2500.0` (pulse).
- **step** (*Optional*, float): Step size. Defaults to `3.0` (angle) or `100.0` (pulse).
- All other options from [Number](https://esphome.io/components/number#config-number).

| Mode | Unit | Min | Max | Step |
|---|---|---|---|---|
| `angle` | `°` | `0` | `180` | `3` |
| `pulse` | `us` | `500` | `2500` | `100` |

## Light

The light platform supports two modes: a single RGB LED on one pin, or an addressable LED strip across multiple consecutive pins starting from pin 0.

### Single RGB light

Controls one RGB LED on a specific pin.

```yaml
light:
  - platform: extio2
    extio2_id: extio2_hub
    name: "EXT.IO2 Light 2"
    type: light
    channel: LIGHT_2
```

### Addressable light

Controls multiple RGB LEDs across consecutive pins starting from pin 0. Supports all ESPHome addressable light effects.

```yaml
light:
  - platform: extio2
    extio2_id: extio2_hub
    name: "Light Bar"
    type: addressable_light
    num_leds: 8
```

### Configuration variables

- **extio2_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the EXT.IO2 hub.
- **type** (*Required*, enum): Light mode.
  - `light`: Single RGB LED on one pin.
  - `addressable_light`: Multiple RGB LEDs across consecutive pins.
- **channel** (*Required* for `light` type, enum): The pin to use. `LIGHT_0` through `LIGHT_7`.
- **num_leds** (*Required* for `addressable_light` type, int): Number of LEDs. Range: `1`–`8`. Pins are allocated starting from pin 0.
- All other options from [Light](https://esphome.io/components/light).

> **Note**: Addressable light pins must be contiguous and start from pin 0. For example, `num_leds: 3` uses pins 0, 1, and 2.

> **Note**: There is no hardware brightness control register. Brightness is applied by scaling the RGB values in software.

## Pin Function Summary

Each pin supports one function at a time. The function is set automatically when the corresponding sub-component is configured.

| Function | Mode constant | Sub-component |
|---|---|---|
| Digital input | `PIN_INPUT` | GPIO pin (`input: true`) |
| Digital output | `PIN_OUTPUT` | GPIO pin (`output: true`) |
| ADC 8-bit | `PIN_ADC_8` | `sensor` (`resolution: 8BIT`) |
| ADC 12-bit | `PIN_ADC_12` | `sensor` (`resolution: 12BIT`) |
| Servo angle | `PIN_SERVO_8` | `number` (`type: angle`) |
| Servo pulse | `PIN_SERVO_16` | `number` (`type: pulse`) |
| RGB LED | `PIN_RGB` | `light` |
| PWM output | `PIN_PWM` | `output` |

## Register Map

| Register | Address | Access | Description |
|---|---|---|---|
| `REG_MODE_BASE` | `0x00`–`0x07` | R/W | Pin mode (one register per pin) |
| `REG_OUTPUT_BASE` | `0x10`–`0x17` | R/W | Digital output state |
| `REG_INPUT_BASE` | `0x20`–`0x27` | R | Digital input state |
| `REG_ADC_8BIT_BASE` | `0x30`–`0x37` | R | 8-bit ADC value |
| `REG_ADC_12BIT_BASE` | `0x40`–`0x47` | R | 12-bit ADC value (2 bytes per pin) |
| `REG_SERVO_8BIT_BASE` | `0x50`–`0x57` | W | Servo angle (0–180°) |
| `REG_SERVO_16BIT_BASE` | `0x60`–`0x67` | W | Servo pulse width in µs (2 bytes per pin) |
| `REG_RGB_BASE` | `0x70`–`0x87` | W | RGB888 color (3 bytes per pin) |
| `REG_PWM_BASE` | `0x90`–`0x97` | W | PWM duty cycle (0–100) |
| `REG_PWM_FREQ` | `0xA0` | W | Global PWM frequency |
| `REG_FIRM_VER` | `0xFE` | R | Firmware version |
| `REG_I2C_ADDR` | `0xFF` | R/W | I2C address |

## Full Configuration Example

```yaml
i2c:
  sda: GPIO2
  scl: GPIO1

extio2:
  id: extio2_hub
  address: 0x45
  reset: true
  pwm_freq: 1kHz

# GPIO — digital output
switch:
  - platform: gpio
    name: "EXT.IO2 GPIO 0"
    pin:
      extio2_id: extio2_hub
      number: 0
      mode:
        output: true

# GPIO — digital input
binary_sensor:
  - platform: gpio
    name: "EXT.IO2 GPIO 1"
    pin:
      extio2_id: extio2_hub
      number: 1
      mode:
        input: true
        pullup: true

# ADC sensor
sensor:
  - platform: extio2
    extio2_id: extio2_hub
    name: "ADC Sensor 2"
    channel: ADC_2
    resolution: 12BIT
    update_interval: 5s

# PWM output used as a monochromatic light
output:
  - platform: extio2
    extio2_id: extio2_hub
    id: pwm_ch3
    channel: PWM_3

light:
  - platform: monochromatic
    name: "PWM Light 3"
    output: pwm_ch3
    effects:
      - pulse:

# Servo — angle control
number:
  - platform: extio2
    extio2_id: extio2_hub
    name: "Servo 4"
    type: angle
    channel: SERVO_4

  # Servo — pulse width control
  - platform: extio2
    extio2_id: extio2_hub
    name: "Servo 5"
    type: pulse
    channel: SERVO_5

# Single RGB LED
  - platform: extio2
    extio2_id: extio2_hub
    name: "RGB Light 6"
    type: light
    channel: LIGHT_6

# Addressable LED strip (all 8 pins)
  - platform: extio2
    extio2_id: extio2_hub
    name: "Light Bar"
    type: addressable_light
    num_leds: 8
```
