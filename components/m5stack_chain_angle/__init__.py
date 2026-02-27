import esphome.codegen as cg
from esphome.components import sensor, uart

CODEOWNERS = ["@Jasionf"]

m5stack_chain_angle_ns = cg.esphome_ns.namespace("m5stack_chain_angle")
ChainAngleSensor = m5stack_chain_angle_ns.class_(
    "ChainAngleSensor",
    sensor.Sensor,
    cg.PollingComponent,
    uart.UARTDevice,
)

