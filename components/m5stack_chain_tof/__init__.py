import esphome.codegen as cg
from esphome.components import sensor, uart

CODEOWNERS = ["@Jasionf"]

m5stack_chain_tof_ns = cg.esphome_ns.namespace("m5stack_chain_tof")
ChainToFSensor = m5stack_chain_tof_ns.class_(
    "ChainToFSensor",
    sensor.Sensor,
    cg.PollingComponent,
    uart.UARTDevice,
)
