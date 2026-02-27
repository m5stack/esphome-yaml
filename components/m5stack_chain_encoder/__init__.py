import esphome.codegen as cg
from esphome.components import sensor, uart

CODEOWNERS = ["@Jasionf"]

m5stack_chain_encoder_ns = cg.esphome_ns.namespace("m5stack_chain_encoder")
ChainEncoderSensor = m5stack_chain_encoder_ns.class_(
    "ChainEncoderSensor",
    sensor.Sensor,
    cg.PollingComponent,
    uart.UARTDevice,
)
