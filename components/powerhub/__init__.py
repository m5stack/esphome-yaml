import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_ID


DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]
CONF_POWER_HUB_ID = "powerhub_id"

powerhub_ns = cg.esphome_ns.namespace("powerhub")
PowerHub = powerhub_ns.class_("PowerHub", cg.PollingComponent, i2c.I2CDevice)


BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_POWER_HUB_ID): cv.use_id(PowerHub),
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PowerHub)
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x50)) # Default STM32 address
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)