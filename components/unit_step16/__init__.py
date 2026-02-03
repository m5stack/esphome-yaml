import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]

unit_step16_ns = cg.esphome_ns.namespace("unit_step16")
UnitStep16Component = unit_step16_ns.class_("UnitStep16Component", cg.Component, i2c.I2CDevice)
OutputChannel = unit_step16_ns.enum("OutputChannel", is_class=True)

CONF_UNIT_STEP16_ID = "unit_step16_id"
CONF_LED_BRIGHTNESS = "led_brightness"
CONF_LED_ENABLED = "led_enabled"
CONF_RGB_BRIGHTNESS = "rgb_brightness"
CONF_RGB_ENABLED = "rgb_enabled"
CONF_RGB_RED = "rgb_red"
CONF_RGB_GREEN = "rgb_green"
CONF_RGB_BLUE = "rgb_blue"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UnitStep16Component),
            cv.Optional(CONF_LED_BRIGHTNESS, default=50): cv.int_range(min=0, max=100),
            cv.Optional(CONF_LED_ENABLED, default=True): cv.boolean,
            cv.Optional(CONF_RGB_BRIGHTNESS, default=50): cv.int_range(min=0, max=100),
            cv.Optional(CONF_RGB_ENABLED, default=True): cv.boolean,
            cv.Optional(CONF_RGB_RED, default=0): cv.int_range(min=0, max=255),
            cv.Optional(CONF_RGB_GREEN, default=255): cv.int_range(min=0, max=255),
            cv.Optional(CONF_RGB_BLUE, default=0): cv.int_range(min=0, max=255),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x48))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
    # Set initial values
    cg.add(var.set_initial_led_brightness(config[CONF_LED_BRIGHTNESS]))
    cg.add(var.set_initial_led_enabled(config[CONF_LED_ENABLED]))
    cg.add(var.set_initial_rgb_brightness(config[CONF_RGB_BRIGHTNESS]))
    cg.add(var.set_initial_rgb_enabled(config[CONF_RGB_ENABLED]))
    cg.add(var.set_initial_rgb_color(config[CONF_RGB_RED], config[CONF_RGB_GREEN], config[CONF_RGB_BLUE]))

