# SCS9009 Serial Bus Servo (FTServo Platform Implementation)

> ⚠️ **Work In Progress**
>
> This component is under active development. Only basic functionality is currently implemented. Configuration options, APIs, and behaviors are subject to change without notice.
> Any better implementation designs of UART components are welcome! 

The `scs9009` component implements the [FTServo](../ftservo/README.md) platform for Feetech SCS9009 serial bus servos. It communicates over a half-duplex UART bus using the SCS protocol and exposes each physical servo as an individually addressable `FTServo` instance.

The `scs9009` component itself acts as a **bus controller** — one component manages the UART bus and one or more `servo` sub-devices are declared under it, each addressed by a unique servo ID.

## Dependencies

- [`uart`](https://esphome.io/components/uart)
- [`ftservo`](../ftservo/README.md) (platform base)

## Bus Controller

The bus controller manages the UART connection to the SCS9009 servo bus. All servos on the same physical bus share one controller.

```yaml
uart:
  id: uart_bus
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 1000000
  stop_bits: 1
  parity: NONE
  data_bits: 8

scs9009:
  id: scs9009_bus
  uart_id: uart_bus
```

### Configuration variables

- **id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): Specify an ID for the SCS9009 bus controller. Required if you declare multiple UART devices.
- **uart_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the UART component. Required if multiple UART buses are configured.
- All options from [UART Device](https://esphome.io/components/uart#config-uart).

> **Note**: The SCS9009 servo bus requires **1 000 000 baud, 8N1**. The UART component will emit a warning at startup if these settings do not match.

> **Assumption**: The SCS9009 servo bus is half-duplex. Wiring the TX and RX lines together through a suitable resistor or bus transceiver is required depending on the hardware platform.

## Servo

Each physical servo on the bus is declared as an individual servo sub-device. A servo is identified by its **bus address** (factory default typically `1`).

```yaml
scs9009:
  id: scs9009_bus
  uart_id: uart_bus

ftservo:
  - id: my_servo
    scs9009_id: scs9009_bus
    address: 1
```

### Configuration variables

- **id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of this servo instance. Referenced by sensor, number, and switch sub-components.
- **scs9009_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the `scs9009` bus controller. Can be omitted when only one bus controller is configured.
- **address** (*Required*, int): The servo's hardware bus address. Range: 0–253. Address `0xFE` (254) is reserved for broadcast.

> **Note**: On startup the component issues an initial `PING` to the declared address. If the servo does not respond immediately, a second ping is attempted after **5 seconds**. The component will mark itself failed if the servo cannot be reached within this window.

## Sensor

Servo telemetry is provided by the [FTServo sensor platform](../ftservo/README.md#sensor). Reference the servo `id` from the `scs9009_servo` declaration.

```yaml
sensor:
  - platform: ftservo
    ftservo_id: my_servo
    update_interval: 200ms
    position:
      name: "Servo Position"
    speed:
      name: "Servo Speed"
    load:
      name: "Servo Load"
    voltage:
      name: "Servo Voltage"
    current:
      name: "Servo Current"
    temperature:
      name: "Servo Temperature"
```

All supported fields for the SCS9009:

| Sub-sensor | Unit | Notes |
|---|---|---|
| `position` | steps | Present position (0–1023 typical) |
| `speed` | steps/s | Signed; negative = reverse direction |
| `load` | % | Output torque percentage (signed, 10-bit) |
| `voltage` | V | Reported in 0.1 V increments |
| `current` | A | Signed; negative = reverse current |
| `temperature` | °C | Internal servo temperature |

## Number

Position control via UI is provided by the [FTServo number platform](../ftservo/README.md#number). After commanding a move, the component waits for the estimated travel time and then releases torque.

```yaml
number:
  - platform: ftservo
    ftservo_id: my_servo
    name: "Servo Position"
    min_value: 0
    max_value: 1024
    step: 20
```

## Switch

Torque enable/disable is provided by the [FTServo switch platform](../ftservo/README.md#switch). When torque is disabled the servo shaft can be rotated freely by hand.

```yaml
switch:
  - platform: ftservo
    ftservo_id: my_servo
    name: "Servo Torque"
```

## Memory Map

### EPROM (Read-only)

| Register | Address | Description |
|---|---|---|
| `SCSCL_VERSION_L` | 0x03 | Firmware version low byte |
| `SCSCL_VERSION_H` | 0x04 | Firmware version high byte |

### EPROM (Read / Write)

| Register | Address | Description |
|---|---|---|
| `SCSCL_ID` | 0x05 | Servo bus address |
| `SCSCL_BAUD_RATE` | 0x06 | Communication baud rate |
| `SCSCL_MIN_ANGLE_LIMIT_L` | 0x09 | Minimum angle limit low byte |
| `SCSCL_MIN_ANGLE_LIMIT_H` | 0x0A | Minimum angle limit high byte |
| `SCSCL_MAX_ANGLE_LIMIT_L` | 0x0B | Maximum angle limit low byte |
| `SCSCL_MAX_ANGLE_LIMIT_H` | 0x0C | Maximum angle limit high byte |
| `SCSCL_CW_DEAD` | 0x1A | Clockwise dead zone |
| `SCSCL_CCW_DEAD` | 0x1B | Counter-clockwise dead zone |
| `SCSCL_LOCK` | 0x30 | EPROM lock (`0` = unlocked, `1` = locked) |

### SRAM (Read / Write)

| Register | Address | Description |
|---|---|---|
| `SCSCL_TORQUE_ENABLE` | 0x28 | Torque enable (`0` = off, `1` = on) |
| `SCSCL_GOAL_POSITION_L` | 0x2A | Target position low byte |
| `SCSCL_GOAL_POSITION_H` | 0x2B | Target position high byte |
| `SCSCL_GOAL_TIME_L` | 0x2C | Target travel time low byte |
| `SCSCL_GOAL_TIME_H` | 0x2D | Target travel time high byte |
| `SCSCL_GOAL_SPEED_L` | 0x2E | Target speed low byte |
| `SCSCL_GOAL_SPEED_H` | 0x2F | Target speed high byte |

### SRAM (Read-only)

| Register | Address | Description |
|---|---|---|
| `SCSCL_PRESENT_POSITION_L` | 0x38 | Current position low byte |
| `SCSCL_PRESENT_POSITION_H` | 0x39 | Current position high byte |
| `SCSCL_PRESENT_SPEED_L` | 0x3A | Current speed low byte |
| `SCSCL_PRESENT_SPEED_H` | 0x3B | Current speed high byte |
| `SCSCL_PRESENT_LOAD_L` | 0x3C | Current load low byte |
| `SCSCL_PRESENT_LOAD_H` | 0x3D | Current load high byte |
| `SCSCL_PRESENT_VOLTAGE` | 0x3E | Current supply voltage |
| `SCSCL_PRESENT_TEMPERATURE` | 0x3F | Current temperature |
| `SCSCL_MOVING` | 0x42 | Movement status (`0` = idle, `1` = moving) |
| `SCSCL_PRESENT_CURRENT_L` | 0x45 | Current draw low byte |
| `SCSCL_PRESENT_CURRENT_H` | 0x46 | Current draw high byte |

## Protocol Instructions

| Instruction | Value | Description |
|---|---|---|
| `INST_PING` | `0x01` | Query servo presence |
| `INST_READ` | `0x02` | Read register(s) |
| `INST_WRITE` | `0x03` | Write register(s) immediately |
| `INST_REG_WRITE` | `0x04` | Stage register write (deferred) |
| `INST_REG_ACTION` | `0x05` | Execute staged write |
| `INST_SYNC_READ` | `0x82` | Simultaneous read from multiple servos |
| `INST_SYNC_WRITE` | `0x83` | Simultaneous write to multiple servos |

## Full Configuration Example

```yaml
uart:
  id: uart_bus
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 1000000
  stop_bits: 1
  parity: NONE
  data_bits: 8

scs9009:
  id: scs9009_bus
  uart_id: uart_bus

scs9009_servo:
  - id: servo_1
    scs9009_id: scs9009_bus
    address: 1
  - id: servo_2
    scs9009_id: scs9009_bus
    address: 2

sensor:
  - platform: ftservo
    ftservo_id: servo_1
    update_interval: 200ms
    position:
      name: "Servo 1 Position"
    speed:
      name: "Servo 1 Speed"
    load:
      name: "Servo 1 Load"
    voltage:
      name: "Servo 1 Voltage"
    current:
      name: "Servo 1 Current"
    temperature:
      name: "Servo 1 Temperature"

number:
  - platform: ftservo
    ftservo_id: servo_1
    name: "Servo 1 Position Control"
    min_value: 0
    max_value: 1024
    step: 20
  - platform: ftservo
    ftservo_id: servo_2
    name: "Servo 2 Position Control"
    min_value: 0
    max_value: 1024
    step: 20

switch:
  - platform: ftservo
    ftservo_id: servo_1
    name: "Servo 1 Torque"
  - platform: ftservo
    ftservo_id: servo_2
    name: "Servo 2 Torque"

# Automation example using ftservo actions
on_boot:
  - ftservo.move:
      id: servo_1
      position: 512
      speed: 500
  - wait_until:
      condition:
        not:
          ftservo.is_moving:
            id: servo_1
  - logger.log: "Servo 1 reached center position"
```


### Reference

+ [FTServo Arduino](https://github.com/ftservo/FTServo_Arduino/tree/main)