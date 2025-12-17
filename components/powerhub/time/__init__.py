import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import time
from esphome import automation

from esphome.const import (
    CONF_ID
)

from .. import powerhub_ns, CONF_POWER_HUB_ID, BASE_SCHEMA

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]


CONF_POWERHUB_TIME = "powerhub_time"

PowerHubTime = powerhub_ns.class_("PowerHubTime", time.RealTimeClock)
WriteAction = powerhub_ns.class_("WriteAction", automation.Action)
ReadAction = powerhub_ns.class_("ReadAction", automation.Action)


CONFIG_SCHEMA = time.TIME_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(PowerHubTime),
    }
).extend(BASE_SCHEMA)


@automation.register_action(
    "powerhub.write_time",
    WriteAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(PowerHubTime),
        }
    ),
)
async def powerhub_write_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

@automation.register_action(
    "powerhub.read_time",
    ReadAction,
    automation.maybe_simple_id(
        {
            cv.GenerateID(): cv.use_id(PowerHubTime),
        }
    ),
)
async def powerhub_read_time_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_POWER_HUB_ID])