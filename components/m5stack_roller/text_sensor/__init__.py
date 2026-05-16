import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import CONF_ID
from .. import ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = []
CODEOWNERS = ["@ItsRebaseTime"]

CONF_MOTOR_STATUS = "motor_status"
CONF_MOTOR_MODE = "motor_mode"
CONF_MOTOR_ERROR = "motor_error"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_MOTOR_STATUS): text_sensor.text_sensor_schema(
            icon="mdi:state-machine",
        ),
        cv.Optional(CONF_MOTOR_MODE): text_sensor.text_sensor_schema(
            icon="mdi:cog-outline",
        ),
        cv.Optional(CONF_MOTOR_ERROR): text_sensor.text_sensor_schema(
            icon="mdi:alert-circle-outline",
        ),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_MOTOR_STATUS in config:
        s = await text_sensor.new_text_sensor(config[CONF_MOTOR_STATUS])
        cg.add(parent.set_status_text_sensor(s))

    if CONF_MOTOR_MODE in config:
        s = await text_sensor.new_text_sensor(config[CONF_MOTOR_MODE])
        cg.add(parent.set_mode_text_sensor(s))

    if CONF_MOTOR_ERROR in config:
        s = await text_sensor.new_text_sensor(config[CONF_MOTOR_ERROR])
        cg.add(parent.set_error_text_sensor(s))
