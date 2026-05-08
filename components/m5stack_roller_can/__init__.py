import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import canbus
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

CODEOWNERS = ["@ItsRebaseTime"]
DEPENDENCIES = ["m5stack_roller", "canbus"]
MULTI_CONF = True

m5stack_roller_ns = cg.esphome_ns.namespace("m5stack_roller")
RollerBase = m5stack_roller_ns.class_("RollerBase")

m5stack_roller_can_ns = cg.esphome_ns.namespace("m5stack_roller_can")
# RollerMotorCAN inherits from RollerBase (C++) and PollingComponent
RollerMotorCAN = m5stack_roller_can_ns.class_("RollerMotorCAN", RollerBase, cg.PollingComponent)
# RollerCANReceiver inherits from CanbusTrigger (which is also a Component)
RollerCANReceiver = m5stack_roller_can_ns.class_("RollerCANReceiver", canbus.CanbusTrigger)

CONF_CANBUS_ID = "canbus_id"
CONF_MOTOR_ID = "motor_id"
CONF_HOST_ID = "host_id"
CONF_TRIGGER_ID = "trigger_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RollerMotorCAN),
            cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(RollerCANReceiver),
            cv.GenerateID(CONF_CANBUS_ID): cv.use_id(canbus.CanbusComponent),
            cv.Required(CONF_MOTOR_ID): cv.int_range(min=1, max=255),
            cv.Optional(CONF_HOST_ID, default=0): cv.int_range(min=0, max=255),
        }
    )
    .extend(cv.polling_component_schema("500ms"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_motor_id(config[CONF_MOTOR_ID]))
    cg.add(var.set_host_id(config[CONF_HOST_ID]))

    canbus_var = await cg.get_variable(config[CONF_CANBUS_ID])
    cg.add(var.set_canbus(canbus_var))

    # Create a trigger that catches all extended frames and routes to this motor
    receiver = cg.new_Pvariable(config[CONF_TRIGGER_ID], canbus_var, var)
    await cg.register_component(receiver, {})
