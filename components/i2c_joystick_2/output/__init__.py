import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_CHANNEL

from .. import CONF_I2C_JOYSTICK_2_ID, i2c_joystick_2_ns, I2CJoystick2Component

JoyStick2FloatOutput = i2c_joystick_2_ns.class_("JoyStick2FloatOutput", output.FloatOutput)

JoyStick2RGBChannel = i2c_joystick_2_ns.enum("RGBChannel")

RGB_CHANNEL = {
    "CHANNEL_R": JoyStick2RGBChannel.CHANNEL_R,
    "CHANNEL_G": JoyStick2RGBChannel.CHANNEL_G,
    "CHANNEL_B": JoyStick2RGBChannel.CHANNEL_B,
}


CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(JoyStick2FloatOutput),
        cv.GenerateID(CONF_I2C_JOYSTICK_2_ID): cv.use_id(I2CJoystick2Component),
        cv.Required(CONF_CHANNEL) : cv.enum(RGB_CHANNEL)
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    await cg.register_parented(var, config[CONF_I2C_JOYSTICK_2_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))