import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]
CONF_MODULE_4_RELAY_ID = "module4relay_id"

module4relay_ns = cg.esphome_ns.namespace("module4relay")
Module4Relay = module4relay_ns.class_("Module4Relay", cg.Component, i2c.I2CDevice)


BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_MODULE_4_RELAY_ID): cv.use_id(Module4Relay),
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Module4Relay)
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x26))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
