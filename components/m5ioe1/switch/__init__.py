import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch

from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_SWITCH,
    CONF_PIN,
    ICON_PULSE
)

from .. import m5ioe1_ns, BASE_SCHEMA, CONF_M5IOE1_ID

DEPENDENCIES = ["m5ioe1"]

CONF_AW9737A = "aw9737a"

CONF_PULSE_NUM = "pulse_num"

AW9737ASwitch = m5ioe1_ns.class_("AW9737ASwitch", cg.Component, switch.Switch)

CONFIG_SCHEMA = switch.switch_schema(
    AW9737ASwitch,
    device_class=DEVICE_CLASS_SWITCH,
    icon=ICON_PULSE,
    default_restore_mode="RESTORE_DEFAULT_OFF"
).extend(
    {
        cv.Required(CONF_PIN) : cv.int_range(min=0, max=13),
        cv.Required(CONF_PULSE_NUM) : cv.int_range(min=0, max=3) 
    }
).extend(BASE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_M5IOE1_ID])
    await switch.new_switch(var, config)
    cg.add(var.set_pin(config[CONF_PIN]))
    cg.add(var.set_pulse_num(config[CONF_PULSE_NUM]))
