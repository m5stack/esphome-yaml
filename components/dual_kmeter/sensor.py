import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    UNIT_CELSIUS,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    ICON_THERMOMETER,
    CONF_ID,
    CONF_CHANNEL,
)

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@m5stack"]


CONF_UNIT = "unit"

dual_kmeter = cg.esphome_ns.namespace("dual_kmeter")
DualKmeterComponent = dual_kmeter.class_(
    "DualKmeterComponent", sensor.Sensor, i2c.I2CDevice, cg.PollingComponent)

DualKmeterChannel = dual_kmeter.enum("KmeterChannel")

KMETER_CHANNELS = {
    "KMETER_1" : DualKmeterChannel.KMETER_1,
    "KMETER_2" : DualKmeterChannel.KMETER_2,
    "INTERNAL" : DualKmeterChannel.INTERNAL
}

DualKmeterUnit = dual_kmeter.enum("KmeterUnit")

KMETER_UNITS = {
    "CELSIUS" : DualKmeterUnit.CELSIUS,
    "FAHRENHEIT" : DualKmeterUnit.FAHRENHEIT
}

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        DualKmeterComponent,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
        unit_of_measurement=UNIT_CELSIUS,
        icon=ICON_THERMOMETER).extend(
            {
                cv.Required(CONF_CHANNEL): cv.enum(
                    KMETER_CHANNELS, upper=True, space='_'
                ),
                cv.Optional(CONF_UNIT, default="CELSIUS"): cv.enum(
                    KMETER_UNITS, upper=True, space=')'
                )
            }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x11))  # default address of Module dual Kmeter
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    await sensor.register_sensor(var, config)

    cg.add(var.set_kmeter_unit(config[CONF_UNIT]))
    cg.add(var.set_kmeter_channel(config[CONF_CHANNEL]))

