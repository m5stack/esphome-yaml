import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from . import CONF_I2C_JOYSTICK_2_ID, I2CJoystick2Component, I2CJoystick2Sensor

DEPENDENCIES = ["i2c_joystick_2"]

CONF_AXIS = "axis"
CONF_MODE = "mode"

AXIS_OPTIONS = {
    "x": 0,
    "y": 1,
}

MODE_OPTIONS = {
    "adc_16bit": 0,
    "adc_8bit": 1,
    "offset_12bit": 2,
    "offset_8bit": 3,
}

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        I2CJoystick2Sensor,
        icon="mdi:axis-arrow",
        accuracy_decimals=0,
    )
    .extend(
        {
            cv.GenerateID(CONF_I2C_JOYSTICK_2_ID): cv.use_id(I2CJoystick2Component),
            cv.Optional(CONF_AXIS, default="x"): cv.one_of(*AXIS_OPTIONS.keys(), lower=True),
            cv.Optional(CONF_MODE, default="offset_12bit"): cv.one_of(*MODE_OPTIONS.keys(), lower=True),
        }
    )
    .extend(cv.polling_component_schema("100ms"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    parent = await cg.get_variable(config[CONF_I2C_JOYSTICK_2_ID])
    cg.add(var.set_parent(parent))

    cg.add(var.set_axis(AXIS_OPTIONS[config[CONF_AXIS]]))
    cg.add(var.set_mode(MODE_OPTIONS[config[CONF_MODE]]))
