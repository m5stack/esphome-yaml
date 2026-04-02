from ... import ftservo
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_ADDRESS
from .. import scs9009_ns, BASE_SCHEMA, CONF_SCS9009_ID

SCS9009Servo = scs9009_ns.class_("SCS9009Servo", ftservo.FTServo, cg.Component)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SCS9009Servo),
            cv.Required(CONF_ADDRESS): cv.uint8_t
        }
    )
    .extend(BASE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_SCS9009_ID])
    await cg.register_component(var, config)
    await ftservo.register_ftservo(var, config)

    cg.add(var.set_address(config[CONF_ADDRESS]))
