import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number

from esphome.const import (
    ICON_FLASH,
    ENTITY_CATEGORY_CONFIG,
    UNIT_MILLIAMP,
    UNIT_MILLIVOLT,
    DEVICE_CLASS_POWER
)

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

CONF_RS485_CAN_OUTPUT_VOLTAGE = "rs485_can_output_voltage"
CONF_RS485_CAN_CURRENT_LIMIT  = "rs485_can_current_limit"


RS485CANVoltageNumber = powerhub_ns.class_("RS485CANOutputVoltageNumber", number.Number)
RS485CANCurrentNumber = powerhub_ns.class_("RS485CANOutputCurrentNumber", number.Number)

ICON_CURRENT_DC = "mdi:current-dc"


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_RS485_CAN_OUTPUT_VOLTAGE) : number.number_schema(
            RS485CANVoltageNumber,
            icon=ICON_FLASH,
            device_class=DEVICE_CLASS_POWER,
            unit_of_measurement=UNIT_MILLIVOLT,
            entity_category=ENTITY_CATEGORY_CONFIG
        ),
        cv.Optional(CONF_RS485_CAN_CURRENT_LIMIT) : number.number_schema(
            RS485CANCurrentNumber,
            icon=ICON_CURRENT_DC,
            device_class=DEVICE_CLASS_POWER,
            unit_of_measurement=UNIT_MILLIAMP,
            entity_category=ENTITY_CATEGORY_CONFIG
        ),
    }
).extend(BASE_SCHEMA)

async def to_code(config):
    
    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    if CONF_RS485_CAN_OUTPUT_VOLTAGE in config:
        var = await number.new_number(
            config[CONF_RS485_CAN_OUTPUT_VOLTAGE],
            min_value=3000.0,
            max_value=20000.0,
            step=20.0
        )
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_rs485_can_output_voltage_number(var))
        cg.add(var.publish_state(3000.0))
    
    if CONF_RS485_CAN_CURRENT_LIMIT in config:
        var = await number.new_number(
            config[CONF_RS485_CAN_CURRENT_LIMIT],
            min_value=13.0,
            max_value=3000.0,
            step=20.0
        )
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_rs485_can_current_limit_number(var))
        cg.add(var.publish_state(13.0))