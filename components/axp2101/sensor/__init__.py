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
    ENTITY_CATEGORY_DIAGNOSTIC,
    DEVICE_CLASS_BATTERY_CHARGING,
    
)

from .. import axp2101_ns, BASE_SCHEMA, CONF_AXP2101_ID

DEPENDENCIES = ["axp2101"]

CONF_AXP_TEMPERATURE = "axp_temperature"
CONF_BATTERY_CHARGING = "battery_charging"

AXP2101Sensor = axp2101_ns.class_("AXP2101Sensor", sensor.Sensor, cg.PollingComponent)


CONFIG_SCHEMA = (
    sensor.sensor_schema(AXP2101Sensor)
    .extend(
        {
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_BATTERY,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_AXP_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BATTERY_VOLTAGE): sensor.sensor_schema(
                device_class=DEVICE_CLASS_VOLTAGE,
                accuracy_decimals=3,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
            cv.Optional(CONF_BATTERY_CHARGING): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_BATTERY_CHARGING,
                icon="mdi:battery-charging",
            ),
        }
    )
    .extend(BASE_SCHEMA)
    .extend(cv.polling_component_schema("60s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_parented(var, config[CONF_AXP2101_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    if CONF_BATTERY_LEVEL in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_LEVEL])
        cg.add(var.set_battery_level_sensor(sens))
    
    if CONF_AXP_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_AXP_TEMPERATURE])
        cg.add(var.set_axp_temperature_sensor(sens))
    
    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_battery_voltage_sensor(sens))
    
    if CONF_BATTERY_CHARGING in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BATTERY_CHARGING])
        cg.add(var.set_battery_charging_binary_sensor(sens))

    
    
