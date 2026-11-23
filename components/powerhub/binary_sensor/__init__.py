import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BUTTON,
)
from .. import CONF_POWER_HUB_ID, BASE_SCHEMA

DEPENDENCIES = ["powerhub"]

CONF_VIN_STATUS = "vin_status"

# the Top PMU button is a square shape
ICON_GESTURE_TAP_BUTTON = "mdi:gesture-tap-button"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_BUTTON): binary_sensor.binary_sensor_schema(icon=ICON_GESTURE_TAP_BUTTON).extend(
                {
                    cv.Optional("update_interval", default=20): cv.positive_int,
                }
            ),
            cv.Optional(CONF_VIN_STATUS): binary_sensor.binary_sensor_schema(icon="mdi:power-plug").extend(
                {
                    cv.Optional("update_interval", default=500): cv.positive_int,
                }
            ),
        }
    )
    .extend(BASE_SCHEMA)
)


async def to_code(config):
    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    if CONF_BUTTON in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(powerhub.set_button_binary_sensor(sens))
        cg.add(powerhub.set_button_update_interval(config[CONF_BUTTON]["update_interval"]))

    if CONF_VIN_STATUS in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_VIN_STATUS])
        cg.add(powerhub.set_vin_status_binary_sensor(sens))
        cg.add(powerhub.set_vin_status_update_interval(config[CONF_VIN_STATUS]["update_interval"]))