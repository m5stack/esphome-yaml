import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor

from . import ChainJoystickSensor

DEPENDENCIES = ["uart"]

CONF_CHAIN_ID = "chain_id"
CONF_AXIS = "axis"


AXIS_OPTIONS = {
    "x": 0,
    "y": 1,
}


CONFIG_SCHEMA = (
    sensor.sensor_schema(
        ChainJoystickSensor,
        icon="mdi:axis-arrow",
    )
    .extend(
        {
            cv.Optional(CONF_CHAIN_ID, default=1): cv.int_range(min=1, max=255),
            cv.Optional(CONF_AXIS, default="x"): cv.one_of(*AXIS_OPTIONS.keys(), lower=True),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.polling_component_schema("50ms"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    cg.add(var.set_device_id(config[CONF_CHAIN_ID]))
    cg.add(var.set_axis(AXIS_OPTIONS[config[CONF_AXIS]]))
