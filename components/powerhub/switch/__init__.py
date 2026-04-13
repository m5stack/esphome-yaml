import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_CHANNEL,
    ICON_POWER,
)

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

ICON_EXPORT = "mdi:export"

# General Power switch on powerhub
PowerSwitch = powerhub_ns.class_("PowerSwitch", switch.Switch, cg.Component)

# The RS485/CAN power direction switch
RS485CANDirSwitch = powerhub_ns.class_("RS485CANDirSwitch", switch.Switch, cg.Component)
PowerChannel = powerhub_ns.enum("PowerChannel", is_class=True)

POWER_CHANNEL = {
    "LED":        PowerChannel.POWER_LED,
    "USB":        PowerChannel.POWER_USB,
    "GROVE_RED":  PowerChannel.POWER_GROVE_RED,
    "GROVE_BLUE": PowerChannel.POWER_GROVE_BLUE,
    "RS485_CAN":  PowerChannel.POWER_RS485_CAN,
    "VAMETER":    PowerChannel.POWER_VAMETER,
    "CHARGE":     PowerChannel.POWER_CHARGE,
}

CONF_RS485_CAN_DIR = "rs485_can_dir"
ICON_ARROW_LEFT_RIGHT = "mdi:arrow-left-right"

POWER_SWITCH_SCHEMA = cv.Schema(
    switch.switch_schema(
        PowerSwitch,
        icon=ICON_POWER,
    )
    .extend(
        {
            cv.Required(CONF_CHANNEL): cv.enum(POWER_CHANNEL),
        }
    )
    .extend(BASE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

RS485_CAN_DIR_SCHEMA = cv.Schema(
    switch.switch_schema(
        RS485CANDirSwitch,
        icon=ICON_ARROW_LEFT_RIGHT
    )
    .extend(BASE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)

def CONFIG_SCHEMA(config):
    if CONF_CHANNEL in config:
        return POWER_SWITCH_SCHEMA(config)
    return RS485_CAN_DIR_SCHEMA(config)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_POWER_HUB_ID])
    if CONF_CHANNEL in config:
        cg.add(var.set_channel(config[CONF_CHANNEL]))
