import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    DEVICE_CLASS_SWITCH,
    ICON_FLASH
)

from .. import unit4relay_ns, BASE_SCHEMA, CONF_UNIT_4_RELAY_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["unit4relay"]


CONF_RELAY_1 = "relay_1"
CONF_RELAY_2 = "relay_2"
CONF_RELAY_3 = "relay_3"
CONF_RELAY_4 = "relay_4"


Unit4RelaySwitch1 = unit4relay_ns.class_("Unit4RelaySwitch1", switch.Switch, cg.Component)
Unit4RelaySwitch2 = unit4relay_ns.class_("Unit4RelaySwitch2", switch.Switch, cg.Component)
Unit4RelaySwitch3 = unit4relay_ns.class_("Unit4RelaySwitch3", switch.Switch, cg.Component)
Unit4RelaySwitch4 = unit4relay_ns.class_("Unit4RelaySwitch4", switch.Switch, cg.Component)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_RELAY_1): switch.switch_schema(
            Unit4RelaySwitch1,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_FLASH,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_RELAY_2): switch.switch_schema(
            Unit4RelaySwitch2,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_FLASH,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_RELAY_3): switch.switch_schema(
            Unit4RelaySwitch3,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_FLASH,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_RELAY_4): switch.switch_schema(
            Unit4RelaySwitch4,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_FLASH,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
    }

).extend(BASE_SCHEMA)


async def to_code(config):

    relayhub = await cg.get_variable(config[CONF_UNIT_4_RELAY_ID])

    for switch_type in [
        CONF_RELAY_1, 
        CONF_RELAY_2, 
        CONF_RELAY_3, 
        CONF_RELAY_4, 
    ]:
        if conf := config.get(switch_type):
            var = await switch.new_switch(conf)
            await cg.register_component(var, conf)
            await cg.register_parented(var, relayhub)
            cg.add(getattr(relayhub, f"set_{switch_type}_switch")(var))