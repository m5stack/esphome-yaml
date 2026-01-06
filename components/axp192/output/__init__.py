import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import (
    CONF_ID, 
    CONF_VOLTAGE,
    CONF_SWITCH,
    CONF_CHANNEL,
    CONF_STEP,
    CONF_RANGE,
    CONF_TYPE
)

from .. import axp192_ns, BASE_SCHEMA, CONF_AXP192_ID

DEPENDENCIES = ["axp192"]

CONF_MIN_VOLTAGE = "min_voltage"
CONF_MAX_VOLTAGE = "max_voltage"

AXP192FloatOutput = axp192_ns.class_("AXP192FloatOutput", output.FloatOutput)
AXP192BinaryOutput = axp192_ns.class_("AXP192BinaryOutput", output.BinaryOutput)
PowerChannel = axp192_ns.enum("PowerChannel", is_class=True)

POWER_CHANNELS = {
    "DCDC1": PowerChannel.DCDC1,
    "DCDC2": PowerChannel.DCDC2,
    "DCDC3": PowerChannel.DCDC3,
    "ALDO1": PowerChannel.ALDO1,
    "ALDO2": PowerChannel.ALDO2,
    "ALDO3": PowerChannel.ALDO3,
    "ALDO4": PowerChannel.ALDO4,
    "DLDO2": PowerChannel.DLDO2,
    "DLDO3": PowerChannel.DLDO3,
}

CONFIG_SCHEMA = cv.typed_schema(
    {
        # Use type: range to cooperate with FloatOutput component.
        # This can be used to tweak the output voltage range
        CONF_RANGE: output.FLOAT_OUTPUT_SCHEMA.extend(BASE_SCHEMA).extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(AXP192FloatOutput),
                cv.Required(CONF_CHANNEL): cv.enum(POWER_CHANNELS, upper=True, space="_"),
                cv.Optional(CONF_STEP, default=100): cv.uint16_t, 
                cv.Optional(CONF_MIN_VOLTAGE, default=1000): cv.uint16_t,
                cv.Optional(CONF_MAX_VOLTAGE, default=3300): cv.uint16_t,
            }
        ),

        CONF_SWITCH: output.BINARY_OUTPUT_SCHEMA.extend(BASE_SCHEMA).extend(
            {
                cv.GenerateID(CONF_ID): cv.declare_id(AXP192BinaryOutput),
                cv.Required(CONF_CHANNEL): cv.enum(POWER_CHANNELS, upper=True, space="_"),
                cv.Optional(CONF_VOLTAGE, default=1800): cv.uint16_t,
            }
        )
    },
    key=CONF_TYPE,
    lower=True,
    default_type="switch",
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AXP192_ID])
    await output.register_output(var, config)

    if config[CONF_TYPE] == CONF_RANGE:
        cg.add(var.set_channel(config[CONF_CHANNEL]))
        cg.add(var.set_min_voltage(config[CONF_MIN_VOLTAGE]))
        cg.add(var.set_max_voltage(config[CONF_MAX_VOLTAGE]))
    else:
        cg.add(var.set_channel(config[CONF_CHANNEL]))
        cg.add(var.set_voltage(config[CONF_VOLTAGE]))
    