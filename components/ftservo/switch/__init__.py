import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_ID
from .. import ftservo_ns, CONF_FTSERVO_ID, FTSERVO_BASE_SCHEMA


CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["ftservo"]

ICON_LOCK = "mdi:lock"

ServoTorqueSwitch = ftservo_ns.class_(
    "ServoTorqueSwitch",
    switch.Switch,
    cg.Component,
)
CONFIG_SCHEMA = (
    switch.switch_schema(
        ServoTorqueSwitch,
        icon="mdi:lock",
        default_restore_mode="RESTORE_DEFAULT_OFF"
    )
    .extend(FTSERVO_BASE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
)
async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
    
    parent = await cg.get_variable(config[CONF_FTSERVO_ID])
    cg.add(var.set_parent(parent))
    # cg.add(parent.register_torque_switch(var))