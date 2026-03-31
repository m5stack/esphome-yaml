# Si12T Touch Sensor

SI12T 12-Channel Capacitive Touch Sensor
The SI12T is a 12-channel capacitive touch sensor with automatic sensitivity calibration. It communicates via I2C and can detect touch intensity across three levels: low, medium, and high.

For detailed specifications, refer to the SI12T [Datasheet](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/Si12T_Datasheet_EN.pdf).

## Component/Hub

The I2C component is required to use this device.

```YAML
# Example configuration entry
si12t:
  id: si12t_hub
  auto_mode: true
  first_touch: FTT_10S
  intr_level: OUTPUT_MEDIUM_HIGH
  res_time: RES_CYCLE_5
  reset: true
  sleep_mode: true
```

### Configuration variables
- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the SI12T component.
- **auto_mode** (*Optional*, boolean): Enable automatic operation mode. When disabled, uses fast mode. Defaults to `true`.
- **first_touch** (*Optional*, enum): First touch calibration time. Defaults to `FTT_10S`.
  - `FTT_5S`: 5 seconds
  - `FTT_10S`: 10 seconds
  - `FTT_15S`: 15 seconds
  - `FTT_20S`: 20 seconds
- **intr_level** (*Optional*, enum): Output interrupt level configuration. Defaults to `OUTPUT_MEDIUM_HIGH`.
  - `OUTPUT_MEDIUM_HIGH`: Output medium and high touch levels
  - `OUTPUT_ALL`: Output all touch levels (low, medium, high)
  - `OUTPUT_MEDIUM_HIGH_ALT`: Alternative medium and high output mode
  - `OUTPUT_HIGH`: Output only high touch level
- **res_time** (*Optional*, enum): Response time in cycles. Defaults to `RES_CYCLE_5`. 
  - `RES_CYCLE_2` to `RES_CYCLE_9`: Response cycles from 2 to 9
- **reset** (*Optional*, boolean): Perform software reset on initialization. Defaults to `true`.
- **sleep_mode** (*Optional*, boolean): Enable sleep mode for power saving. Defaults to `true`.
- **address** (*Optional*, int): The I2C address of the device. Defaults to `0x68`.
- All other options from [I2C Component](https://esphome.io/components/i2c/).

## Text Sensor

The SI12T text sensor platform reports touch detection results for individual channels. Each channel can detect four states: `no touch`, `low`, `medium`, and `high` touch intensity.

```YAML
text_sensor:
  - platform: si12t
    id: touch_ch1
    si12t: si12t_hub
    channel: CH_1
    sensitivity: SENS_LEVEL_3
    extend_threshold: true
    update_interval: 50ms
  - platform: si12t
    id: touch_ch2
    si12t: si12t_hub
    channel: CH_2
    sensitivity: SENS_LEVEL_4
    extend_threshold: false
    update_interval: 50ms
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID to use for this text sensor component.
- **si12t** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the SI12T component. If you have only one SI12T component, it can be omitted.
- **channel** (*Required*, enum): The touch channel to monitor. Must be one of `CH_1` through `CH_12`.
- **sensitivity** (*Optional*, enum): Touch sensitivity level. Defaults to `SENS_LEVEL_3`.
  - `SENS_LEVEL_0` to `SENS_LEVEL_7`: Sensitivity levels from 0 (lowest) to 7 (highest)
- **extend_threshold** (*Optional*, boolean): Extend the touch detection threshold. Defaults to `true`.
- **update_interval** (*Optional*, Time): The interval to check the sensor. Defaults to `50ms`.
- All other options from Text Sensor.

Output States:

The text sensor will report one of the following states:

- **`No touch`**: No touch detected on the channel
- **`Low`**: Low intensity touch detected
- **`Medium`**: Medium intensity touch detected
- **`High`**: High intensity touch detected

## Full Configuration Example

```YAML
i2c:
  sda: GPIOXX
  scl: GPIOXX

si12t:
  id: si12t_hub
  auto_mode: true
  first_touch: FTT_10S
  intr_level: OUTPUT_ALL
  res_time: RES_CYCLE_5
  reset: true
  sleep_mode: true

text_sensor:
  - platform: si12t
    id: touch_channel_1
    si12t: si12t_hub
    channel: CH_1
    sensitivity: SENS_LEVEL_3
    extend_threshold: true
    update_interval: 50ms
    name: "Touch Channel 1"
  
  - platform: si12t
    id: touch_channel_2
    si12t: si12t_hub
    channel: CH_2
    sensitivity: SENS_LEVEL_3
    extend_threshold: true
    update_interval: 50ms
    name: "Touch Channel 2"
  
  - platform: si12t
    id: touch_channel_3
    si12t: si12t_hub
    channel: CH_3
    sensitivity: SENS_LEVEL_4
    extend_threshold: false
    update_interval: 50ms
    name: "Touch Channel 3"
```