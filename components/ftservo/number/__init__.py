import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_ID,
    CONF_STEP,
    CONF_ANGLE,
    CONF_POSITION,
    CONF_SPEED,
    CONF_TIME,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    ICON_TIMER,
    UNIT_STEPS,
    UNIT_DEGREES
)
from .. import ftservo_ns, FTSERVO_BASE_SCHEMA, CONF_FTSERVO_ID


CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["ftservo"]

CONF_SERVO_CONTROL = "servo_control"
CONF_ANGLE = "angle"
CONF_USE_RAW_ANGLE = "use_raw_angle"
CONF_ANGLE_OFFSET = "angle_offset"
ICON_GAME_PAD = "mdi:gamepad"
ICON_SPEEDOMETER = "mdi:speedometer"
ICON_ANGLE_ACUTE = "mdi:angle-acute"

ServoPositionNumber = ftservo_ns.class_("ServoPositionNumber", number.Number)
ServoAngleNumber    = ftservo_ns.class_("ServoAngleNumber",    number.Number)
ServoSpeedNumber    = ftservo_ns.class_("ServoSpeedNumber",    number.Number)
ServoTimeNumber     = ftservo_ns.class_("ServoTimeNumber",     number.Number)


POSITION_SCHEMA = number.number_schema(
    ServoPositionNumber,
    icon=ICON_GAME_PAD,
    unit_of_measurement=UNIT_STEPS,
).extend({
    cv.Optional(CONF_MIN_VALUE, default=0.0):   cv.float_,
    cv.Optional(CONF_MAX_VALUE, default=1024.0): cv.float_,
    cv.Optional(CONF_STEP,      default=20.0):   cv.positive_float,
})

def validate_angle_config(config):
    if not config[CONF_USE_RAW_ANGLE] and CONF_ANGLE_OFFSET not in config:
        raise cv.Invalid(
            f"When '{CONF_USE_RAW_ANGLE}' is false, '{CONF_ANGLE_OFFSET}' must be provided."
        )
    return config

ANGLE_SCHEMA = cv.All(
    number.number_schema(
        ServoAngleNumber,
        icon=ICON_ANGLE_ACUTE,
        unit_of_measurement=UNIT_DEGREES
    ).extend({
        cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
        cv.Optional(CONF_MAX_VALUE, default=360.0): cv.float_,
        cv.Optional(CONF_STEP, default=10.0): cv.positive_float,
        cv.Optional(CONF_USE_RAW_ANGLE, default=True): cv.boolean,
        cv.Optional(CONF_ANGLE_OFFSET): cv.float_,
    }),
    validate_angle_config,
)


def validate_speed_schema(config):
    if config[CONF_MIN_VALUE] <= 0:
        raise cv.Invalid("speed min_value must be greater than 0")
    if config[CONF_MAX_VALUE] <= config[CONF_MIN_VALUE]:
        raise cv.Invalid("speed max_value must be greater than min_value")
    return config

SPEED_SCHEMA = cv.All(
    number.number_schema(
        ServoSpeedNumber,
        icon=ICON_SPEEDOMETER,
        unit_of_measurement="steps/s",
    ).extend({
        cv.Optional(CONF_MIN_VALUE, default=0.0):   cv.float_,
        cv.Optional(CONF_MAX_VALUE, default=1500.0): cv.float_,
        cv.Optional(CONF_STEP,      default=500.0):  cv.positive_float,
    }),
    validate_speed_schema,
)

TIME_SCHEMA = number.number_schema(
    ServoTimeNumber,
    icon=ICON_TIMER,
    unit_of_measurement="ms",
).extend({
    cv.Optional(CONF_MIN_VALUE, default=0.0):    cv.positive_float,
    cv.Optional(CONF_MAX_VALUE, default=5000.0): cv.float_,
    cv.Optional(CONF_STEP,      default=200.0):  cv.positive_float,
})

def validate_servo_control(config):
    if CONF_POSITION not in config and CONF_ANGLE not in config:
        raise cv.Invalid("One of 'position' or 'angle' must be specified")
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.Exclusive(CONF_POSITION, "target_control"): POSITION_SCHEMA,
        cv.Exclusive(CONF_ANGLE, "target_control"): ANGLE_SCHEMA,
        cv.Optional(CONF_SPEED): SPEED_SCHEMA,
        cv.Optional(CONF_TIME): TIME_SCHEMA,
    })
    .extend(FTSERVO_BASE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA),
    validate_servo_control,
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_FTSERVO_ID])
    if CONF_POSITION in config:
        pos_cfg = config[CONF_POSITION]
        pos_var = await number.new_number(
            pos_cfg,
            min_value=pos_cfg[CONF_MIN_VALUE],
            max_value=pos_cfg[CONF_MAX_VALUE],
            step=pos_cfg[CONF_STEP],
        )
        await cg.register_parented(pos_var, parent)
        cg.add(parent.set_position_number(pos_var))

    if CONF_ANGLE in config:
        angle_cfg = config[CONF_ANGLE]
        angle_var = await number.new_number(
            angle_cfg,
            min_value=angle_cfg[CONF_MIN_VALUE],
            max_value=angle_cfg[CONF_MAX_VALUE],
            step=angle_cfg[CONF_STEP],
        )
        await cg.register_parented(angle_var, parent)
        cg.add(parent.set_angle_number(angle_var))
        cg.add(angle_var.set_use_raw_angle(angle_cfg[CONF_USE_RAW_ANGLE]))
        cg.add(angle_var.set_angle_offset(angle_cfg[CONF_ANGLE_OFFSET]))

    if CONF_SPEED in config:
        spd_cfg = config[CONF_SPEED]
        spd_var = await number.new_number(
            spd_cfg,
            min_value=spd_cfg[CONF_MIN_VALUE],
            max_value=spd_cfg[CONF_MAX_VALUE],
            step=spd_cfg[CONF_STEP],
        )
        await cg.register_parented(spd_var, parent)
        cg.add(parent.set_speed_number(spd_var))
    if CONF_TIME in config:
        time_cfg = config[CONF_TIME]
        time_var = await number.new_number(
            time_cfg,
            min_value=time_cfg[CONF_MIN_VALUE],
            max_value=time_cfg[CONF_MAX_VALUE],
            step=time_cfg[CONF_STEP],
        )
        await cg.register_parented(time_var, parent)
        cg.add(parent.set_time_number(time_var))