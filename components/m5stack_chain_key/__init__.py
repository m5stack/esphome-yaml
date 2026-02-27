import esphome.codegen as cg
from esphome.components import binary_sensor, uart

CODEOWNERS = ["@Jasionf"]

m5stack_chain_key_ns = cg.esphome_ns.namespace("m5stack_chain_key")
ChainKeyBinarySensor = m5stack_chain_key_ns.class_(
    "ChainKeyBinarySensor",
    binary_sensor.BinarySensor,
    cg.PollingComponent,
    uart.UARTDevice,
)
