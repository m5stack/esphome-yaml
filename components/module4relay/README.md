# M5Stack Module 4Relay

<img
  src="https://static-cdn.m5stack.com/resource/docs/products/module/4relay/4relay_01.webp"
  width="400"
/>

Module 4Relay is a 4-channel relay module controlled over I2C.
This ESPHome component mirrors the existing `unit4relay` integration style,
but uses the Module 4Relay register behavior.

## Component/Hub

The [I2C](https://esphome.io/components/i2c/) component is required.

```yaml
module4relay:
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for Module 4 Relay component.

## Switch

Switch entities let you control each relay channel.

```yaml
switch:
  - platform: module4relay
    relay_1:
      name: "Module Relay Channel 1"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_2:
      name: "Module Relay Channel 2"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_3:
      name: "Module Relay Channel 3"
      restore_mode: RESTORE_DEFAULT_OFF
    relay_4:
      name: "Module Relay Channel 4"
      restore_mode: RESTORE_DEFAULT_OFF
```

### Configuration variables

- **relay_1** (*Optional*): Relay switch for channel 1.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_2** (*Optional*): Relay switch for channel 2.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_3** (*Optional*): Relay switch for channel 3.
  All options from [Switch](https://esphome.io/components/switch#config-switch)

- **relay_4** (*Optional*): Relay switch for channel 4.
  All options from [Switch](https://esphome.io/components/switch#config-switch)
