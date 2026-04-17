import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from . import CONF_I2C_JOYSTICK_2_ID, I2CJoystick2Component, I2CJoystick2Sensor, i2c_joystick_2_ns

DEPENDENCIES = ["i2c_joystick_2"]

CONF_AXIS = "axis"
CONF_MODE = "mode"

JoyStick2Axis = i2c_joystick_2_ns.enum("Axis")
JoyStick2Modes = i2c_joystick_2_ns.enum("OutputMode")

AXIS_OPTIONS = {
    "X":  JoyStick2Axis.AXIS_X,
    "Y":  JoyStick2Axis.AXIS_Y,
}
MODE_OPTIONS = {
    "ADC_16BIT":    JoyStick2Modes.MODE_ADC_16BIT,
    "ADC_8BIT":     JoyStick2Modes.MODE_ADC_8BIT,
    "OFFSET_12BIT": JoyStick2Modes.MODE_OFFSET_12BIT,
    "OFFSET_8BIT":  JoyStick2Modes.MODE_OFFSET_8BIT,
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
            cv.Optional(CONF_AXIS, default="X"): cv.enum(AXIS_OPTIONS, upper=True),
            cv.Optional(CONF_MODE, default="OFFSET_12BIT"): cv.enum(MODE_OPTIONS, upper=True, space="_"),
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
