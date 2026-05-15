import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_CHANNEL, CONF_ID

from .. import M5UnitEncoder, m5unit_encoder_ns, CONF_M5UNIT_ENCODER_ID

CODEOWNERS = ["@m5stack"]

M5UnitEncoderLedOutput = m5unit_encoder_ns.class_(
    "M5UnitEncoderLedOutput", output.FloatOutput
)

LedColorChannel = m5unit_encoder_ns.enum("LedColorChannel", is_class=True)

CHANNEL_MAP = {
    "red": LedColorChannel.LED_RED,
    "green": LedColorChannel.LED_GREEN,
    "blue": LedColorChannel.LED_BLUE,
}

CONF_LED_INDEX = "led_index"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(M5UnitEncoderLedOutput),
        cv.GenerateID(CONF_M5UNIT_ENCODER_ID): cv.use_id(M5UnitEncoder),
        cv.Required(CONF_LED_INDEX): cv.int_range(min=0, max=1),
        cv.Required(CONF_CHANNEL): cv.enum(CHANNEL_MAP),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)

    parent = await cg.get_variable(config[CONF_M5UNIT_ENCODER_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_led_index(config[CONF_LED_INDEX]))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
