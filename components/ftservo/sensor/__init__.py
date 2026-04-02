import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID, 
    CONF_POSITION,
    CONF_SPEED,
    CONF_VOLTAGE,
    CONF_CURRENT,
    CONF_TEMPERATURE,
    ICON_ACCELERATION,
    ICON_FLASH,
    ICON_THERMOMETER,
    UNIT_STEPS,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_VOLTAGE
)

from .. import ftservo_ns, FTSERVO_BASE_SCHEMA, CONF_FTSERVO_ID

DEPENDENCIES = ["ftservo"]
CODEOWNERS = ["@m5stack"]

CONF_LOAD = "load"
UNIT_STEPS_PER_SECOND = "steps/s"

ICON_SPEEDOMETER = "mdi:speedometer"
ICON_WEIGHT_GRAM = "mdi:weight-gram"
ICON_CURRENT_DC = "mdi:current-dc"

FTServoSensor = ftservo_ns.class_("FTServoSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(FTServoSensor)
    .extend(
        {
            cv.Optional(CONF_POSITION) : sensor.sensor_schema(
                unit_of_measurement=UNIT_STEPS,
                icon=ICON_ACCELERATION,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SPEED) : sensor.sensor_schema(
                unit_of_measurement=UNIT_STEPS_PER_SECOND,
                icon=ICON_SPEEDOMETER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_LOAD) : sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_WEIGHT_GRAM,
                state_class=STATE_CLASS_MEASUREMENT,                
            ),
            cv.Optional(CONF_VOLTAGE) : sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=1,
                state_class=STATE_CLASS_MEASUREMENT
            ),
            cv.Optional(CONF_CURRENT) : sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_DC,
                state_class=STATE_CLASS_MEASUREMENT
            ),
            cv.Optional(CONF_TEMPERATURE) : sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                state_class=STATE_CLASS_MEASUREMENT
            )
        }
        
    )
    .extend(FTSERVO_BASE_SCHEMA)
    .extend(cv.polling_component_schema("200ms"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    
    parent = await cg.get_variable(config[CONF_FTSERVO_ID])
    cg.add(var.set_parent(parent))

    if CONF_POSITION in config:
        sens = await sensor.new_sensor(config[CONF_POSITION])
        cg.add(var.set_position_sensor(sens))

    if CONF_SPEED in config:
        sens = await sensor.new_sensor(config[CONF_SPEED])
        cg.add(var.set_speed_sensor(sens))
    
    if CONF_LOAD in config:
        sens = await sensor.new_sensor(config[CONF_LOAD])
        cg.add(var.set_load_sensor(sens))
    
    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage_sensor(sens))
    
    if CONF_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_CURRENT])
        cg.add(var.set_current_sensor(sens))
    
    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
        