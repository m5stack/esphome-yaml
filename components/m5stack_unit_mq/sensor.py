import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor, binary_sensor
from esphome.const import (
    CONF_TEMPERATURE,
    UNIT_CELSIUS,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    DEVICE_CLASS_VOLTAGE,
    UNIT_MILLIVOLT,
    UNIT_OHM,
    UNIT_EMPTY,
    DEVICE_CLASS_EMPTY,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_FLASH,
    DEVICE_CLASS_LIGHT,
    CONF_HEAT_MODE,
    CONF_LED,
    CONF_ID
)

DEPENDENCIES = ["i2c"]

AUTO_LOAD = ["binary_sensor"]

CODEOWNERS = ["@m5stack"]


unit_mq_ns = cg.esphome_ns.namespace("unit_mq")
UnitMQSensor = unit_mq_ns.class_(
    "UnitMQSensor", cg.PollingComponent, 
                       i2c.I2CDevice)


HeatMode = unit_mq_ns.enum("HeatMode", is_class=True)

HEAT_MODES = {
    "OFF": HeatMode.OFF,
    "CONTINUOUS": HeatMode.CONTINUOUS,
    "SWITCH": HeatMode.SWITCH,
}


CONF_HIGH_LEVEL_TIME = "high_level_time"
CONF_LOW_LEVEL_TIME = "low_level_time"
CONF_REFERENCE_VOLTAGE = "reference_voltage"
CONF_MQ_VOLTAGE = "mq_voltage"
CONF_NTC_VOLTAGE = "ntc_voltage"
CONF_MQ_ADC = "mq_adc"
CONF_NTC_ADC = "ntc_adc"
CONF_PARTS_PER_MILLION = "parts_per_million"
CONF_NTC_RESISTANCE = "ntc_resistance"

ICON_LED = "mdi:alarm-light"
ICON_OMEGA = "mdi:omega"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(UnitMQSensor),

            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_REFERENCE_VOLTAGE): sensor.sensor_schema(
                icon=ICON_FLASH,
                unit_of_measurement=UNIT_MILLIVOLT,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MQ_VOLTAGE): sensor.sensor_schema(
                icon=ICON_FLASH,
                unit_of_measurement=UNIT_MILLIVOLT,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NTC_VOLTAGE): sensor.sensor_schema(
                icon=ICON_FLASH,
                unit_of_measurement=UNIT_MILLIVOLT,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NTC_RESISTANCE): sensor.sensor_schema(
                icon=ICON_OMEGA,
                unit_of_measurement=UNIT_OHM,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                device_class=DEVICE_CLASS_EMPTY,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_MQ_ADC): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=0,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_NTC_ADC): sensor.sensor_schema(
                unit_of_measurement=UNIT_EMPTY,
                accuracy_decimals=0,
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_LED): binary_sensor.binary_sensor_schema(
                icon=ICON_LED,
                device_class=DEVICE_CLASS_LIGHT
            ),
            cv.Optional(CONF_HEAT_MODE): cv.enum(HEAT_MODES, upper=True, space="_"),
            cv.Optional(CONF_HIGH_LEVEL_TIME, default=30): cv.All(
                cv.uint8_t,
                cv.Range(min=30),
            ),
            cv.Optional(CONF_LOW_LEVEL_TIME, default=5): cv.All(
                cv.uint8_t,
                cv.Range(min=5),
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x11))  # default address of Unit MQ
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if temperature := config.get(CONF_TEMPERATURE):
        sens = await sensor.new_sensor(temperature)
        cg.add(var.set_temperature_sensor(sens))
    if ref_volt := config.get(CONF_REFERENCE_VOLTAGE):
        sens = await sensor.new_sensor(ref_volt)
        cg.add(var.set_reference_voltage_sensor(sens))
    
    if mq_volt := config.get(CONF_MQ_VOLTAGE):
        sens = await sensor.new_sensor(mq_volt)
        cg.add(var.set_mq_voltage_sensor(sens))

    if ntc_volt := config.get(CONF_NTC_VOLTAGE):
        sens = await sensor.new_sensor(ntc_volt)
        cg.add(var.set_ntc_voltage_sensor(sens))

    if ntc_res := config.get(CONF_NTC_RESISTANCE):
        sens = await sensor.new_sensor(ntc_res)
        cg.add(var.set_ntc_resistance_sensor(sens))

    if mq_adc := config.get(CONF_MQ_ADC):
        sens = await sensor.new_sensor(mq_adc)
        cg.add(var.set_mq_adc_sensor(sens))

    if ntc_adc := config.get(CONF_NTC_ADC):
        sens = await sensor.new_sensor(ntc_adc)
        cg.add(var.set_ntc_adc_sensor(sens))

    if CONF_LED in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_LED])
        cg.add(var.set_led_binary_sensor(sens))

    if CONF_HEAT_MODE in config:
        cg.add(var.set_heat_mode(config[CONF_HEAT_MODE]))

    if CONF_HIGH_LEVEL_TIME in config:
        cg.add(var.set_high_level_time(config[CONF_HIGH_LEVEL_TIME]))

    if CONF_LOW_LEVEL_TIME in config:
        cg.add(var.set_low_level_time(config[CONF_LOW_LEVEL_TIME]))