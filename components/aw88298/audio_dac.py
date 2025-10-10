import esphome.codegen as cg
from esphome.components import i2c
from esphome.components.audio_dac import AudioDac
import esphome.config_validation as cv
from esphome.const import CONF_BITS_PER_SAMPLE, CONF_ID, CONF_SAMPLE_RATE

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["i2c"]

aw88298_ns = cg.esphome_ns.namespace("aw88298")
AW88298 = aw88298_ns.class_("AW88298", AudioDac, cg.Component, i2c.I2CDevice)

_validate_bits = cv.float_with_unit("bits", "bit")

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AW88298),
            # cv.Optional(CONF_BITS_PER_SAMPLE, default="16bit"): cv.All(
            #     _validate_bits, cv.enum(ES8311_BITS_PER_SAMPLE_ENUM)
            # ),

            cv.Optional(CONF_SAMPLE_RATE, default=16000): cv.int_range(min=1),

        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    # cg.add(var.set_bits_per_sample(config[CONF_BITS_PER_SAMPLE]))
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
