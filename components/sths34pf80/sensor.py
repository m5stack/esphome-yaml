import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor, binary_sensor
from esphome.const import (
    CONF_ID,
    UNIT_CELSIUS,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
)

from . import STHS34PF80Component

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["binary_sensor"]

CONF_PRESENCE = "presence"
CONF_MOTION = "motion"
CONF_TEMPERATURE = "temperature"
CONF_PRES_FLAG = "pres_flag"
CONF_MOT_FLAG = "mot_flag"
CONF_PRESENCE_THRESHOLD = "presence_threshold"
CONF_MOTION_THRESHOLD = "motion_threshold"
CONF_PRESENCE_HYSTERESIS = "presence_hysteresis"
CONF_MOTION_HYSTERESIS = "motion_hysteresis"
CONF_ODR = "odr"


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(STHS34PF80Component),
            cv.Optional(CONF_PRESENCE): sensor.sensor_schema(
                icon="mdi:account",
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MOTION): sensor.sensor_schema(
                icon="mdi:run",
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_PRES_FLAG): binary_sensor.binary_sensor_schema(
                icon="mdi:account-check",
            ),
            cv.Optional(CONF_MOT_FLAG): binary_sensor.binary_sensor_schema(
                icon="mdi:run-fast",
            ),
            # Hardware detection thresholds (0–32767); default matches Arduino example (0xC8 = 200)
            cv.Optional(CONF_PRESENCE_THRESHOLD, default=200): cv.int_range(min=0, max=32767),
            cv.Optional(CONF_MOTION_THRESHOLD, default=200): cv.int_range(min=0, max=32767),
            # Hysteresis (0–255); default matches Arduino example (0x32 = 50)
            cv.Optional(CONF_PRESENCE_HYSTERESIS, default=50): cv.int_range(min=0, max=255),
            cv.Optional(CONF_MOTION_HYSTERESIS, default=50): cv.int_range(min=0, max=255),
            # Internal sensor ODR (1=0.25Hz 2=0.5Hz 3=1Hz 4=2Hz 5=4Hz 6=8Hz 7=15Hz 8=30Hz)
            # MUST be >= 1/update_interval. Higher ODR = smaller per-frame thermal deltas
            # = fewer false motion triggers. Default 8 (30Hz) matches Arduino example.
            cv.Optional(CONF_ODR, default=8): cv.int_range(min=1, max=8),
        }
    )
    .extend(cv.polling_component_schema("1s"))
    .extend(i2c.i2c_device_schema(0x5A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if presence_conf := config.get(CONF_PRESENCE):
        cg.add(var.set_presence_sensor(await sensor.new_sensor(presence_conf)))

    if motion_conf := config.get(CONF_MOTION):
        cg.add(var.set_motion_sensor(await sensor.new_sensor(motion_conf)))

    if temp_conf := config.get(CONF_TEMPERATURE):
        cg.add(var.set_temperature_sensor(await sensor.new_sensor(temp_conf)))

    if pres_flag_conf := config.get(CONF_PRES_FLAG):
        cg.add(var.set_pres_flag_sensor(await binary_sensor.new_binary_sensor(pres_flag_conf)))

    if mot_flag_conf := config.get(CONF_MOT_FLAG):
        cg.add(var.set_mot_flag_sensor(await binary_sensor.new_binary_sensor(mot_flag_conf)))

    cg.add(var.set_presence_threshold(config[CONF_PRESENCE_THRESHOLD]))
    cg.add(var.set_motion_threshold(config[CONF_MOTION_THRESHOLD]))
    cg.add(var.set_presence_hysteresis(config[CONF_PRESENCE_HYSTERESIS]))
    cg.add(var.set_motion_hysteresis(config[CONF_MOTION_HYSTERESIS]))
    cg.add(var.set_odr(config[CONF_ODR]))

