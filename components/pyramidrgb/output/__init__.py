import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_CHANNEL
from .. import pyramidrgb_ns, CONF_PYRAMIDRGB_ID, BASE_SCHEMA

CODEOWNERS = ["@Jasionf"]
DEPENDENCIES = ["pyramidrgb"]

PyramidRGBOutput = pyramidrgb_ns.class_("PyramidRGBOutput", output.FloatOutput)
RGBColorChannel = pyramidrgb_ns.enum("RGBColorChannel", is_class=True)

COLOR_MAP = {
    "red": RGBColorChannel.COLOR_R,
    "green": RGBColorChannel.COLOR_G,
    "blue": RGBColorChannel.COLOR_B,
}

CONF_COLOR = "color"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(PyramidRGBOutput),
        cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
        cv.Required(CONF_COLOR): cv.enum(COLOR_MAP),
    }
).extend(BASE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_PYRAMIDRGB_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_color(config[CONF_COLOR]))
    await output.register_output(var, config)
