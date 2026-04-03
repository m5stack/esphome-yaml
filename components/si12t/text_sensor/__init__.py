import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_CHANNEL, CONF_SENSITIVITY

from .. import si12t_ns, BASE_SCHEMA, CONF_SI12T


DEPENDENCIES = ["si12t"]

ICON_GESTURE_TAP_BUTTON = "mdi:gesture-tap-button"

SI12TTextSensor = si12t_ns.class_("SI12TTextSensor", text_sensor.TextSensor, cg.PollingComponent)

SI12TChannel = si12t_ns.enum("SI12TChannel")

CHANNEL = {
    "CH_1": SI12TChannel.CH_1,
    "CH_2": SI12TChannel.CH_2,
    "CH_3": SI12TChannel.CH_3,
    "CH_4": SI12TChannel.CH_4,
    "CH_5": SI12TChannel.CH_5,
    "CH_6": SI12TChannel.CH_6,
    "CH_7": SI12TChannel.CH_7,
    "CH_8": SI12TChannel.CH_8,
    "CH_9": SI12TChannel.CH_9,
    "CH_10": SI12TChannel.CH_10,
    "CH_11": SI12TChannel.CH_11,
    "CH_12": SI12TChannel.CH_12,
}

SI12TSensitivityLevel = si12t_ns.enum("SI12TSensitivityLevel")

SENS_LEVEL = {
    "SENS_LEVEL_0": SI12TSensitivityLevel.SENS_LEVEL_0,
    "SENS_LEVEL_1": SI12TSensitivityLevel.SENS_LEVEL_1,
    "SENS_LEVEL_2": SI12TSensitivityLevel.SENS_LEVEL_2,
    "SENS_LEVEL_3": SI12TSensitivityLevel.SENS_LEVEL_3,
    "SENS_LEVEL_4": SI12TSensitivityLevel.SENS_LEVEL_4,
    "SENS_LEVEL_5": SI12TSensitivityLevel.SENS_LEVEL_5,
    "SENS_LEVEL_6": SI12TSensitivityLevel.SENS_LEVEL_6,
    "SENS_LEVEL_7": SI12TSensitivityLevel.SENS_LEVEL_7,
}

CONF_EXTEND_THRESHOLD = "extend_threshold"

CONFIG_SCHEMA = cv.Schema(
    text_sensor.text_sensor_schema(
        SI12TTextSensor,
        icon=ICON_GESTURE_TAP_BUTTON,
    )
    .extend(
        {
            cv.Required(CONF_CHANNEL): cv.enum(
                CHANNEL, upper=True, space="_"
            ),
            cv.Optional(CONF_SENSITIVITY, default="SENS_LEVEL_3"): cv.enum(
                SENS_LEVEL, upper=True, space="_"
            ),
            cv.Optional(CONF_EXTEND_THRESHOLD, default=True): cv.boolean
        }
    )
    .extend(BASE_SCHEMA)
    .extend(cv.polling_component_schema("50ms")),
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_SI12T])
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)

    cg.add(var.set_channel(config[CONF_CHANNEL]))
    cg.add(var.set_sensitivity(config[CONF_SENSITIVITY]))
    cg.add(var.set_extend_threshold(config[CONF_EXTEND_THRESHOLD]))