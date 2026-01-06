import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_SWITCH,
    ICON_POWER
)

from .. import axp192_ns, BASE_SCHEMA, CONF_AXP192_ID

DEPENDENCIES = ["axp192"]

CONF_LED_PWR = "led_pwr"

AXP192Switch = axp192_ns.class_("AXP192Switch", switch.Switch, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_LED_PWR): switch.switch_schema(
            AXP192Switch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_ON"
        ),
    }
).extend(BASE_SCHEMA)

 
async def to_code(config):
    parentvar = await cg.get_variable(config[CONF_AXP192_ID])
    if conf := config.get(CONF_LED_PWR):
        var = await switch.new_switch(conf)
        await cg.register_component(var, conf)
        await cg.register_parented(var, parentvar)
