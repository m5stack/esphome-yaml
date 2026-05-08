import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_SPEED,
    CONF_CURRENT,
    CONF_TEMPERATURE,
    ICON_FLASH,
    ICON_THERMOMETER,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_TEMPERATURE,
    UNIT_CELSIUS,
    UNIT_AMPERE,
    UNIT_VOLT,
)
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = ["m5stack_roller"]
CODEOWNERS = ["@ItsRebaseTime"]

CONF_POSITION = "position"
CONF_VIN = "vin"
CONF_ENCODER = "encoder"

UNIT_RPM = "rpm"
UNIT_DEGREE = "°"
UNIT_MILLIAMPERE = "mA"
UNIT_COUNT = "count"

ICON_SPEEDOMETER = "mdi:speedometer"
ICON_ROTATE = "mdi:rotate-right"
ICON_CURRENT_DC = "mdi:current-dc"
ICON_ENCODER = "mdi:counter"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),  # dummy – sensors are sub-entities
        cv.Optional(CONF_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_RPM,
            icon=ICON_SPEEDOMETER,
            accuracy_decimals=1,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_POSITION): sensor.sensor_schema(
            unit_of_measurement=UNIT_DEGREE,
            icon=ICON_ROTATE,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_MILLIAMPERE,
            icon=ICON_CURRENT_DC,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VIN): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            icon=ICON_FLASH,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ENCODER): sensor.sensor_schema(
            unit_of_measurement=UNIT_COUNT,
            icon=ICON_ENCODER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_SPEED in config:
        s = await sensor.new_sensor(config[CONF_SPEED])
        cg.add(parent.set_speed_sensor(s))

    if CONF_POSITION in config:
        s = await sensor.new_sensor(config[CONF_POSITION])
        cg.add(parent.set_position_sensor(s))

    if CONF_CURRENT in config:
        s = await sensor.new_sensor(config[CONF_CURRENT])
        cg.add(parent.set_current_sensor(s))

    if CONF_VIN in config:
        s = await sensor.new_sensor(config[CONF_VIN])
        cg.add(parent.set_vin_sensor(s))

    if CONF_TEMPERATURE in config:
        s = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(parent.set_temperature_sensor(s))

    if CONF_ENCODER in config:
        s = await sensor.new_sensor(config[CONF_ENCODER])
        cg.add(parent.set_encoder_sensor(s))
