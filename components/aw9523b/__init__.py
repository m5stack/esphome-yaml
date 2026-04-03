from esphome import pins
import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv

from esphome.core import TimePeriodMilliseconds

from esphome.const import (
    CONF_ID,
    CONF_INPUT,
    CONF_INVERTED,
    CONF_MODE,
    CONF_NUMBER,
    CONF_OUTPUT,
    CONF_RESET
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

CONF_AW9523B_ID = "aw9523b_id"
CONF_P0_DRIVE_MODE = "p0_drive_mode"
CONF_LED_MAX_CURRENT = "led_max_current"

AW9523BP0DriveMode = aw9523b_ns.enum("AW9523BP0DriveMode")

P0_DRIVE_MODES = {
    "OPEN_DRAIN": AW9523BP0DriveMode.OPEN_DRAIN,
    "PUSH_PULL": AW9523BP0DriveMode.PUSH_PULL
}

AW9523BLEDMaxCurrent = aw9523b_ns.enum("AW9523BLEDMaxCurrent")

LED_MAX_CURRENTS = {
    "37mA": AW9523BLEDMaxCurrent.CURRENT_MAX,
    "27.75mA": AW9523BLEDMaxCurrent.CURRENT_3QUARTERS,
    "18.5mA": AW9523BLEDMaxCurrent.CURRENT_HALF,
    "9.25mA": AW9523BLEDMaxCurrent.CURRENT_1QUARTER
}

BASE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_AW9523B_ID): cv.use_id(AW9523BComponent),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.declare_id(AW9523BComponent),
            cv.Optional(CONF_RESET, default=True): cv.boolean,
            cv.Optional(CONF_P0_DRIVE_MODE, default="OPEN_DRAIN"): cv.enum(
                P0_DRIVE_MODES, upper=True, space="_"),
            cv.Optional(CONF_LED_MAX_CURRENT, default="37mA"): cv.enum(LED_MAX_CURRENTS)
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
    ],
    mode_validator=validate_mode,
).extend(BASE_SCHEMA)

@pins.PIN_SCHEMA_REGISTRY.register(CONF_AW9523B_ID, AW9523B_PIN_SCHEMA)
async def aw9523b_pin_schema(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AW9523B_ID])

    cg.add(var.set_pin(config[CONF_NUMBER]))
    cg.add(var.set_inverted(config[CONF_INVERTED]))
    cg.add(var.set_flags(pins.gpio_flags_expr(config[CONF_MODE])))
    return var

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_p0_drive_mode(config[CONF_P0_DRIVE_MODE]))
    cg.add(var.set_led_max_current(config[CONF_LED_MAX_CURRENT]))
    cg.add(var.set_reset(config[CONF_RESET]))