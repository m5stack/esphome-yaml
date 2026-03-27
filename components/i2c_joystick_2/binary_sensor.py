import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from . import CONF_I2C_JOYSTICK_2_ID, I2CJoystick2BinarySensor, I2CJoystick2Component

DEPENDENCIES = ["i2c_joystick_2"]

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    I2CJoystick2BinarySensor,
    icon="mdi:gesture-tap-button",
).extend(
    {
        cv.GenerateID(CONF_I2C_JOYSTICK_2_ID): cv.use_id(I2CJoystick2Component),
    }
).extend(cv.polling_component_schema("100ms"))


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_I2C_JOYSTICK_2_ID])
    cg.add(var.set_parent(parent))
