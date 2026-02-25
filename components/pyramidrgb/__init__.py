import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
MULTI_CONF = True
CODEOWNERS = ["@Jasionf"]

CONF_PYRAMIDRGB_ID = "pyramidrgb_id"
CONF_STRIP = "strip"
CONF_BRIGHTNESS = "brightness"
CONF_INITIAL_WHITE = "initial_white"
CONF_LOGARITHMIC_DIMMING = "logarithmic_dimming"
CONF_GAMMA = "gamma"
CONF_USE_INTERNAL_CLK = "use_internal_clk"
CONF_POWER_SAVE_MODE = "power_save_mode"
CONF_HIGH_PWM_FREQ = "high_pwm_freq"
CONF_RED_CURRENT = "red_current"
CONF_GREEN_CURRENT = "green_current"
CONF_BLUE_CURRENT = "blue_current"
CONF_WHITE_CURRENT = "white_current"
CONF_REF_CURRENT = "ref_current"

pyramidrgb_ns = cg.esphome_ns.namespace("pyramidrgb")
PyramidRGBComponent = pyramidrgb_ns.class_("PyramidRGBComponent", cg.Component, i2c.I2CDevice)

BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_PYRAMIDRGB_ID): cv.use_id(PyramidRGBComponent),
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PyramidRGBComponent),
            cv.Optional(CONF_STRIP, default=1): cv.int_range(min=1, max=2),
            cv.Optional(CONF_BRIGHTNESS, default=100): cv.int_range(min=0, max=100),
            cv.Optional(CONF_INITIAL_WHITE, default=0): cv.int_range(min=0, max=255),
            # Dimming and mapping options
            cv.Optional(CONF_LOGARITHMIC_DIMMING, default=False): cv.boolean,
            cv.Optional(CONF_GAMMA, default=1.0): cv.float_range(min=0.1, max=5.0),
            # LP5562-like options (stored/logged; used for scaling)
            cv.Optional(CONF_USE_INTERNAL_CLK, default=False): cv.boolean,
            cv.Optional(CONF_POWER_SAVE_MODE, default=False): cv.boolean,
            cv.Optional(CONF_HIGH_PWM_FREQ, default=False): cv.boolean,
            # Per-color nominal current; used to compute scaling vs ref_current
            cv.Optional(CONF_RED_CURRENT, default=22.5): cv.float_,
            cv.Optional(CONF_GREEN_CURRENT, default=22.5): cv.float_,
            cv.Optional(CONF_BLUE_CURRENT, default=22.5): cv.float_,
            cv.Optional(CONF_WHITE_CURRENT, default=22.5): cv.float_,
            cv.Optional(CONF_REF_CURRENT, default=22.5): cv.float_,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x1A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    # 设置初始参数，实际写入放在 C++ setup 中执行
    cg.add(var.set_initial_strip(config[CONF_STRIP]))
    cg.add(var.set_initial_brightness(config[CONF_BRIGHTNESS]))
    cg.add(var.set_initial_white(config[CONF_INITIAL_WHITE]))
    # Dimming/scaling configuration
    cg.add(var.set_logarithmic_dimming(config[CONF_LOGARITHMIC_DIMMING]))
    cg.add(var.set_gamma(config[CONF_GAMMA]))
    cg.add(var.set_use_internal_clk(config[CONF_USE_INTERNAL_CLK]))
    cg.add(var.set_power_save_mode(config[CONF_POWER_SAVE_MODE]))
    cg.add(var.set_high_pwm_freq(config[CONF_HIGH_PWM_FREQ]))
    cg.add(var.set_ref_current(config[CONF_REF_CURRENT]))
    cg.add(var.set_color_currents(
        config[CONF_RED_CURRENT],
        config[CONF_GREEN_CURRENT],
        config[CONF_BLUE_CURRENT],
        config[CONF_WHITE_CURRENT]
    ))