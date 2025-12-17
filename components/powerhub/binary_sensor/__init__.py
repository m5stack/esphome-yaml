import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BUTTON,
)
from .. import CONF_POWER_HUB_ID, BASE_SCHEMA

DEPENDENCIES = ["powerhub"]

# the Top PMU button is a square shape
ICON_GESTURE_TAP_BUTTON = "mdi:gesture-tap-button"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_BUTTON) : binary_sensor.binary_sensor_schema(
                icon=ICON_GESTURE_TAP_BUTTON
            )
        }
    )
    .extend(BASE_SCHEMA)
)


async def to_code(config):
    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    if CONF_BUTTON in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(powerhub.set_button_binary_sensor(sens))