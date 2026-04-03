import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import (
    CONF_ID,
    CONF_PINS,
    CONF_PIN,
    CONF_LEVEL,
    CONF_DELAY
)

early_init_ns = cg.esphome_ns.namespace("early_init")
EarlyInitComponent = early_init_ns.class_("EarlyInitComponent", cg.Component)
PIN_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_LEVEL, default=False): cv.boolean,
    }
)
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(EarlyInitComponent),
        cv.Optional(CONF_PINS, default=[]): cv.ensure_list(PIN_SCHEMA),
        cv.Optional(CONF_DELAY, default="0ms"): cv.positive_time_period_milliseconds,
    }
).extend(cv.COMPONENT_SCHEMA)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    for pin_conf in config[CONF_PINS]:
        pin = await cg.gpio_pin_expression(pin_conf[CONF_PIN])
        cg.add(var.add_pin(pin, pin_conf[CONF_LEVEL]))
    cg.add(var.set_delay(config[CONF_DELAY].total_milliseconds))