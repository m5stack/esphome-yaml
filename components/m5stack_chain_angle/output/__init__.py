import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_CHANNEL

from .. import m5stack_chain_angle_ns, ChainAngleSensor

CODEOWNERS = ["@Jasionf"]

ChainAngleLedOutput = m5stack_chain_angle_ns.class_(
    "ChainAngleLedOutput",
    output.FloatOutput,
)

LedChannel = m5stack_chain_angle_ns.enum("LedChannel", is_class=True)

CONF_CHAIN_ANGLE_ID = "chain_angle_id"

CHANNEL_MAP = {
    "led_brightness": LedChannel.LED_BRIGHTNESS,
    "rgb_brightness": LedChannel.RGB_BRIGHTNESS,
    "rgb_red": LedChannel.RGB_RED,
    "rgb_green": LedChannel.RGB_GREEN,
    "rgb_blue": LedChannel.RGB_BLUE,
}

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(ChainAngleLedOutput),
        cv.Required(CONF_CHANNEL): cv.enum(CHANNEL_MAP),
        cv.GenerateID(CONF_CHAIN_ANGLE_ID): cv.use_id(ChainAngleSensor),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)

    parent = await cg.get_variable(config[CONF_CHAIN_ANGLE_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_channel(CHANNEL_MAP[config[CONF_CHANNEL]]))
