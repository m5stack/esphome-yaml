import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_STEP,
    UNIT_PERCENT,
)
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = ["m5stack_roller"]
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

def _number_schema(cls, unit, min_val, max_val, step=1.0, icon=None):
    kwargs = {
        CONF_MIN_VALUE: min_val,
        CONF_MAX_VALUE: max_val,
        CONF_STEP: step,
    }
    if unit:
        kwargs["unit_of_measurement"] = unit
    if icon:
        kwargs["icon"] = icon
    return number.number_schema(cls).extend(cv.COMPONENT_SCHEMA)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_SPEED): number.number_schema(RollerSpeedNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_SPEED_MAX_CURRENT): number.number_schema(RollerSpeedMaxCurrentNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POSITION): number.number_schema(RollerPositionNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POSITION_MAX_CURRENT): number.number_schema(RollerPositionMaxCurrentNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_CURRENT): number.number_schema(RollerCurrentNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_SPEED_KP): number.number_schema(RollerSpeedKpNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_SPEED_KI): number.number_schema(RollerSpeedKiNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_SPEED_KD): number.number_schema(RollerSpeedKdNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POSITION_KP): number.number_schema(RollerPositionKpNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POSITION_KI): number.number_schema(RollerPositionKiNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_POSITION_KD): number.number_schema(RollerPositionKdNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RGB_BRIGHTNESS): number.number_schema(RollerRgbBrightnessNumber).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_ENCODER): number.number_schema(RollerEncoderNumber).extend(cv.COMPONENT_SCHEMA),
    }
).extend(ROLLER_BASE_SCHEMA)


async def _register_number(config, key, set_fn, parent):
    if key not in config:
        return
    n = cg.new_Pvariable(config[key][CONF_ID])
    await cg.register_component(n, config[key])
    await number.register_number(n, config[key],
                                 min_value=config[key].get(CONF_MIN_VALUE, -2100000),
                                 max_value=config[key].get(CONF_MAX_VALUE, 2100000),
                                 step=config[key].get(CONF_STEP, 1))
    cg.add(n.set_parent(parent))


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    for key, _ in [
        (CONF_SPEED, None),
        (CONF_SPEED_MAX_CURRENT, None),
        (CONF_POSITION, None),
        (CONF_POSITION_MAX_CURRENT, None),
        (CONF_CURRENT, None),
        (CONF_SPEED_KP, None),
        (CONF_SPEED_KI, None),
        (CONF_SPEED_KD, None),
        (CONF_POSITION_KP, None),
        (CONF_POSITION_KI, None),
        (CONF_POSITION_KD, None),
        (CONF_RGB_BRIGHTNESS, None),
        (CONF_ENCODER, None),
    ]:
        if key not in config:
            continue
        n = cg.new_Pvariable(config[key][CONF_ID])
        await cg.register_component(n, config[key])
        min_v = config[key].get(CONF_MIN_VALUE, -2100000.0)
        max_v = config[key].get(CONF_MAX_VALUE, 2100000.0)
        step_v = config[key].get(CONF_STEP, 1.0)
        await number.register_number(n, config[key],
                                     min_value=min_v,
                                     max_value=max_v,
                                     step=step_v)
        cg.add(n.set_parent(parent))
