import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor

from . import M5UnitEncoder, CONF_M5UNIT_ENCODER_ID

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(icon="mdi:gesture-tap-button")
    .extend(
        {
            cv.GenerateID(CONF_M5UNIT_ENCODER_ID): cv.use_id(M5UnitEncoder),
        }
    )
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)

    parent = await cg.get_variable(config[CONF_M5UNIT_ENCODER_ID])
    cg.add(parent.set_button_sensor(var))
