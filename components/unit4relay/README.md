# M5Stack Unit 4 Relay

<img
  src="https://static-cdn.m5stack.com/resource/docs/products/unit/4relay/4relay_01.webp"
  width="400"
/>

Unit 4Relay is a Unit integrated with 4 relays, controlled via I2C. The relay control voltage is up to AC 250V / DC 28V, with a rated current of 10A and an instantaneous current capacity of 16A. Each relay can be controlled independently, and each relay has a programmable status indicator light.


## Component/Hub

The [I2C](https://esphome.io/components/i2c/) is required to configure the device. 

```yaml
# Example configuration entry
unit4relay:
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for Unit 4 Relay component.


## Switch

Switch allow you to control each channel individually.

<img
  src="https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/782/unit_4_relay_ha_dash.png"
  width="300"
/>

```yaml
switch:
  - platform: unit4relay
    relay_1:
      name: "Relay Channel 1"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_2:
      name: "Relay Channel 2"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_3:
      name: "Relay Channel 3"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_4:
      name: "Relay Channel 4"
      restore_mode: RESTORE_DEFAULT_OFF
```

The LED light is automatically controlled, it lights up when corresponding relay channel was enabled.

<img
  src="https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/782/unit_4_relay_pic.jpg"
  width="300"
/>

### Configuration variables

- **relay_1** (*Optional*): Relay switch for channel 1. Defaults to `false`.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_2** (*Optional*): Relay switch for channel 2. Defaults to `false`.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_3** (*Optional*): Relay switch for channel 3. Defaults to `false`.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_4** (*Optional*): Relay switch for channel 4. Defaults to `false`.
  All options from [Switch](https://esphome.io/components/switch#config-switch)



