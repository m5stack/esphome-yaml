import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    ICON_CHIP,
    ENTITY_CATEGORY_DIAGNOSTIC
)
from .. import CONF_POWER_HUB_ID, BASE_SCHEMA

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

CONF_CHARGE_STATUS = "charge_status"
CONF_VIN_STATUS = "vin_status"
CONF_FIRMWARE_VER = "firmware_ver"
CONF_BOOTLOADER_VER = "bootloader_ver"

ICON_POWER_PLUG_BATTERY = "mdi:power-plug-battery"
ICON_POWER_PLUG = "mdi:power-plug"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_CHARGE_STATUS): text_sensor.text_sensor_schema(icon=ICON_POWER_PLUG_BATTERY).extend(
                {
                    cv.Optional("update_interval", default=500): cv.positive_int,
                }
            ),
            cv.Optional(CONF_VIN_STATUS): text_sensor.text_sensor_schema(icon=ICON_POWER_PLUG).extend(
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
    
    if CONF_CHARGE_STATUS in config:
        ts = await text_sensor.new_text_sensor(config[CONF_CHARGE_STATUS])
        cg.add(powerhub.set_charge_status_text_sensor(ts))
        cg.add(powerhub.set_charge_status_update_interval(config[CONF_CHARGE_STATUS]["update_interval"]))
    
    if CONF_VIN_STATUS in config:
        ts = await text_sensor.new_text_sensor(config[CONF_VIN_STATUS])
        cg.add(powerhub.set_vin_status_text_sensor(ts))
        cg.add(powerhub.set_vin_status_update_interval(config[CONF_VIN_STATUS]["update_interval"]))
