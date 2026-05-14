# M5Stack Module13.2 Dual Kmeter Temperature Sensor

The `dual_kmeter` sensor platform allows you to use the
[M5Stack Module13.2 Dual Kmeter](https://docs.m5stack.com/en/module/DualKmeter%20Module13.2)
dual-channel K-Type thermocouple temperature module with ESPHome.
The module exposes two independent thermocouple inputs (Kmeter 1 and Kmeter 2)
as well as an on-board internal temperature sensor, all accessible over a single
I²C bus.

The [I²C Bus](/components/i2c) is required to be set up in your configuration for
this sensor to work.

```yaml
# Example configuration entry
i2c:
  sda: GPIO21
  scl: GPIO22

sensor:
  - platform: dual_kmeter
    name: "Thermocouple Channel 1"
    channel: KMETER_1
    update_interval: 60s
```

## Configuration Variables

- **channel** (**Required**): The measurement channel to read. One of:

  | Value | Description |
  |-------|-------------|
  | `KMETER_1` | K-Type thermocouple on input 1 |
  | `KMETER_2` | K-Type thermocouple on input 2 |
  | `INTERNAL` | On-board internal temperature sensor |

- **unit** (*Optional*): The temperature unit reported by the sensor. One of
  `CELSIUS` or `FAHRENHEIT`. Defaults to `CELSIUS`.

- **update_interval** (*Optional*, [Time](/guides/configuration-types#time)):
  The interval at which the sensor is polled. Defaults to `60s`.

- **address** (*Optional*, int): The I²C address of the module.
  Defaults to `0x11`. See [I²C Address](#i2c-address).

- All other options from [Sensor](/components/sensor).

## Measurement Behavior

### Thermocouple channels (`KMETER_1` / `KMETER_2`)

When a thermocouple channel is selected, the component writes the channel
selection register (`0x20`) on every `update()` cycle, then polls the
ready-flag register (`0x30`) until the module signals that a fresh conversion
is available. The component waits up to **300 ms**; if the flag is not
asserted within that window the update is skipped and a warning is logged:

```
[W][dual_kmeter]: Kmeter sampling timeout
```

Temperature values are stored in the module firmware as signed 32-bit integers
in little-endian byte order, scaled by a factor of **0.01** (i.e. a register
value of `2537` represents 25.37 °C).

### Internal channel (`INTERNAL`)

The internal channel reads the module's on-board thermistor directly without
waiting for a ready flag. This measurement is always available and is not
affected by the thermocouple channel selection register.

## I²C Address

The M5Stack Module Dual Kmeter uses a fixed I²C address of **`0x11`** by
default. If the hardware address has been changed via the module's [DIP switch](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1028/M127-Module-DualKmeter-I2C-Protocol.pdf),
specify the new address with the `address` option:

```yaml
sensor:
  - platform: dual_kmeter
    name: "Kiln Temperature"
    channel: KMETER_2
    address: 0x12        # non-default address
```

> Note: To ensure a successful firmware upload to the M5Stack Core series, 
> please detach the host from the module before starting. 
> There is a known issue with uploading when they are stacked. 
> Once the upload is complete, reset the host before reassembling the module for use.

## Full Configuration Example

```yaml
i2c:
  sda: GPIO21
  scl: GPIO22

sensor:
  # --- Thermocouple channel 1, Celsius ---
  - platform: dual_kmeter
    name: "Kiln Thermocouple"
    channel: KMETER_1
    unit: CELSIUS
    update_interval: 30s
    filters:
      - sliding_window_moving_average:
          window_size: 5
          send_every: 5

  # --- Thermocouple channel 2, Fahrenheit ---
  - platform: dual_kmeter
    name: "Oven Thermocouple (°F)"
    channel: KMETER_2
    unit: FAHRENHEIT
    # for unit consistency, default 'unit_of_measurement' is celsius
    unit_of_measurement: "°F"  
    update_interval: 30s

  # --- On-board internal temperature ---
  - platform: dual_kmeter
    name: "Module Internal Temperature"
    channel: INTERNAL
    update_interval: 60s
```


## See Also

- [Sensor Filters](/components/sensor#sensor-filters)
- [I²C Bus](/components/i2c)
- [M5Stack Module13.2 Dual Kmeter product page](https://docs.m5stack.com/en/module/DualKmeter%20Module13.2)
- [M5Stack Module13.2 Dual Kmeter Protocol](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1028/M127-Module-DualKmeter-I2C-Protocol.pdf)