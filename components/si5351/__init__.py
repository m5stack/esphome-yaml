import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_RESET_PIN
from esphome import pins

CODEOWNERS = ["@Vincent"]
DEPENDENCIES = ["i2c"]

si5351_ns = cg.esphome_ns.namespace("si5351")
Si5351Component = si5351_ns.class_(
    "Si5351Component", cg.Component, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Si5351Component),
            cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x60))  # Si5351 default address
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_RESET_PIN in config:
        reset = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))