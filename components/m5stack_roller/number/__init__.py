import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import CONF_ID, UNIT_PERCENT
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = []
CODEOWNERS = ["@ItsRebaseTime"]

CONF_SPEED = "speed"
CONF_SPEED_MAX_CURRENT = "speed_max_current"
CONF_POSITION = "position"
CONF_POSITION_MAX_CURRENT = "position_max_current"
CONF_CURRENT = "current"
CONF_SPEED_KP = "speed_kp"
CONF_SPEED_KI = "speed_ki"
CONF_SPEED_KD = "speed_kd"
CONF_POSITION_KP = "position_kp"
CONF_POSITION_KI = "position_ki"
CONF_POSITION_KD = "position_kd"
CONF_RGB_BRIGHTNESS = "rgb_brightness"
CONF_ENCODER = "encoder"

UNIT_RPM = "rpm"
UNIT_DEGREE = "°"
UNIT_MILLIAMPERE = "mA"

RollerSpeedNumber = m5stack_roller_ns.class_("RollerSpeedNumber", number.Number, cg.Component)
RollerSpeedMaxCurrentNumber = m5stack_roller_ns.class_("RollerSpeedMaxCurrentNumber", number.Number, cg.Component)
RollerPositionNumber = m5stack_roller_ns.class_("RollerPositionNumber", number.Number, cg.Component)
RollerPositionMaxCurrentNumber = m5stack_roller_ns.class_("RollerPositionMaxCurrentNumber", number.Number, cg.Component)
RollerCurrentNumber = m5stack_roller_ns.class_("RollerCurrentNumber", number.Number, cg.Component)
RollerSpeedKpNumber = m5stack_roller_ns.class_("RollerSpeedKpNumber", number.Number, cg.Component)
RollerSpeedKiNumber = m5stack_roller_ns.class_("RollerSpeedKiNumber", number.Number, cg.Component)
RollerSpeedKdNumber = m5stack_roller_ns.class_("RollerSpeedKdNumber", number.Number, cg.Component)
RollerPositionKpNumber = m5stack_roller_ns.class_("RollerPositionKpNumber", number.Number, cg.Component)
RollerPositionKiNumber = m5stack_roller_ns.class_("RollerPositionKiNumber", number.Number, cg.Component)
RollerPositionKdNumber = m5stack_roller_ns.class_("RollerPositionKdNumber", number.Number, cg.Component)
RollerRgbBrightnessNumber = m5stack_roller_ns.class_("RollerRgbBrightnessNumber", number.Number, cg.Component)
RollerEncoderNumber = m5stack_roller_ns.class_("RollerEncoderNumber", number.Number, cg.Component)

# (conf_key, class, min_value, max_value, step, setter_method)
_NUMBER_ENTRIES = [
    (CONF_SPEED,              RollerSpeedNumber,              -21000,       21000,       1,        "set_speed_number"),
    (CONF_SPEED_MAX_CURRENT,  RollerSpeedMaxCurrentNumber,     0,            1200,        10,       "set_speed_max_current_number"),
    (CONF_POSITION,           RollerPositionNumber,           -21000000,    21000000,    1,        "set_position_number"),
    (CONF_POSITION_MAX_CURRENT, RollerPositionMaxCurrentNumber, 0,           1200,        10,       "set_position_max_current_number"),
    (CONF_CURRENT,            RollerCurrentNumber,            -1200,         1200,        10,       "set_current_number"),
    (CONF_SPEED_KP,           RollerSpeedKpNumber,             0,            100,         0.00001,  "set_speed_kp_number"),
    (CONF_SPEED_KI,           RollerSpeedKiNumber,             0,            100,         0.0000001,"set_speed_ki_number"),
    (CONF_SPEED_KD,           RollerSpeedKdNumber,             0,            100,         0.00001,  "set_speed_kd_number"),
    (CONF_POSITION_KP,        RollerPositionKpNumber,          0,            100,         0.00001,  "set_position_kp_number"),
    (CONF_POSITION_KI,        RollerPositionKiNumber,          0,            100,         0.0000001,"set_position_ki_number"),
    (CONF_POSITION_KD,        RollerPositionKdNumber,          0,            100,         0.00001,  "set_position_kd_number"),
    (CONF_RGB_BRIGHTNESS,     RollerRgbBrightnessNumber,       0,            100,         1,        "set_rgb_brightness_number"),
    (CONF_ENCODER,            RollerEncoderNumber,            -2147483648,   2147483647,  1,        "set_encoder_number"),
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        **{
            cv.Optional(key): number.number_schema(cls).extend(cv.COMPONENT_SCHEMA)
            for key, cls, *_ in _NUMBER_ENTRIES
        },
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    for key, cls, mn, mx, st, setter in _NUMBER_ENTRIES:
        if key not in config:
            continue
        n = cg.new_Pvariable(config[key][CONF_ID])
        await cg.register_component(n, config[key])
        await number.register_number(n, config[key], min_value=mn, max_value=mx, step=st)
        cg.add(n.set_parent(parent))
