import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output

from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
)

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

USBCLight = powerhub_ns.class_("USBCLight", light.LightOutput)
USBALight = powerhub_ns.class_("USBALight", light.LightOutput)
GroveBlueLight = powerhub_ns.class_("GroveBlueLight", light.LightOutput)
GroveRedLight = powerhub_ns.class_("GroveRedLight", light.LightOutput)
RS485CANLight = powerhub_ns.class_("RS485CANLight", light.LightOutput)
BATChargeLight = powerhub_ns.class_("BATChargeLight", light.LightOutput)
PowerLeftLight = powerhub_ns.class_("PowerLeftLight", light.LightOutput)
PowerRightLight = powerhub_ns.class_("PowerRightLight", light.LightOutput)

CONF_USB_C_RGB = "usb_c_rgb"
CONF_USB_A_RGB = "usb_a_rgb"
CONF_GROVE_BLUE_RGB = "grove_blue_rgb"
CONF_GROVE_RED_RGB = "grove_red_rgb"
CONF_RS485_CAN_RGB = "rs485_can_rgb"
CONF_BAT_CHARGE_RGB = "bat_charge_rgb"
CONF_PWR_L_RGB = "pwr_l_rgb"
CONF_PWR_R_RGB = "pwr_r_rgb"


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_USB_C_RGB): light.light_schema(
            USBCLight,
            light.LightType.RGB,
        ),
        cv.Optional(CONF_USB_A_RGB): light.light_schema(
            USBALight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_GROVE_BLUE_RGB): light.light_schema(
            GroveBlueLight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_GROVE_RED_RGB): light.light_schema(
            GroveRedLight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_RS485_CAN_RGB): light.light_schema(
            RS485CANLight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_BAT_CHARGE_RGB): light.light_schema(
            BATChargeLight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_PWR_L_RGB): light.light_schema(
            PowerLeftLight,
            light.LightType.RGB
        ),
        cv.Optional(CONF_PWR_R_RGB): light.light_schema(
            PowerRightLight,
            light.LightType.RGB
        ),
    }
).extend(BASE_SCHEMA)


async def to_code(config):

    if CONF_USB_C_RGB in config:
        var = await light.new_light(config[CONF_USB_C_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_USB_A_RGB in config:
        var = await light.new_light(config[CONF_USB_A_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_GROVE_BLUE_RGB in config:
        var = await light.new_light(config[CONF_GROVE_BLUE_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_GROVE_RED_RGB in config:
        var = await light.new_light(config[CONF_GROVE_RED_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_RS485_CAN_RGB in config:
        var = await light.new_light(config[CONF_RS485_CAN_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_BAT_CHARGE_RGB in config:
        var = await light.new_light(config[CONF_BAT_CHARGE_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_PWR_L_RGB in config:
        var = await light.new_light(config[CONF_PWR_L_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])

    if CONF_PWR_R_RGB in config:
        var = await light.new_light(config[CONF_PWR_R_RGB])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])










