import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    ICON_ROTATE_RIGHT,
    STATE_CLASS_MEASUREMENT,
)
from . import UnitStep16Component, CONF_UNIT_STEP16_ID

DEPENDENCIES = ["unit_step16"]

unit_step16_ns = cg.esphome_ns.namespace("unit_step16")
UnitStep16Sensor = unit_step16_ns.class_("UnitStep16Sensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        UnitStep16Sensor,
        icon=ICON_ROTATE_RIGHT,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(CONF_UNIT_STEP16_ID): cv.use_id(UnitStep16Component),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    
    parent = await cg.get_variable(config[CONF_UNIT_STEP16_ID])
    cg.add(var.set_parent(parent))
