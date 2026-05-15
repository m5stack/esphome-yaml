import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    STATE_CLASS_NONE,
    UNIT_STEPS,
    ICON_ROTATE_RIGHT,
)

from . import M5UnitEncoder

DEPENDENCIES = ["i2c"]

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        M5UnitEncoder,
        unit_of_measurement=UNIT_STEPS,
        icon=ICON_ROTATE_RIGHT,
        accuracy_decimals=0,
        state_class=STATE_CLASS_NONE,
    )
    .extend(i2c.i2c_device_schema(0x40))
    .extend(cv.polling_component_schema("100ms"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
