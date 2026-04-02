import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    CONF_ID,
    UNIT_STEPS,
    CONF_STEP,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE
)
from .. import ftservo_ns, FTSERVO_BASE_SCHEMA, CONF_FTSERVO_ID



CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["ftservo"]

CONF_SERVO_CONTROL = "servo_control"
ICON_GAME_PAD = "mdi:gamepad"

ServoControlNumber = ftservo_ns.class_("ServoControlNumber", number.Number, cg.Component)


CONFIG_SCHEMA = (
    number.number_schema(
        ServoControlNumber,
        icon=ICON_GAME_PAD,
        unit_of_measurement=UNIT_STEPS,
    )
    .extend({
        cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
        cv.Optional(CONF_MAX_VALUE, default=1024.0): cv.float_,
        cv.Optional(CONF_STEP, default=20.0): cv.positive_float,
    })
    .extend(FTSERVO_BASE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = await number.new_number(
            config,
            min_value=config[CONF_MIN_VALUE],
            max_value=config[CONF_MAX_VALUE],
            step=config[CONF_STEP]
        )
    
    parent = await cg.get_variable(config[CONF_FTSERVO_ID])
    await cg.register_parented(var, parent)