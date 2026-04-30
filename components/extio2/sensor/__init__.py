import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
    CONF_RESOLUTION
)

from .. import (
    extio2_ns, 
    BASE_SCHEMA, 
    CONF_EXTIO2_ID
)

DEPENDENCIES = ["extio2"]


EXTIO2Sensor = extio2_ns.class_("EXTIO2Sensor", sensor.Sensor, cg.PollingComponent)
EXTIO2ADCChannel = extio2_ns.enum("EXTIO2ADCChannel")
EXTIO2ADCResolution = extio2_ns.enum("EXTIO2ADCResolution")

ADC_CHANNEL = {
    "ADC_0" : EXTIO2ADCChannel.ADC_0,
    "ADC_1" : EXTIO2ADCChannel.ADC_1,
    "ADC_2" : EXTIO2ADCChannel.ADC_2,
    "ADC_3" : EXTIO2ADCChannel.ADC_3,
    "ADC_4" : EXTIO2ADCChannel.ADC_4,
    "ADC_5" : EXTIO2ADCChannel.ADC_5,
    "ADC_6" : EXTIO2ADCChannel.ADC_6,
    "ADC_7" : EXTIO2ADCChannel.ADC_7,
}

ADC_RESOLUTION = {
    "8BIT" : EXTIO2ADCResolution.ADC_8BIT,
    "12BIT" : EXTIO2ADCResolution.ADC_12BIT
}

CONFIG_SCHEMA = (
    cv.Schema(
        sensor.sensor_schema(EXTIO2Sensor).extend(
            {
                cv.Required(CONF_CHANNEL) : cv.enum(
                    ADC_CHANNEL, upper=True, space='_'),
                cv.Optional(CONF_RESOLUTION, default="8bit") : cv.enum(
                    ADC_RESOLUTION, upper=True, space='_'
                )
            }
        )
        .extend(BASE_SCHEMA)
        .extend(cv.polling_component_schema("60s"))
    )
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_EXTIO2_ID])
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_adc_channel(config[CONF_CHANNEL]))
    cg.add(var.set_adc_resolution(config[CONF_RESOLUTION]))