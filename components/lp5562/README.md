# LP5562 Four-Channel RGB- or White-LED Driver

The LP5562 is a four-channel programmable LED driver supporting RGB or RGBW configurations. It communicates via I2C and provides PWM control for LED brightness and color mixing.

## Component/Hub

The I2C component is required to use this device.

```yaml
# Example configuration entry
lp5562:
  id: lp5562_led
  use_internal_clk: true
  power_save_mode: true
  high_pwm_freq: true
  logarithmic_dimming: true
  red_current: 15.5
  green_current: 16.5
  blue_current: 22.5
  white_current: 21.5
```

### Configuration variables

- **id** (*Optional*, [ID](/guides/configuration-types#id)): Specify an ID for the LP5562 component.
- **use_internal_clk** (*Optional*, boolean): Use the internal clock instead of external clock. Defaults to `false`.
- **power_save_mode** (*Optional*, boolean): Enable power save mode to reduce power consumption. Defaults to `false`.
- **high_pwm_freq** (*Optional*, boolean): Enable high PWM frequency for better LED flickering control. Defaults to `false`.
- **logarithmic_dimming** (*Optional*, boolean): Enable logarithmic dimming curve for more natural brightness adjustment. Defaults to `false`.
- **red_current** (*Optional*, float): Set the maximum current for the red channel in mA. Range: 0.0 to 25.5 mA. Defaults to `0`.
- **green_current** (*Optional*, float): Set the maximum current for the green channel in mA. Range: 0.0 to 25.5 mA. Defaults to `0`.
- **blue_current** (*Optional*, float): Set the maximum current for the blue channel in mA. Range: 0.0 to 25.5 mA. Defaults to `0`.
- **white_current** (*Optional*, float): Set the maximum current for the white channel in mA. Range: 0.0 to 25.5 mA. Defaults to `0`.
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x30`.
- All other options from [I2C Component](/components/i2c#config-i2c).

## Output

The LP5562 output platform allows you to control the brightness of individual LED channels (red, green, blue, white) via PWM.

```yaml
output:
  - platform: lp5562
    id: lp5562_red_channel
    lp5562_id: lp5562_led
    channel: red
  - platform: lp5562
    id: lp5562_green_channel
    lp5562_id: lp5562_led
    channel: green
  - platform: lp5562
    id: lp5562_blue_channel
    lp5562_id: lp5562_led
    channel: blue
  - platform: lp5562
    id: lp5562_white_channel
    lp5562_id: lp5562_led
    channel: white
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this output component.
- **lp5562_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the LP5562 component. If you have only one LP5562 component, this can be omitted.
- **channel** (*Required*, enum): The channel to control. Must be one of `red`, `green`, `blue`, or `white`.
- All other options from [Float Output](https://esphome.io/components/output#float-output).

## Light

The LP5562 can be configured as an RGB, RGBW, or monochromatic (white only) light depending on which output channels you want to use.

### RGB Light

Use this configuration if you only want to control red, green, and blue channels:

```yaml
light:
  - platform: rgb
    name: "LP5562 RGB Lights"
    red: lp5562_red_channel
    green: lp5562_green_channel
    blue: lp5562_blue_channel
```

### RGBW Light

Use this configuration if you want to control all four channels including white:

```yaml
light:
  - platform: rgbw
    name: "LP5562 RGBW Lights"
    red: lp5562_red_channel
    green: lp5562_green_channel
    blue: lp5562_blue_channel
    white: lp5562_white_channel
```

### Monochromatic (White Only) Light

Use this configuration if you only want to control the white channel:

```yaml
light:
  - platform: monochromatic
    name: "LP5562 White Only Light"
    output: lp5562_white_channel
```

### Light Configuration Variables

All options from [Light](https://esphome.io/components/light#config-light) are supported.

## Full Configuration Example

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22

lp5562:
  id: lp5562_led
  use_internal_clk: true
  power_save_mode: true
  high_pwm_freq: true
  logarithmic_dimming: true
  red_current: 15.5
  green_current: 16.5
  blue_current: 22.5
  white_current: 21.5

output:
  - platform: lp5562
    id: lp5562_red_channel
    lp5562_id: lp5562_led
    channel: red
  - platform: lp5562
    id: lp5562_green_channel
    lp5562_id: lp5562_led
    channel: green
  - platform: lp5562
    id: lp5562_blue_channel
    lp5562_id: lp5562_led
    channel: blue
  - platform: lp5562
    id: lp5562_white_channel
    lp5562_id: lp5562_led
    channel: white

light:
  - platform: rgbw
    name: "LP5562 RGBW Lights"
    red: lp5562_red_channel
    green: lp5562_green_channel
    blue: lp5562_blue_channel
    white: lp5562_white_channel
```
