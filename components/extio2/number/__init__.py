import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
    CONF_TYPE,
    UNIT_DEGREES,
)

from .. import extio2_ns, BASE_SCHEMA, CONF_EXTIO2_ID

DEPENDENCIES = ["extio2"]

EXTIO2AngleNumber = extio2_ns.class_("EXTIO2AngleNumber", number.Number, cg.Component)
EXTIO2PulseNumber = extio2_ns.class_("EXTIO2PulseNumber", number.Number, cg.Component)

EXTIO2ServoChannel = extio2_ns.enum("EXTIO2ServoChannel")


ICON_ANGLE = "mdi:angle-acute"

ICON_AXIS = "mdi:axis"

UNIT_MICROSECONDS = "us"

CONF_ANGLE = "angle"
CONF_PULSE = "pulse"

SERVO_CHANNELS = {
    "SERVO_0": EXTIO2ServoChannel.SERVO_0,
    "SERVO_1": EXTIO2ServoChannel.SERVO_1,
    "SERVO_2": EXTIO2ServoChannel.SERVO_2,
    "SERVO_3": EXTIO2ServoChannel.SERVO_3,
    "SERVO_4": EXTIO2ServoChannel.SERVO_4,
    "SERVO_5": EXTIO2ServoChannel.SERVO_5,
    "SERVO_6": EXTIO2ServoChannel.SERVO_6,
    "SERVO_7": EXTIO2ServoChannel.SERVO_7
}


ANGLE_SCHEMA = number.number_schema(
    EXTIO2AngleNumber,
    icon=ICON_ANGLE,
    unit_of_measurement=UNIT_DEGREES
).extend(
    {
        cv.Required(CONF_CHANNEL) : cv.enum(SERVO_CHANNELS, upper=True, space='_'),
        cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
        cv.Optional(CONF_MAX_VALUE, default=180.0): cv.float_,
        cv.Optional(CONF_STEP,      default=3.0): cv.float_
    }
).extend(BASE_SCHEMA)


PULSE_SCHEMA = number.number_schema(
    EXTIO2PulseNumber,
    icon=ICON_AXIS,
    unit_of_measurement=UNIT_MICROSECONDS
).extend(
    {
        cv.Required(CONF_CHANNEL) : cv.enum(SERVO_CHANNELS, upper=True, space='_'),
        cv.Optional(CONF_MIN_VALUE, default=500.0): cv.float_,
        cv.Optional(CONF_MAX_VALUE, default=2500.0): cv.float_,
        cv.Optional(CONF_STEP,      default=100.0): cv.float_
    }
).extend(BASE_SCHEMA)


CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_ANGLE: ANGLE_SCHEMA,
        CONF_PULSE: PULSE_SCHEMA,
    },
    key=CONF_TYPE,
    lower=True,
)


async def to_code(config):
    var = await number.new_number(
        config,
        min_value=config[CONF_MIN_VALUE],
        max_value=config[CONF_MAX_VALUE],
        step=config[CONF_STEP]
    )
    await cg.register_parented(var, config[CONF_EXTIO2_ID])
    await cg.register_component(var, config)

    cg.add(var.set_servo_channel(config[CONF_CHANNEL]))