import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome.const import (
    CONF_ID, 
    CONF_BATTERY_LEVEL, 
    UNIT_PERCENT,
    UNIT_CELSIUS,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    ICON_BATTERY,
    ICON_THERMOMETER,
    CONF_BATTERY_VOLTAGE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    ENTITY_CATEGORY_DIAGNOSTIC,
    DEVICE_CLASS_BATTERY_CHARGING,
    
)

from .. import axp192_ns, BASE_SCHEMA, CONF_AXP192_ID

DEPENDENCIES = ["axp192"]

CONF_USB_PRESENT = "usb_present"
CONF_USB_VALID = "usb_valid"
CONF_VBUS_PRESENT = "vbus_present"
CONF_VBUS_VALID = "vbus_valid"
CONF_BATTERY_DISCHARGING = "battery_discharging"
CONF_OVER_TEMP = "over_temperature"
CONF_BATTERY_CHARGING = "battery_charging"
CONF_BATTERY_CONNECTED = "battery_connected"
CONF_BATTERY_ACTIVE_MODE = "battery_in_active_mode"
CONF_LOW_CHARGE_CURRENT = "low_charge_current"
CONF_USB_VOLTAGE = "usb_voltage"
CONF_USB_CURRENT = "usb_current"
CONF_VBUS_VOLTAGE = "vbus_voltage"
CONF_VBUS_CURRENT = "vbus_current"
CONF_BATTERY_POWER = "battery_power"
CONF_INTERNAL_TEMPERATURE = "internal_temperature"
CONF_BATTERY_CHARGE_CURRENT = "battery_charge_current"
CONF_BATTERY_DISCHARGE_CURRENT = "battery_discharge_current"
CONF_APS_VOLTAGE = "aps_voltage"
CONF_BATTERY_DISCHARGE_100PC_VOLTAGE = "battery_discharge_100pc_voltage"
CONF_BATTERY_DISCHARGE_0PC_VOLTAGE = "battery_discharge_0pc_voltage"
CONF_BATTERY_CHARGE_100PC_CURRENT = "battery_charge_100pc_current"
CONF_BATTERY_CHARGE_0PC_CURRENT = "battery_charge_0pc_current"

AXP192Sensor = axp192_ns.class_("AXP192Sensor", sensor.Sensor, cg.PollingComponent)


CONFIG_SCHEMA = (
    sensor.sensor_schema(AXP192Sensor)
    .extend(
        {
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_BATTERY,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            )
            .extend({
                cv.Optional(CONF_BATTERY_DISCHARGE_0PC_VOLTAGE, default=3000): cv.uint16_t,
            #     cv.Optional(CONF_BATTERY_DISCHARGE_100PC_VOLTAGE, default=3700): cv.uint16_t,
            #     cv.Optional(CONF_BATTERY_CHARGE_0PC_CURRENT, default=430): cv.uint16_t,
            #     cv.Optional(CONF_BATTERY_CHARGE_100PC_CURRENT, default=100): cv.uint16_t,
            })
            ,
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement='V',
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_USB_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement='V',
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_USB_CURRENT): sensor.sensor_schema(
                unit_of_measurement='A',
                device_class=DEVICE_CLASS_CURRENT,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
           cv.Optional(CONF_VBUS_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement='V',
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_VBUS_CURRENT): sensor.sensor_schema(
                unit_of_measurement='A',
                device_class=DEVICE_CLASS_CURRENT,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_INTERNAL_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                icon=ICON_THERMOMETER,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BATTERY_POWER): sensor.sensor_schema(
                unit_of_measurement='W',
                device_class=DEVICE_CLASS_POWER,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_BATTERY_CHARGE_CURRENT): sensor.sensor_schema(
                unit_of_measurement='A',
                device_class=DEVICE_CLASS_CURRENT,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_BATTERY_DISCHARGE_CURRENT): sensor.sensor_schema(
                unit_of_measurement='A',
                device_class=DEVICE_CLASS_CURRENT,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_APS_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement='V',
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=2,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_BATTERY_CHARGING): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_BATTERY_CHARGING,
                icon="mdi:battery-charging",
            ),
            cv.Optional(CONF_USB_PRESENT): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_USB_VALID): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_VBUS_PRESENT): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_VBUS_VALID): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_BATTERY_DISCHARGING): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_OVER_TEMP): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_BATTERY_CONNECTED): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_BATTERY_ACTIVE_MODE): binary_sensor.binary_sensor_schema(
            ),
            cv.Optional(CONF_LOW_CHARGE_CURRENT): binary_sensor.binary_sensor_schema(
            ),
        }
    )
    .extend(BASE_SCHEMA)
    .extend(
        {
            cv.Optional(CONF_BATTERY_DISCHARGE_0PC_VOLTAGE, default=3200): cv.uint16_t,
            cv.Optional(CONF_BATTERY_DISCHARGE_100PC_VOLTAGE, default=3680): cv.uint16_t,
            cv.Optional(CONF_BATTERY_CHARGE_0PC_CURRENT, default=430): cv.uint16_t,
            cv.Optional(CONF_BATTERY_CHARGE_100PC_CURRENT, default=60): cv.uint16_t,
        }
    )
    .extend(cv.polling_component_schema("60s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AXP192_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))
        cg.add(var.set_battery_discharge_0pc_voltage(config[CONF_BATTERY_DISCHARGE_0PC_VOLTAGE]))
        cg.add(var.set_battery_discharge_100pc_voltage(config[CONF_BATTERY_DISCHARGE_100PC_VOLTAGE]))
        cg.add(var.set_battery_charge_0pc_current(config[CONF_BATTERY_CHARGE_0PC_CURRENT]))
        cg.add(var.set_battery_charge_100pc_current(config[CONF_BATTERY_CHARGE_100PC_CURRENT]))
 
    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))

    if CONF_USB_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_USB_VOLTAGE])
        cg.add(var.set_usb_voltage_sensor(sens))

    if CONF_USB_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_USB_CURRENT])
        cg.add(var.set_usb_current_sensor(sens))

    if CONF_VBUS_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VBUS_VOLTAGE])
        cg.add(var.set_vbus_voltage_sensor(sens))

    if CONF_VBUS_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_VBUS_CURRENT])
        cg.add(var.set_vbus_current_sensor(sens))

    if CONF_INTERNAL_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_INTERNAL_TEMPERATURE])
        cg.add(var.set_internal_temperature_sensor(sens))
    
    if CONF_BATTERY_POWER in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_POWER])
        cg.add(var.set_battery_power_sensor(sens))
    
    if CONF_BATTERY_CHARGE_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_CHARGE_CURRENT])
        cg.add(var.set_battery_charge_current_sensor(sens))
    
    if CONF_BATTERY_DISCHARGE_CURRENT in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_DISCHARGE_CURRENT])
        cg.add(var.set_battery_discharge_current_sensor(sens))

    if CONF_APS_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_APS_VOLTAGE])
        cg.add(var.set_aps_voltage_sensor(sens))

    if CONF_BATTERY_CHARGING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BATTERY_CHARGING])
        cg.add(var.set_battery_charging_binary_sensor(sens))

    if CONF_USB_PRESENT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_USB_PRESENT])
        cg.add(var.set_usb_present_binary_sensor(sens))

    if CONF_USB_VALID in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_USB_VALID])
        cg.add(var.set_usb_valid_binary_sensor(sens))

    if CONF_VBUS_PRESENT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_VBUS_PRESENT])
        cg.add(var.set_vbus_present_binary_sensor(sens))

    if CONF_VBUS_VALID in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_VBUS_VALID])
        cg.add(var.set_vbus_valid_binary_sensor(sens))

    if CONF_BATTERY_DISCHARGING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BATTERY_DISCHARGING])
        cg.add(var.set_battery_discharging_binary_sensor(sens))

    if CONF_OVER_TEMP in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_OVER_TEMP])
        cg.add(var.set_over_temp_binary_sensor(sens))

    if CONF_BATTERY_CONNECTED in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BATTERY_CONNECTED])
        cg.add(var.set_battery_connected_binary_sensor(sens))

    if CONF_BATTERY_ACTIVE_MODE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BATTERY_ACTIVE_MODE])
        cg.add(var.set_battery_active_mode_binary_sensor(sens))

    if CONF_LOW_CHARGE_CURRENT in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_LOW_CHARGE_CURRENT])
        cg.add(var.set_low_charge_current_binary_sensor(sens))

    
    
