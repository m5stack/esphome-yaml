import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

CODEOWNERS = ["@ItsRebaseTime"]
DEPENDENCIES = ["i2c"]
MULTI_CONF = True

m5stack_roller_ns = cg.esphome_ns.namespace("m5stack_roller")
RollerBase = m5stack_roller_ns.class_("RollerBase")

m5stack_roller_can_i2c_ns = cg.esphome_ns.namespace("m5stack_roller_can_i2c")
RollerMotorCANI2C = m5stack_roller_can_i2c_ns.class_(
    "RollerMotorCANI2C", RollerBase, cg.PollingComponent, i2c.I2CDevice
)

CONF_MOTOR_ID = "motor_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RollerMotorCANI2C),
            cv.Optional(CONF_MOTOR_ID, default=1): cv.int_range(min=0, max=255),
        }
    )
    .extend(cv.polling_component_schema("500ms"))
    .extend(i2c.i2c_device_schema(0x64))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_motor_id(config[CONF_MOTOR_ID]))
