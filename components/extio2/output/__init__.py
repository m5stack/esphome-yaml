import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_CHANNEL

from .. import extio2_ns, BASE_SCHEMA, CONF_EXTIO2_ID

DEPENDENCIES = ["extio2"]

EXTIO2FloatOutput = extio2_ns.class_("EXTIO2FloatOutput", output.FloatOutput, cg.Component)

EXTIO2PWMChannel = extio2_ns.enum("EXTIO2PWMChannel")

PWM_CHANNELS = {
    "PWM_0" : EXTIO2PWMChannel.PWM_0,
    "PWM_1" : EXTIO2PWMChannel.PWM_1,
    "PWM_2" : EXTIO2PWMChannel.PWM_2,
    "PWM_3" : EXTIO2PWMChannel.PWM_3,
    "PWM_4" : EXTIO2PWMChannel.PWM_4,
    "PWM_5" : EXTIO2PWMChannel.PWM_5,
    "PWM_6" : EXTIO2PWMChannel.PWM_6,
    "PWM_7" : EXTIO2PWMChannel.PWM_7,
}

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(EXTIO2FloatOutput),
        cv.Required(CONF_CHANNEL): cv.enum(
            PWM_CHANNELS, upper=True, space='_'),
    }
).extend(BASE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_EXTIO2_ID])
    await output.register_output(var, config)
    await cg.register_component(var, config)
    cg.add(var.set_pwm_channel(config[CONF_CHANNEL]))
    