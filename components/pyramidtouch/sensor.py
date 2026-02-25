import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, CONF_UPDATE_INTERVAL

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@Jasionf"]

CONF_PUBLISH_SWIPE = "publish_swipe_event"
CONF_SWIPE_TIMEOUT_MS = "swipe_timeout_ms"

CONF_TOUCH1 = "touch1"
CONF_TOUCH2 = "touch2"
CONF_TOUCH3 = "touch3"
CONF_TOUCH4 = "touch4"
CONF_SWIPE_EVENT = "swipe_event"

pyramidtouch_ns = cg.esphome_ns.namespace("pyramidtouch")
PyramidTouchComponent = pyramidtouch_ns.class_(
    "PyramidTouchComponent", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PyramidTouchComponent),
            cv.Optional(CONF_UPDATE_INTERVAL, default="50ms"): cv.update_interval,
            cv.Optional(CONF_PUBLISH_SWIPE, default=True): cv.boolean,
            cv.Optional(CONF_SWIPE_TIMEOUT_MS, default=500): cv.int_range(min=50, max=2000),

            cv.Optional(CONF_TOUCH1): sensor.sensor_schema(
                accuracy_decimals=0,
                icon="mdi:gesture-tap",
            ),
            cv.Optional(CONF_TOUCH2): sensor.sensor_schema(
                accuracy_decimals=0,
                icon="mdi:gesture-tap",
            ),
            cv.Optional(CONF_TOUCH3): sensor.sensor_schema(
                accuracy_decimals=0,
                icon="mdi:gesture-tap",
            ),
            cv.Optional(CONF_TOUCH4): sensor.sensor_schema(
                accuracy_decimals=0,
                icon="mdi:gesture-tap",
            ),
            cv.Optional(CONF_SWIPE_EVENT): sensor.sensor_schema(
                accuracy_decimals=0,
                icon="mdi:gesture-swipe",
            ),
        }
    )
    .extend(cv.polling_component_schema("50ms"))
    .extend(i2c.i2c_device_schema(0x1A))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await i2c.register_i2c_device(var, config)
    await cg.register_component(var, config)

    cg.add(var.set_publish_swipe_event(config[CONF_PUBLISH_SWIPE]))
    cg.add(var.set_swipe_timeout_ms(config[CONF_SWIPE_TIMEOUT_MS]))
    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))

    if CONF_TOUCH1 in config:
        s = await sensor.new_sensor(config[CONF_TOUCH1])
        cg.add(var.set_touch_sensor(0, s))
    if CONF_TOUCH2 in config:
        s = await sensor.new_sensor(config[CONF_TOUCH2])
        cg.add(var.set_touch_sensor(1, s))
    if CONF_TOUCH3 in config:
        s = await sensor.new_sensor(config[CONF_TOUCH3])
        cg.add(var.set_touch_sensor(2, s))
    if CONF_TOUCH4 in config:
        s = await sensor.new_sensor(config[CONF_TOUCH4])
        cg.add(var.set_touch_sensor(3, s))
    if CONF_SWIPE_EVENT in config:
        s = await sensor.new_sensor(config[CONF_SWIPE_EVENT])
        cg.add(var.set_swipe_event_sensor(s))