# M5IOE1 GPIO Expander and Multi-Function I/O Module

The M5IOE1 is a versatile I/O expansion module that provides 14 GPIO pins with multiple functions including digital I/O, PWM output, ADC input, temperature sensing, and addressable LED control (NeoPixel). It communicates via I2C and integrates an AW8737A audio amplifier driver. It was built on top of a PY32L020 MCU and has internal firmware made by M5Stack.

For detailed specifications, refer to the [M5IOE1 Datasheet](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1210/IO_Expander_Datasheet_EN.pdf).

## Component/Hub

The I2C component is required to use this device.

```yaml
# Example configuration entry
m5ioe1:
  id: m5ioe1_hub
  reset: true
  pwm_frequency: 500Hz
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the M5IOE1 component.
- **reset** (*Optional*, boolean): Perform software reset on initialization. Defaults to `true`.
- **pwm_frequency** (*Optional*, frequency): PWM frequency for all PWM channels. Range: 1 Hz to 65535 Hz. Defaults to `500Hz`.
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x6F`.
- All other options from [I2C Component](https://esphome.io/components/i2c#config-i2c).

## GPIO Pin

Individual GPIO pins can be configured for digital input or output with various modes.

```yaml
# Example GPIO pin configuration

# GPIO Binary Sensor
binary_sensor:
  - platform: gpio
    name: "M5IOE1 Input"
    pin:
      m5ioe1_id: m5ioe1_hub
      number: 0
      mode:
        input: true
        pullup: true
      inverted: false

# GPIO Switch
switch:
  - platform: gpio
    name: "M5IOE1 Output"
    pin:
      m5ioe1_id: m5ioe1_hub
      number: 1
      mode:
        output: true
        open_drain: false
```

### Pin Configuration variables

- **m5ioe1_id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the M5IOE1 component.
- **number** (*Required*, int): The pin number. Range: 0 to 13.
- **mode** (*Required*): Pin mode configuration.
  - **input** (*Optional*, boolean): Set pin as input.
  - **output** (*Optional*, boolean): Set pin as output.
  - **pullup** (*Optional*, boolean): Enable internal pull-up resistor (input mode only).
  - **pulldown** (*Optional*, boolean): Enable internal pull-down resistor (input mode only).
  - **open_drain** (*Optional*, boolean): Enable open-drain mode (output mode only).
- **inverted** (*Optional*, boolean): Invert the pin logic. Defaults to `false`.

## Output

The M5IOE1 output platform provides PWM control for up to 4 channels.

```yaml
output:
  - platform: m5ioe1
    id: pwm_output_1
    m5ioe1_id: m5ioe1_hub
    channel: PWM_1
    pwm_polarity: ACTIVE_HIGH
  - platform: m5ioe1
    id: pwm_output_2
    m5ioe1_id: m5ioe1_hub
    channel: PWM_2
    pwm_polarity: ACTIVE_LOW
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this output component.
- **m5ioe1_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the M5IOE1 component. If you have only one M5IOE1 component, this can be omitted.
- **channel** (*Required*, enum): The PWM channel to control. Must be one of `PWM_1`, `PWM_2`, `PWM_3`, or `PWM_4`.
- **pwm_polarity** (*Optional*, enum): PWM output polarity. Defaults to `ACTIVE_HIGH`.
  - `ACTIVE_HIGH`: PWM signal is active high
  - `ACTIVE_LOW`: PWM signal is active low
- All other options from [Float Output](https://esphome.io/components/output#float-output).

## Light

The M5IOE1 supports addressable RGB LEDs (NeoPixel) with up to 32 LEDs. The internal driver uses RGB565 format. Connect the addressable light to `Pin 14` 

```yaml
light:
  - platform: m5ioe1
    id: m5ioe1_leds
    m5ioe1_id: m5ioe1_hub
    num_led: 10
    name: "M5IOE1 RGB LEDs"
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this light component.
- **m5ioe1_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the M5IOE1 component. If you have only one M5IOE1 component, this can be omitted.
- **num_led** (*Required*, int): Number of addressable LEDs. Range: 0 to 32.
- All other options from [Addressable Light](https://esphome.io/components/light#addressable-light).

## Sensor

The M5IOE1 sensor platform provides ADC readings from up to 4 analog input channels.

```yaml
sensor:
  - platform: m5ioe1
    id: adc_channel_1
    m5ioe1_id: m5ioe1_hub
    channel: ADC_1
    update_interval: 60s
    name: "M5IOE1 ADC Channel 1"
  - platform: m5ioe1
    id: adc_channel_2
    m5ioe1_id: m5ioe1_hub
    channel: ADC_2
    update_interval: 60s
    name: "M5IOE1 ADC Channel 2"
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this sensor component.
- **m5ioe1_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the M5IOE1 component. If you have only one M5IOE1 component, this can be omitted.
- **channel** (*Required*, enum): The ADC channel to read. Must be one of:
  - `ADC_NONE`: No ADC channel (placeholder)
  - `ADC_1`: ADC channel 1
  - `ADC_2`: ADC channel 2
  - `ADC_3`: ADC channel 3
  - `ADC_4`: ADC channel 4
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): The interval to check the sensor. Defaults to `60s`.
- All other options from [Sensor](https://esphome.io/components/sensor#config-sensor).

## Switch

The M5IOE1 switch platform controls the integrated AW8737A audio amplifier driver via pulse generation.

```yaml
switch:
  - platform: m5ioe1
    id: aw8737a_control
    m5ioe1_id: m5ioe1_hub
    pin: 0
    pulse_num: 2
    name: "AW8737A Amplifier"
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this switch component.
- **m5ioe1_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the M5IOE1 component. If you have only one M5IOE1 component, this can be omitted.
- **pin** (*Required*, int): The GPIO pin connected to the AW8737A. Range: 0 to 13 (corresponding to IO1-IO14).
- **pulse_num** (*Required*, int): Number of pulses to generate. Range: 0 to 3.
- All other options from [Switch](https://esphome.io/components/switch#config-switch).

## Full Configuration Example

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22

m5ioe1:
  id: m5ioe1_hub
  reset: true
  pwm_frequency: 1000Hz

# GPIO pins as binary sensor and switch
binary_sensor:
  - platform: gpio
    name: "M5IOE1 Button"
    pin:
      m5ioe1_id: m5ioe1_hub
      number: 0
      mode:
        input: true
        pullup: true
      inverted: true

switch:
  - platform: gpio
    name: "M5IOE1 Relay"
    pin:
      m5ioe1_id: m5ioe1_hub
      number: 1
      mode:
        output: true

# PWM outputs
output:
  - platform: m5ioe1
    id: pwm_ch1
    m5ioe1_id: m5ioe1_hub
    channel: PWM_1
    pwm_polarity: ACTIVE_HIGH
  - platform: m5ioe1
    id: pwm_ch2
    m5ioe1_id: m5ioe1_hub
    channel: PWM_2
    pwm_polarity: ACTIVE_HIGH

# PWM-controlled lights
light:
  - platform: monochromatic
    name: "M5IOE1 PWM Light 1"
    output: pwm_ch1
  - platform: monochromatic
    name: "M5IOE1 PWM Light 2"
    output: pwm_ch2

# Addressable RGB LEDs
  - platform: m5ioe1
    id: rgb_leds
    m5ioe1_id: m5ioe1_hub
    num_led: 10
    name: "M5IOE1 RGB Strip"

# ADC sensors
sensor:
  - platform: m5ioe1
    id: adc1
    m5ioe1_id: m5ioe1_hub
    channel: ADC_1
    update_interval: 10s
    name: "M5IOE1 ADC 1"
  - platform: m5ioe1
    id: adc2
    m5ioe1_id: m5ioe1_hub
    channel: ADC_2
    update_interval: 10s
    name: "M5IOE1 ADC 2"

# AW8737A amplifier control
switch:
  - platform: m5ioe1
    id: amplifier
    m5ioe1_id: m5ioe1_hub
    pin: 5
    pulse_num: 2
    name: "Audio Amplifier"
```