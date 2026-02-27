import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor

from . import ChainKeyBinarySensor

DEPENDENCIES = ["uart"]

CONF_CHAIN_ID = "chain_id"


CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(
        ChainKeyBinarySensor,
        icon="mdi:gesture-tap",
    )
    .extend(
        {
            cv.Optional(CONF_CHAIN_ID, default=1): cv.int_range(min=1, max=255),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("50ms"))
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_device_id(config[CONF_CHAIN_ID]))
