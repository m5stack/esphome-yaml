import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["i2c"]

CONF_M5UNIT_ENCODER_ID = "m5unit_encoder_id"

m5unit_encoder_ns = cg.esphome_ns.namespace("m5unit_encoder")
M5UnitEncoder = m5unit_encoder_ns.class_(
    "M5UnitEncoder", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.Schema({cv.GenerateID(): cv.declare_id(M5UnitEncoder)})


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
