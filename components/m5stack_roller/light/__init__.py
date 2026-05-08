import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_ID, CONF_OUTPUT_ID
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = ["m5stack_roller"]
CODEOWNERS = ["@ItsRebaseTime"]

CONF_RGB = "rgb"

RollerRgbLight = m5stack_roller_ns.class_("RollerRgbLight", light.LightOutput)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_RGB): light.RGB_LIGHT_SCHEMA.extend(
            {cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(RollerRgbLight)}
        ),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_RGB in config:
        output = cg.new_Pvariable(config[CONF_RGB][CONF_OUTPUT_ID])
        cg.add(output.set_parent(parent))
        await light.register_light(output, config[CONF_RGB])
