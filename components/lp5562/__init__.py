import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID


DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]

CONF_LP5562_ID = "lp5562_id"

CONF_USE_INTERNAL_CLK = "use_internal_clk"
CONF_POWER_SAVE_MODE = "power_save_mode"
CONF_HIGH_PWM_FREQ = "high_pwm_freq"
CONF_LOGARITHMIC_DIMMING = "logarithmic_dimming"

CONF_RED_CURRENT = "red_current"
CONF_GREEN_CURRENT = "green_current"
CONF_BLUE_CURRENT = "blue_current"
CONF_WHITE_CURRENT = "white_current"

lp5562_ns = cg.esphome_ns.namespace("lp5562")
LP5562Component = lp5562_ns.class_("LP5562Component", cg.Component, i2c.I2CDevice)


BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_LP5562_ID): cv.use_id(LP5562Component),
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LP5562Component),
            cv.Optional(CONF_USE_INTERNAL_CLK, default=False) : cv.boolean,
            cv.Optional(CONF_POWER_SAVE_MODE, default=False) : cv.boolean,
            cv.Optional(CONF_HIGH_PWM_FREQ, default=False) : cv.boolean,
            cv.Optional(CONF_LOGARITHMIC_DIMMING, default=False) : cv.boolean,
            cv.Optional(CONF_RED_CURRENT, default=0) : cv.float_range(min=0.0, max=25.5),
            cv.Optional(CONF_GREEN_CURRENT, default=0) : cv.float_range(min=0.0, max=25.5),
            cv.Optional(CONF_BLUE_CURRENT, default=0) : cv.float_range(min=0.0, max=25.5),
            cv.Optional(CONF_WHITE_CURRENT, default=0) : cv.float_range(min=0.0, max=25.5),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x30)) 
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_use_internal_clk(config[CONF_USE_INTERNAL_CLK]))
    cg.add(var.set_power_save_mode(config[CONF_POWER_SAVE_MODE]))
    cg.add(var.set_high_pwm_freq(config[CONF_HIGH_PWM_FREQ]))
    cg.add(var.set_logarithmic_dimming(config[CONF_LOGARITHMIC_DIMMING]))
    cg.add(var.set_red_current(config[CONF_RED_CURRENT]))
    cg.add(var.set_green_current(config[CONF_GREEN_CURRENT]))
    cg.add(var.set_blue_current(config[CONF_BLUE_CURRENT]))
    cg.add(var.set_white_current(config[CONF_WHITE_CURRENT]))
