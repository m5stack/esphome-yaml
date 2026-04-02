from esphome import automation
from esphome.automation import maybe_simple_id
import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import (
    CONF_ID, 
    CONF_TRIGGER_ID,
    CONF_POSITION,
    CONF_SPEED,
    CONF_TIME,
    CONF_ON_ERROR,
    CONF_INVERTED
)

from esphome.core import CORE
from esphome.coroutine import CoroPriority, coroutine_with_priority

IS_PLATFORM_COMPONENT = True

ftservo_ns = cg.esphome_ns.namespace("ftservo")
FTServo = ftservo_ns.class_("FTServo")
FTServoSource = ftservo_ns.class_("FTServoSource")

CONF_ON_MOVE = "on_move"
CONF_FTSERVO = "ftservo"
CONF_FTSERVO_ID = "ftservo_id" # ftservo component id
CONF_MIN_POSITION = "min_position"
CONF_MAX_POSITION = "max_position"

# Move Actions
MoveAction = ftservo_ns.class_(
    "MoveAction", automation.Action, cg.Parented.template(FTServo)
)

MoveTrigger = ftservo_ns.class_(
    "MoveTrigger",
    automation.Trigger.template()
)

IsMovingCondition = ftservo_ns.class_(
    "IsMovingCondition", automation.Condition
)


ErrorTrigger = ftservo_ns.class_(
    "ErrorTrigger",
    automation.Trigger.template()
)

async def setup_ftservo_core_(var, config):
    for conf in config.get(CONF_ON_MOVE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger,
            [],
            conf,
        )

    for conf in config.get(CONF_ON_ERROR, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(
            trigger,
            [(cg.std_string_ref, "x")],
            conf,
        )

async def register_ftservo(var, config):
    if not CORE.has_id(config[CONF_ID]):
        var = cg.Pvariable(config[CONF_ID], var)
    await setup_ftservo_core_(var, config)

FTSERVO_BASE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_FTSERVO_ID) : cv.use_id(FTServo),
    }
)

FTSERVO_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ON_MOVE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(MoveTrigger),
            }
        ),
        cv.Optional(CONF_ON_ERROR): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(ErrorTrigger),
            }
        ),
    }
)

FTSERVO_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(FTServo),
        cv.Required(CONF_POSITION): cv.templatable(cv.int_),
        cv.Optional(CONF_TIME, default=0) : cv.templatable(cv.int_),
        cv.Optional(CONF_SPEED, default=500): cv.templatable(cv.int_)
    }
)

FTSERVO_CONDITION_SCHEMA = maybe_simple_id({
    cv.GenerateID(): cv.use_id(FTServo),
})

async def ftservo_action(config, action_id, template_arg, arg):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    template_ = await cg.templatable(config[CONF_POSITION], arg, int)
    cg.add(var.set_position(template_))

    if CONF_TIME in config:
        template_ = await cg.templatable(config[CONF_TIME], arg, int)
        cg.add(var.set_time(template_))
    if CONF_SPEED in config:
        template_ = await cg.templatable(config[CONF_SPEED], arg, int)
        cg.add(var.set_speed(template_))
    return var


async def ftservo_condition(config, condition_id, template_arg, arg):
    var = cg.new_Pvariable(condition_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

# def servo_source_schema():
#     return cv.All(
#         automation.maybe_conf(
#             CONF_FTSERVO,
#             {
#                 cv.GenerateID(CONF_ID): cv.declare_id(FTServoSource),
#                 cv.GenerateID(CONF_FTSERVO): cv.use_id(FTServo),
#                 cv.Optional(CONF_MIN_POSITION): cv.int_,
#                 cv.Optional(CONF_MAX_POSITION): cv.int_,
#                 cv.Optional(CONF_INVERTED, default=False): cv.boolean,
#             },
#         ),
#     )



# async def servo_source_to_code(config):
#     servo = await cg.get_variable(config[CONF_FTSERVO])

#     var = cg.new_Pvariable(
#         config[CONF_ID],
#         servo,
#     )

#     if CONF_MIN_POSITION in config:
#         cg.add(var.set_min_position(config[CONF_MIN_POSITION]))

#     if CONF_MAX_POSITION in config:
#         cg.add(var.set_max_position(config[CONF_MAX_POSITION]))

#     if CONF_INVERTED in config:
#         cg.add(var.set_inverted(config[CONF_INVERTED]))

#     return var

automation.register_action(
    "ftservo.move",
    MoveAction,
    FTSERVO_ACTION_SCHEMA,
    synchronous=True
)(ftservo_action)


automation.register_condition(
    "ftservo.is_moving",
    IsMovingCondition, 
    FTSERVO_CONDITION_SCHEMA
)(ftservo_condition)

@coroutine_with_priority(CoroPriority.CORE)
async def to_code(config):
    cg.add_global(ftservo_ns.using)
