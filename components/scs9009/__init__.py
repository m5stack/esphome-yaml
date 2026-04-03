from esphome.components import uart
import esphome.codegen as cg
import esphome.config_validation as cv
from .. import ftservo
from esphome.const import (
    CONF_ID,

)

DEPENDENCIES = ["uart"]

CONF_SCS9009_ID = "scs9009_id"

scs9009_ns = cg.esphome_ns.namespace("scs9009")
SCS9009Component = scs9009_ns.class_(
    "SCS9009Component", cg.Component, uart.UARTDevice
)


BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SCS9009_ID): cv.use_id(SCS9009Component),
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SCS9009Component),
    }
).extend(uart.UART_DEVICE_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)