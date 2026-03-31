import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome.const import (
    CONF_ID,
    CONF_CHANNEL
)

from .. import (
    m5ioe1_ns, 
    BASE_SCHEMA, 
    ADC_CHANNEL,
    CONF_M5IOE1_ID
)

DEPENDENCIES = ["m5ioe1"]


M5IOE1Sensor = m5ioe1_ns.class_("M5IOE1Sensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    cv.Schema(
        sensor.sensor_schema(M5IOE1Sensor).extend(
            {
                cv.Required(CONF_CHANNEL) : cv.enum(ADC_CHANNEL)
            }
        )
        .extend(BASE_SCHEMA)
        .extend(cv.polling_component_schema("60s"))
    )
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_M5IOE1_ID])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_channel(config[CONF_CHANNEL]))