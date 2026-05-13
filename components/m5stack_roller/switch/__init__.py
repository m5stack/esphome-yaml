import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = []
CODEOWNERS = ["@ItsRebaseTime"]

CONF_MOTOR_OUTPUT = "motor_output"
CONF_STALL_PROTECTION = "stall_protection"
CONF_OVER_RANGE_PROTECTION = "over_range_protection"
CONF_BUTTON_SWITCHING_MODE = "button_switching_mode"

RollerMotorOutputSwitch = m5stack_roller_ns.class_("RollerMotorOutputSwitch", switch.Switch, cg.Component)
RollerStallProtectionSwitch = m5stack_roller_ns.class_("RollerStallProtectionSwitch", switch.Switch, cg.Component)
RollerOverRangeProtectionSwitch = m5stack_roller_ns.class_("RollerOverRangeProtectionSwitch", switch.Switch, cg.Component)
RollerButtonSwitchingModeSwitch = m5stack_roller_ns.class_("RollerButtonSwitchingModeSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_MOTOR_OUTPUT): switch.switch_schema(RollerMotorOutputSwitch).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_STALL_PROTECTION): switch.switch_schema(RollerStallProtectionSwitch).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_OVER_RANGE_PROTECTION): switch.switch_schema(RollerOverRangeProtectionSwitch).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_BUTTON_SWITCHING_MODE): switch.switch_schema(RollerButtonSwitchingModeSwitch).extend(cv.COMPONENT_SCHEMA),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_MOTOR_OUTPUT in config:
        sw = cg.new_Pvariable(config[CONF_MOTOR_OUTPUT][CONF_ID])
        await cg.register_component(sw, config[CONF_MOTOR_OUTPUT])
        await switch.register_switch(sw, config[CONF_MOTOR_OUTPUT])
        cg.add(sw.set_parent(parent))

    if CONF_STALL_PROTECTION in config:
        sw = cg.new_Pvariable(config[CONF_STALL_PROTECTION][CONF_ID])
        await cg.register_component(sw, config[CONF_STALL_PROTECTION])
        await switch.register_switch(sw, config[CONF_STALL_PROTECTION])
        cg.add(sw.set_parent(parent))

    if CONF_OVER_RANGE_PROTECTION in config:
        sw = cg.new_Pvariable(config[CONF_OVER_RANGE_PROTECTION][CONF_ID])
        await cg.register_component(sw, config[CONF_OVER_RANGE_PROTECTION])
        await switch.register_switch(sw, config[CONF_OVER_RANGE_PROTECTION])
        cg.add(sw.set_parent(parent))

    if CONF_BUTTON_SWITCHING_MODE in config:
        sw = cg.new_Pvariable(config[CONF_BUTTON_SWITCHING_MODE][CONF_ID])
        await cg.register_component(sw, config[CONF_BUTTON_SWITCHING_MODE])
        await switch.register_switch(sw, config[CONF_BUTTON_SWITCHING_MODE])
        cg.add(sw.set_parent(parent))
