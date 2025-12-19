import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select

from esphome.const import (
    ENTITY_CATEGORY_CONFIG
)

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]


CONF_USB_MODE = "usb_mode"
ICON_USB_PORT = "mdi:usb-port"

USBModeSelect = powerhub_ns.class_("USBModeSelect", select.Select, cg.Component)

OPTIONS = [
    "Default",
    "Host for USB-C",
    "Host for USB-A"
]


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_USB_MODE): select.select_schema(
            USBModeSelect,
            icon=ICON_USB_PORT,
            entity_category=ENTITY_CATEGORY_CONFIG
        )
    }
).extend(BASE_SCHEMA)

async def to_code(config):
    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    if usb_mode_conf := config.get(CONF_USB_MODE):
        var = await select.new_select(
            usb_mode_conf,
            options=OPTIONS
        )
        await cg.register_component(var, usb_mode_conf)
        await cg.register_parented(var, powerhub)
        cg.add(powerhub.set_usb_mode_select(var))
