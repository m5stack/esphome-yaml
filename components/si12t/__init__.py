import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import (
    CONF_ID,
    CONF_RESET,
)

si12t_ns = cg.esphome_ns.namespace("si12t")

SI12TComponent = si12t_ns.class_("SI12TComponent", cg.Component, i2c.I2CDevice)

CONF_SI12T = "si12t"
CONF_AUTO_MODE = "auto_mode"
CONF_FIRST_TOUCH_TIME = "first_touch"
CONF_INTR_LEVEL = "intr_level"
CONF_RES_TIME = "res_time"
CONF_SLEEP_MODE = "sleep_mode"

SI12TFirstTouchTime = si12t_ns.enum("SI12TFirstTouchTime")

FTT = {
    "FTT_5S": SI12TFirstTouchTime.FTT_5S,
    "FTT_10S": SI12TFirstTouchTime.FTT_10S,
    "FTT_15S": SI12TFirstTouchTime.FTT_15S,
    "FTT_20S": SI12TFirstTouchTime.FTT_20S,
}

SI12TInterruptLevel = si12t_ns.enum("SI12TInterruptLevel")

INTR_LEVEL = {
    "OUTPUT_MEDIUM_HIGH": SI12TInterruptLevel.OUTPUT_MEDIUM_HIGH,
    "OUTPUT_ALL": SI12TInterruptLevel.OUTPUT_ALL,
    "OUTPUT_MEDIUM_HIGH_ALT": SI12TInterruptLevel.OUTPUT_MEDIUM_HIGH_ALT,
    "OUTPUT_HIGH": SI12TInterruptLevel.OUTPUT_HIGH
}

SI12TResponseTime = si12t_ns.enum("SI12TResponseTime")

RES_TIME = {
    "RES_CYCLE_2": SI12TResponseTime.RES_CYCLE_2,
    "RES_CYCLE_3": SI12TResponseTime.RES_CYCLE_3,
    "RES_CYCLE_4": SI12TResponseTime.RES_CYCLE_4,
    "RES_CYCLE_5": SI12TResponseTime.RES_CYCLE_5,
    "RES_CYCLE_6": SI12TResponseTime.RES_CYCLE_6,
    "RES_CYCLE_7": SI12TResponseTime.RES_CYCLE_7,
    "RES_CYCLE_8": SI12TResponseTime.RES_CYCLE_8,
    "RES_CYCLE_9": SI12TResponseTime.RES_CYCLE_9,
}



BASE_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_SI12T): cv.use_id(SI12TComponent)
})

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(SI12TComponent),
            cv.Optional(CONF_AUTO_MODE, default=True): cv.boolean,
            cv.Optional(CONF_FIRST_TOUCH_TIME, default="FTT_10S"): cv.enum(
                FTT, upper=True, space="_"
            ),
            cv.Optional(CONF_INTR_LEVEL, default="OUTPUT_MEDIUM_HIGH"): cv.enum(
                INTR_LEVEL, upper=True, space="_"
            ),
            cv.Optional(CONF_RES_TIME, default="RES_CYCLE_5"): cv.enum(
                RES_TIME, upper=True, space = "_"
            ),
            cv.Optional(CONF_RESET, default=True): cv.boolean,
            cv.Optional(CONF_SLEEP_MODE, default=True): cv.boolean
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x68))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_auto_mode(config[CONF_AUTO_MODE]))
    cg.add(var.set_first_touch_time(config[CONF_FIRST_TOUCH_TIME]))
    cg.add(var.set_intr_level(config[CONF_INTR_LEVEL]))
    cg.add(var.set_res_time(config[CONF_RES_TIME]))
    cg.add(var.set_reset(config[CONF_RESET]))
    cg.add(var.set_sleep_mode(config[CONF_SLEEP_MODE]))