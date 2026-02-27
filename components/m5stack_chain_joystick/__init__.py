import esphome.codegen as cg
from esphome.components import sensor, uart

CODEOWNERS = ["@Jasionf"]

m5stack_chain_joystick_ns = cg.esphome_ns.namespace("m5stack_chain_joystick")
ChainJoystickSensor = m5stack_chain_joystick_ns.class_(
    "ChainJoystickSensor",
    sensor.Sensor,
    cg.PollingComponent,
    uart.UARTDevice,
)
