import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID

from .. import m5stack_chain_tof_ns, ChainToFSensor, CONF_M5STACK_CHAIN_TOF_ID

CONF_CHANNEL = "channel"

LedChannel = m5stack_chain_tof_ns.enum("LedChannel")

ChainToFLedOutput = m5stack_chain_tof_ns.class_(
    "ChainToFLedOutput", output.FloatOutput, cg.Component
)

CHANNEL_MAP = {
    "led_brightness": LedChannel.LED_BRIGHTNESS,
    "rgb_brightness": LedChannel.RGB_BRIGHTNESS,
    "rgb_red": LedChannel.RGB_RED,
    "rgb_green": LedChannel.RGB_GREEN,
    "rgb_blue": LedChannel.RGB_BLUE,
}

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_ID): cv.declare_id(ChainToFLedOutput),
        cv.GenerateID(CONF_M5STACK_CHAIN_TOF_ID): cv.use_id(ChainToFSensor),
        cv.Required(CONF_CHANNEL): cv.one_of(*CHANNEL_MAP, lower=True),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    parent = await cg.get_variable(config[CONF_M5STACK_CHAIN_TOF_ID])
    cg.add(var.set_parent(parent))

    channel = CHANNEL_MAP[config[CONF_CHANNEL]]
    cg.add(var.set_channel(channel))
