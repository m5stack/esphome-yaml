import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_CHANNEL
from .. import lp5562_ns, CONF_LP5562_ID, BASE_SCHEMA

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["lp5562"]



LP5562Output = lp5562_ns.class_("LP5562Output", output.FloatOutput)
RGBWChannel = lp5562_ns.enum("Channel", is_class=True)

CHANNEL_MAP = {
    "red": RGBWChannel.CHANNEL_R,
    "green": RGBWChannel.CHANNEL_G,
    "blue": RGBWChannel.CHANNEL_B,
    "white": RGBWChannel.CHANNEL_W,
}

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(LP5562Output),
        cv.Required(CONF_CHANNEL): cv.enum(CHANNEL_MAP),
    }
).extend(BASE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_LP5562_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    await output.register_output(var, config)