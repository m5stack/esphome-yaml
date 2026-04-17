import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import CONF_ID

CODEOWNERS = ["@ItsRebaseTime"]
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["binary_sensor", "sensor", "output"]
MULTI_CONF = True

CONF_I2C_JOYSTICK_2_ID = "i2c_joystick_2_id"


i2c_joystick_2_ns = cg.esphome_ns.namespace("i2c_joystick_2")
I2CJoystick2Component = i2c_joystick_2_ns.class_(
    "I2CJoystick2Component",
    cg.Component,
    i2c.I2CDevice,
)
I2CJoystick2Sensor = i2c_joystick_2_ns.class_(
    "I2CJoystick2Sensor",
    sensor.Sensor,
    cg.PollingComponent,
)
I2CJoystick2BinarySensor = i2c_joystick_2_ns.class_(
    "I2CJoystick2BinarySensor",
    binary_sensor.BinarySensor,
    cg.PollingComponent,
)


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(I2CJoystick2Component),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x63))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
