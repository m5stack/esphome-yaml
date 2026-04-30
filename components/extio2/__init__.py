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
    CONF_OPEN_DRAIN,
    CONF_RESET,
)

AUTO_LOAD = ["gpio_expander"]
CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

extio2_ns = cg.esphome_ns.namespace("extio2")
EXTIO2Component = extio2_ns.class_(
    "EXTIO2Component", cg.Component, i2c.I2CDevice
)
EXTIO2Pin = extio2_ns.class_("EXTIO2Pin", cg.GPIOPin)

EXTIO2PWMFrequency = extio2_ns.enum("EXTIO2PWMFrequency")

PWM_FREQUENCY = {
    "2kHz" : EXTIO2PWMFrequency.FREQ_2KHZ,
    "1kHz" : EXTIO2PWMFrequency.FREQ_1KHZ,
    "500Hz" : EXTIO2PWMFrequency.FREQ_500HZ,
    "250Hz" : EXTIO2PWMFrequency.FREQ_250HZ,
    "125Hz" : EXTIO2PWMFrequency.FREQ_125HZ
}

CONF_EXTIO2_ID = "extio2_id"

CONF_PWM_FREQ = "pwm_freq"


BASE_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_EXTIO2_ID): cv.use_id(EXTIO2Component),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(EXTIO2Component),
            cv.Optional(CONF_RESET, default=True): cv.boolean,
            cv.Optional(CONF_PWM_FREQ, default="1kHz") : cv.enum(PWM_FREQUENCY),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x45))
)


def validate_mode(value):
    if not (value[CONF_INPUT] or value[CONF_OUTPUT]):
        raise cv.Invalid("Mode must be either input or output")
    if value[CONF_INPUT] and value[CONF_OUTPUT]:
        raise cv.Invalid("Mode must be either input or output")
    return value


EXTIO2_PIN_SCHEMA = pins.gpio_base_schema(
    EXTIO2Pin,
    cv.int_range(min=0, max=7),
    modes=[
        CONF_INPUT,
        CONF_OUTPUT,
        CONF_PULLUP,
        CONF_PULLDOWN,
        CONF_OPEN_DRAIN,
    ],
    mode_validator=validate_mode,
).extend(BASE_SCHEMA)


@pins.PIN_SCHEMA_REGISTRY.register(CONF_EXTIO2_ID, EXTIO2_PIN_SCHEMA)
async def extio2_pin_schema(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_EXTIO2_ID])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_reset(config[CONF_RESET]))
    cg.add(var.set_pwm_freq(config[CONF_PWM_FREQ]))
