import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import CONF_ID
from .. import m5stack_roller_ns, ROLLER_BASE_SCHEMA, CONF_ROLLER_BASE_ID

DEPENDENCIES = ["m5stack_roller"]
CODEOWNERS = ["@ItsRebaseTime"]

CONF_SAVE_TO_FLASH = "save_to_flash"
CONF_RELEASE_STALL_PROTECTION = "release_stall_protection"

RollerSaveToFlashButton = m5stack_roller_ns.class_("RollerSaveToFlashButton", button.Button, cg.Component)
RollerReleaseStallButton = m5stack_roller_ns.class_("RollerReleaseStallButton", button.Button, cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(cg.Component),
        cv.Optional(CONF_SAVE_TO_FLASH): button.button_schema(
            RollerSaveToFlashButton, icon="mdi:content-save"
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RELEASE_STALL_PROTECTION): button.button_schema(
            RollerReleaseStallButton, icon="mdi:lock-open"
        ).extend(cv.COMPONENT_SCHEMA),
    }
).extend(ROLLER_BASE_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_ROLLER_BASE_ID])

    if CONF_SAVE_TO_FLASH in config:
        btn = cg.new_Pvariable(config[CONF_SAVE_TO_FLASH][CONF_ID])
        await cg.register_component(btn, config[CONF_SAVE_TO_FLASH])
        await button.register_button(btn, config[CONF_SAVE_TO_FLASH])
        cg.add(btn.set_parent(parent))

    if CONF_RELEASE_STALL_PROTECTION in config:
        btn = cg.new_Pvariable(config[CONF_RELEASE_STALL_PROTECTION][CONF_ID])
        await cg.register_component(btn, config[CONF_RELEASE_STALL_PROTECTION])
        await button.register_button(btn, config[CONF_RELEASE_STALL_PROTECTION])
        cg.add(btn.set_parent(parent))
