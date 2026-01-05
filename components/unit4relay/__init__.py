import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]
CONF_UNIT_4_RELAY_ID = "unit4relay_id"

unit4relay_ns = cg.esphome_ns.namespace("unit4relay")
Unit4Relay = unit4relay_ns.class_("Unit4Relay", cg.Component, i2c.I2CDevice)


BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_UNIT_4_RELAY_ID): cv.use_id(Unit4Relay),
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Unit4Relay)
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x26)) # Default STM32 address
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)