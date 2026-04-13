import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light

from esphome.const import (
    CONF_OUTPUT_ID,
    CONF_CHANNEL,
)

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

PowerHubLight = powerhub_ns.class_("PowerHubLight", light.LightOutput)
LightChannel = powerhub_ns.enum("LightChannel", is_class=True)

LIGHT_CHANNEL = {
    "USB_A":       LightChannel.LED_USB_A,
    "USB_C":       LightChannel.LED_USB_C,
    "GROVE_RED":   LightChannel.LED_GROVE_RED,
    "GROVE_BLUE":  LightChannel.LED_GROVE_BLUE,
    "RS485_CAN":   LightChannel.LED_RS485_CAN,
    "CHARGE":      LightChannel.LED_CHARGE,
    "POWER_LEFT":  LightChannel.LED_POWER_LEFT,
    "POWER_RIGHT": LightChannel.LED_POWER_RIGHT,
}

CONFIG_SCHEMA = light.light_schema(
    PowerHubLight,
    light.LightType.RGB,
).extend(
    {
        cv.Required(CONF_CHANNEL): cv.enum(LIGHT_CHANNEL),
    }
).extend(BASE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await light.register_light(var, config)
    await cg.register_parented(var, config[CONF_POWER_HUB_ID])
    cg.add(var.set_channel(config[CONF_CHANNEL]))
