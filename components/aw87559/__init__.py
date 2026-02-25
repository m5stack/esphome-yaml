import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@Jasionf"]

aw87559_ns = cg.esphome_ns.namespace("aw87559")
AW87559Component = aw87559_ns.class_(
    "AW87559Component", cg.Component, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AW87559Component),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x5B))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)