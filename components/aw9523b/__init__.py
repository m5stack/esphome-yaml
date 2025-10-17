from esphome import pins
import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_INPUT,
    CONF_INVERTED,
    CONF_MODE,
    CONF_NUMBER,
    CONF_OUTPUT,
    CONF_PULLDOWN,
    CONF_PULLUP,
    CONF_RESET,
)

AUTO_LOAD = ["gpio_expander"]
CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True


aw9523b_ns = cg.esphome_ns.namespace("aw9523b")
AW9523BComponent = aw9523b_ns.class_(
    "AW9523BComponent", cg.Component, i2c.I2CDevice
)
AW9523BGPIOPin = aw9523b_ns.class_("AW9523BGPIOPin", cg.GPIOPin)

CONF_AW9523B = "aw9523b"
CONF_P0_PUSH_PULL = "p0_push_pull"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(AW9523BComponent),
            cv.Optional(CONF_RESET, default=True): cv.boolean,
            cv.Optional(CONF_P0_PUSH_PULL, default=True): cv.boolean,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x58))
)

def validate_mode(value):
    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
        raise cv.Invalid("Mode must be either input or output")
    if value[CONF_INPUT] and value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value


AW9523B_PIN_SCHEMA = pins.gpio_base_schema(
    AW9523BGPIOPin,
    cv.int_range(min=0, max=15),
    modes=[
        CONF_INPUT,
        CONF_OUTPUT,
        CONF_PULLUP,
        CONF_PULLDOWN,
    ],
    mode_validator=validate_mode,
).extend(
    {
        cv.Required(CONF_AW9523B): cv.use_id(AW9523BComponent),
    }
)

@pins.PIN_SCHEMA_REGISTRY.register(CONF_AW9523B, AW9523B_PIN_SCHEMA)
async def aw9523b_pin_schema(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AW9523B])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_p0_push_pull_mode(config[CONF_P0_PUSH_PULL]))
    cg.add(var.set_reset(config[CONF_RESET]))