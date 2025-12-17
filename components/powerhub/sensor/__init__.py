import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

from esphome.const import (
    CONF_ID, 
    CONF_BATTERY_LEVEL,
    ICON_BATTERY,
    ICON_FLASH,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_POWER,
    STATE_CLASS_MEASUREMENT,
    UNIT_PERCENT,
    UNIT_MILLIVOLT,
    UNIT_MILLIAMP
)

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_BATTERY_CURRENT = "battery_current"

CONF_GROVE_RED_VOLTAGE = "grove_red_voltage"
CONF_GROVE_RED_CURRENT = "grove_red_current"

CONF_GROVE_BLUE_VOLTAGE = "grove_blue_voltage"
CONF_GROVE_BLUE_CURRENT = "grove_blue_current"

CONF_CAN_VOLTAGE = "can_voltage"
CONF_CAN_CURRENT = "can_current"

CONF_RS485_VOLTAGE = "rs485_voltage"
CONF_RS485_CURRENT = "rs485_current"

CONF_USB_VOLTAGE = "usb_voltage"
CONF_USB_CURRENT = "usb_current"

ICON_CURRENT_DC = "mdi:current-dc"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_BATTERY,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_BATTERY_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_BATTERY,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_BATTERY,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_GROVE_RED_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_GROVE_RED_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_CURRENT_DC,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_GROVE_BLUE_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_GROVE_BLUE_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_CURRENT_DC,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_CAN_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CAN_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_CURRENT_DC,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_RS485_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_RS485_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_CURRENT_DC,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),

            cv.Optional(CONF_USB_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIVOLT,
                icon=ICON_FLASH,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_USB_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIAMP,
                icon=ICON_CURRENT_DC,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(BASE_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)



async def to_code(config):
    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(powerhub.set_battery_level_sensor(sens))
    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(powerhub.set_battery_voltage_sensor(sens))
    if CONF_BATTERY_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_CURRENT])
        cg.add(powerhub.set_battery_current_sensor(sens))

    if CONF_GROVE_RED_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_GROVE_RED_VOLTAGE])
        cg.add(powerhub.set_grove_red_voltage_sensor(sens))
    if CONF_GROVE_RED_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_GROVE_RED_CURRENT])
        cg.add(powerhub.set_grove_red_current_sensor(sens))

    if CONF_GROVE_BLUE_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_GROVE_BLUE_VOLTAGE])
        cg.add(powerhub.set_grove_blue_voltage_sensor(sens))
    if CONF_GROVE_BLUE_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_GROVE_BLUE_CURRENT])
        cg.add(powerhub.set_grove_blue_current_sensor(sens))

    if CONF_CAN_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_CAN_VOLTAGE])
        cg.add(powerhub.set_can_voltage_sensor(sens))
    if CONF_CAN_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_CAN_CURRENT])
        cg.add(powerhub.set_can_current_sensor(sens))

    if CONF_RS485_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_RS485_VOLTAGE])
        cg.add(powerhub.set_rs485_voltage_sensor(sens))
    if CONF_RS485_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_RS485_CURRENT])
        cg.add(powerhub.set_rs485_current_sensor(sens))

    if CONF_USB_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_USB_VOLTAGE])
        cg.add(powerhub.set_usb_voltage_sensor(sens))
    if CONF_USB_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_USB_CURRENT])
        cg.add(powerhub.set_usb_current_sensor(sens))
        