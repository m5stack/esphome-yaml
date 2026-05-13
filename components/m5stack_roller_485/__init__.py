import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

CODEOWNERS = ["@ItsRebaseTime"]
DEPENDENCIES = []
MULTI_CONF = True

m5stack_roller_ns = cg.esphome_ns.namespace("m5stack_roller")
RollerBase = m5stack_roller_ns.class_("RollerBase")

m5stack_roller_485_ns = cg.esphome_ns.namespace("m5stack_roller_485")
RollerMotor485 = m5stack_roller_485_ns.class_("RollerMotor485", RollerBase, cg.Component)

CONF_UART_MULTI_ID = "uart_multi_id"
CONF_MOTOR_ID = "motor_id"
CONF_HOST_ID = "host_id"

# Import uart_multi references without importing the Python module directly
uart_multi_ns = cg.esphome_ns.namespace("uart_multi")
UARTMulti = uart_multi_ns.class_("UARTMulti")

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(RollerMotor485),
        cv.GenerateID(CONF_UART_MULTI_ID): cv.use_id(UARTMulti),
        cv.Required(CONF_MOTOR_ID): cv.int_range(min=1, max=255),
        cv.Optional(CONF_HOST_ID, default=0): cv.int_range(min=0, max=255),
        cv.Optional(CONF_UPDATE_INTERVAL, default="500ms"): cv.update_interval,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_motor_id(config[CONF_MOTOR_ID]))
    cg.add(var.set_host_id(config[CONF_HOST_ID]))
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))

    parent = await cg.get_variable(config[CONF_UART_MULTI_ID])
    cg.add(var.set_parent(parent))
    cg.add(parent.register_device(var))
