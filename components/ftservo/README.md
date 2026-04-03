# FTServo Serial Bus Servo Controller

> ⚠️ **Work In Progress**
>
> This component is under active development. Only basic functionality is currently implemented. Configuration options, APIs, and behaviors are subject to change without notice.
> If you have better ideas/suggestions in servo design, feel free to let us know!

The `ftservo` component provides a platform interface for FT (Feetech) serial bus servos in ESPHome. It abstracts servo communication into a reusable hub that sub-components — sensor, number, and switch — can consume to expose position control, telemetry, and torque management.

## Component/Hub

The `ftservo` namespace acts as a **platform component**. It does not declare hardware directly; instead, it provides a base class (`FTServo`) that a concrete bus implementation must extend (e.g., via UART). The hub exposes shared automation triggers and conditions.

### Automation (Work in progress)

#### Triggers

- **on_move**: Fired when the servo begins a move command.
- **on_error** (`x` → `string`): Fired when the servo reports an error. The variable `x` contains the error description.

```yaml
ftservo:
  on_move:
    - logger.log: "Servo is moving"
  on_error:
    - logger.log:
        format: "Servo error: %s"
        args: [x]
```

#### Actions

- **ftservo.move**: Command the servo to move to a target position.

```yaml
on_...:
  - ftservo.move:
      position: 512
      speed: 1000     # optional, steps per second, default 1000
      time: 0         # optional, duration in ms (0 = use speed), default 0
```

##### Action Configuration variables

- **position** (*Required*, int, [templatable](https://esphome.io/guides/automations#config-templatable)): Target position in steps.
- **speed** (*Optional*, int, [templatable](https://esphome.io/guides/automations#config-templatable)): Movement speed in steps per second. Defaults to `1000`.
- **time** (*Optional*, int, [templatable](https://esphome.io/guides/automations#config-templatable)): Movement duration in milliseconds. When set to a non-zero value, the servo reaches the target within the given time regardless of speed. Defaults to `0`.

#### Conditions

- **ftservo.is_moving**: Returns `true` if the servo is currently executing a move command.

```yaml
on_...:
  - if:
      condition:
        ftservo.is_moving:
      then:
        - logger.log: "Servo is busy"
```

## Number

The `number` sub-component provides UI-controllable number entities for servo control. It supports multiple control modes: position (raw steps), angle (degrees), speed, and time parameters.

```yaml
number:
  - platform: ftservo
    ftservo_id: my_servo
    position:
      name: "Servo Position"
      min_value: 0
      max_value: 1024
      step: 20
    speed:
      name: "Servo Speed"
      min_value: 100
      max_value: 1500
      step: 100
    time:
      name: "Movement Time"
      min_value: 0
      max_value: 5000
      step: 200
```

### Configuration variables

- **ftservo_id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the `FTServo` component.
- **position** (*Optional*, exclusive with `angle`): Position control in raw steps. When changed, the servo moves to the target position and releases torque after completion.
  - **min_value** (*Optional*, float): Minimum position. Defaults to `0`.
  - **max_value** (*Optional*, float): Maximum position. Defaults to `1024`.
  - **step** (*Optional*, float): Step size. Defaults to `20`.
  - Unit: `steps`
- **angle** (*Optional*, exclusive with `position`): Angle control in degrees. When changed, the servo moves to the target angle and releases torque after completion.
  - **min_value** (*Optional*, float): Minimum angle. Defaults to `0`.
  - **max_value** (*Optional*, float): Maximum angle. Defaults to `360`.
  - **step** (*Optional*, float): Step size. Defaults to `10`.
  - **use_raw_angle** (*Optional*, boolean): If `true`, uses the angle value directly. If `false`, applies `angle_offset`. Defaults to `true`.
  - **angle_offset** (*Required when `use_raw_angle` is `false`*, float): Offset to add to the angle value before sending to servo.
  - Unit: `°`
- **speed** (*Optional*): Movement speed control. This value is used by position/angle movements.
  - **min_value** (*Optional*, float): Minimum speed. Must be greater than `0`. Defaults to `0`.
  - **max_value** (*Optional*, float): Maximum speed. Defaults to `1500`.
  - **step** (*Optional*, float): Step size. Defaults to `500`.
  - Unit: `steps/s`
- **time** (*Optional*): Movement time control. When set to non-zero, the servo reaches the target within the specified duration regardless of speed.
  - **min_value** (*Optional*, float): Minimum time. Defaults to `0`.
  - **max_value** (*Optional*, float): Maximum time. Defaults to `5000`.
  - **step** (*Optional*, float): Step size. Defaults to `200`.
  - Unit: `ms`
- All other options from [Number](https://esphome.io/components/number#config-number).

> **Note**: Either `position` or `angle` must be specified, but not both. The `speed` and `time` parameters are optional and control how movements are executed.

Alternative: use angle control instead of position

```yaml
number:
  - platform: ftservo
    ftservo_id: my_servo
    angle:
      name: "Servo Angle"
      min_value: -90
      max_value: 90
      step: 10
      use_raw_angle: false
      angle_offset: 90.0
```

Internally the angle data were converted to position, the default raw angle for a servo is `[0, 360]`, corresponding to `[0, 1024]` in steps. When you are not using the raw angle, remind that you should normalize your angle with corresponding offset, the converted new range is `[ min_value + angle_offset, max_value + angle_offset ]`

## Sensor

The `sensor` sub-component polls servo telemetry at a configurable interval. Each telemetry field is an independent optional sub-sensor.

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
      accuracy_decimals: 1
    current:
      name: "Servo Current"
    temperature:
      name: "Servo Temperature"
```

### Configuration variables

- **ftservo_id** (*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the `FTServo` component.
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): Polling interval for all sub-sensors. Defaults to `200ms`.
- **position** (*Optional*): Current servo position.
  - Unit: `steps`
- **speed** (*Optional*): Current servo movement speed.
  - Unit: `steps/s`
- **load** (*Optional*): Current servo torque load.
  - Unit: `%`
- **voltage** (*Optional*): Supply voltage measured at the servo. Reported in 0.1 V increments internally.
  - Unit: `V`
- **current** (*Optional*): Current draw of the servo.
  - Unit: `A`
- **temperature** (*Optional*): Internal servo temperature.
  - Unit: `°C`
- All other options from [Sensor](https://esphome.io/components/sensor#config-sensor).

> **Note**: Sub-sensors that are not supported by a particular servo model will publish `NaN` and set a component warning.

## Switch

The `switch` sub-component controls servo torque enable/disable. When torque is disabled, the servo shaft can be rotated freely by hand.

```yaml
switch:
  - platform: ftservo
    ftservo_id: my_servo
    name: "Servo Torque"
```

- **ftservo_id**(*Required*, [ID](https://esphome.io/guides/configuration-types#id)): The ID of the FTServo component.

- All other options from [Switch](https://esphome.io/components/switch#config-switch).

## Full Configuration Example

```yaml
# Assumption: a concrete ftservo UART implementation is registered separately
# as "my_servo" before the sub-components are declared.

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

number:
  - platform: ftservo
    ftservo_id: my_servo
    name: "Servo Control"
    min_value: 0
    max_value: 1024
    step: 20

switch:
  - platform: ftservo
    ftservo_id: my_servo
    name: "Servo Torque"

# Automation example
on_boot:
  - ftservo.move:
      position: 512
      speed: 500
  - wait_until:
      condition:
        not:
          ftservo.is_moving:
  - logger.log: "Servo reached center position"
```

## Error Codes

| Code | Description |
|---|---|
| `FT_OK` | No error |
| `FT_ERR_NO_REPLY` | Servo did not respond |
| `FT_ERR_CRC_CMP` | CRC checksum mismatch |
| `FT_ERR_SLAVE_ID` | Slave ID mismatch |
| `FT_ERR_BUFF_LEN` | Buffer length error |
