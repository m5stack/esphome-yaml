import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID
from esphome.const import (
    CONF_ID,
    CONF_CHANNEL,
)

from .. import extio2_ns, BASE_SCHEMA, CONF_EXTIO2_ID

DEPENDENCIES = ["extio2"]

CONF_LIGHT = "light"
CONF_ADDRESSABLE_LIGHT = "addressable_light"
CONF_TYPE = "type"

EXTIO2AddressableLight = extio2_ns.class_(
    "EXTIO2AddressableLight",
    light.AddressableLight
)

EXTIO2Light = extio2_ns.class_(
    "EXTIO2Light",
    light.LightOutput,
    cg.Component
)

EXTIO2LightChannel = extio2_ns.enum("EXTIO2LightChannel")

LIGHT_CHANNELS = {
    "LIGHT_0": EXTIO2LightChannel.LIGHT_0,
    "LIGHT_1": EXTIO2LightChannel.LIGHT_1,
    "LIGHT_2": EXTIO2LightChannel.LIGHT_2,
    "LIGHT_3": EXTIO2LightChannel.LIGHT_3,
    "LIGHT_4": EXTIO2LightChannel.LIGHT_4,
    "LIGHT_5": EXTIO2LightChannel.LIGHT_5,
    "LIGHT_6": EXTIO2LightChannel.LIGHT_6,
    "LIGHT_7": EXTIO2LightChannel.LIGHT_7
}

CONF_NUM_LEDS = "num_leds"

ADDRESSABLE_LIGHT_SCHEMA = light.ADDRESSABLE_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID) : cv.declare_id(EXTIO2AddressableLight),
        cv.Required(CONF_NUM_LEDS): cv.int_range(min=1, max=8)
    }
).extend(BASE_SCHEMA)


LIGHT_SCHEMA = light.light_schema(
    EXTIO2Light,
    light.LightType.RGB
).extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(EXTIO2Light),
        cv.Required(CONF_CHANNEL): cv.enum(
            LIGHT_CHANNELS, upper=True, space='_')
    }
).extend(BASE_SCHEMA)


CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_LIGHT : LIGHT_SCHEMA,
        CONF_ADDRESSABLE_LIGHT: ADDRESSABLE_LIGHT_SCHEMA
    },
    key=CONF_TYPE,
    lower=True
)

async def to_code(config):
    var = await light.new_light(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_EXTIO2_ID])
    if config[CONF_TYPE] == "light":
        cg.add(var.set_light_channel(config[CONF_CHANNEL]))
    else:
        cg.add(var.set_num_leds(config[CONF_NUM_LEDS]))