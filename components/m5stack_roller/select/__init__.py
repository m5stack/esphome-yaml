import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = ["m5stack_roller"]
CODEOWNERS = ["@ItsRebaseTime"]

CONF_MODE = "mode"

ROLLER_MODES = ["Speed", "Position", "Current", "Encoder"]

RollerModeSelect = m5stack_roller_ns.class_("RollerModeSelect", select.Select, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_MODE): select.select_schema(RollerModeSelect).extend(cv.COMPONENT_SCHEMA),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_MODE in config:
        sel = cg.new_Pvariable(config[CONF_MODE][CONF_ID])
        await cg.register_component(sel, config[CONF_MODE])
        await select.register_select(sel, config[CONF_MODE], options=ROLLER_MODES)
        cg.add(sel.set_parent(parent))
