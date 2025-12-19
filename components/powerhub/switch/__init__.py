import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.cpp_generator import MockObjClass
from esphome.const import (
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
    DEVICE_CLASS_SWITCH,
    ICON_POWER
)

from .. import powerhub_ns, BASE_SCHEMA, CONF_POWER_HUB_ID

CODEOWNERS = ["@m5stack"]
DEPENDENCIES = ["powerhub"]

CONF_LED_PWR = "led_pwr"
CONF_USB_PWR = "usb_pwr"
CONF_GROVE_RED_PWR = "grove_red_pwr"
CONF_GROVE_BLUE_PWR = "grove_blue_pwr"
CONF_RS485_CAN_PWR = "rs485_can_pwr"
CONF_VAMETER_PWR = "vameter_pwr"
CONF_CHARGE_PWR = "charge_pwr"

# CONF_RS485_CAN_OUTPUT = "rs485_can_output"
CONF_RS485_CAN_DIRECTION = "rs485_can_direction"

CONF_RTC_WAKE_UP = "rtc_wake_up"
CONF_VIN_WAKE_UP = "vin_wake_up"
CONF_VUSB_WAKE_UP = "vusb_wake_up"

ICON_EXPORT = "mdi:export"
ICON_BED_CLOCK = "mdi:bed-clock"

# power channel
LEDSwitch = powerhub_ns.class_("LEDSwitch", cg.Component, switch.Switch)
USBSwitch = powerhub_ns.class_("USBSwitch", switch.Switch, cg.Component)
GroveRedSwitch = powerhub_ns.class_("GroveRedSwitch", switch.Switch, cg.Component)
GroveBlueSwitch = powerhub_ns.class_("GroveBlueSwitch", switch.Switch, cg.Component)
RS485CANSwitch = powerhub_ns.class_("RS485CANSwitch", switch.Switch, cg.Component)
VAmeterSwitch = powerhub_ns.class_("VAMeterSwitch", switch.Switch, cg.Component)
ChargeSwitch = powerhub_ns.class_("ChargeSwitch", switch.Switch, cg.Component)

# RS485 CAN interface power direction control
# ON:  RS485 / CAN interface will power the external device
# OFF: RS485 / CAN interface won't power the external device
DirectionSwitch = powerhub_ns.class_("DirectionSwitch", switch.Switch, cg.Component)

# wake up source
RTCWakeUpSwitch = powerhub_ns.class_("RTCWakeUpSwitch", switch.Switch, cg.Component)
VINWakeUpSwitch = powerhub_ns.class_("VINWakeUpSwitch", switch.Switch, cg.Component)
VUSBWakeUpSwitch = powerhub_ns.class_("VUSBWakeUpSwitch", switch.Switch, cg.Component)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_LED_PWR): switch.switch_schema(
            LEDSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_ON"
        ),
        cv.Optional(CONF_USB_PWR): switch.switch_schema(
            USBSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_GROVE_RED_PWR): switch.switch_schema(
            GroveRedSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_GROVE_BLUE_PWR): switch.switch_schema(
            GroveBlueSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_RS485_CAN_PWR): switch.switch_schema(
            RS485CANSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
        cv.Optional(CONF_VAMETER_PWR): switch.switch_schema(
            VAmeterSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_ON"
        ),
        cv.Optional(CONF_CHARGE_PWR): switch.switch_schema(
            ChargeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_ON"
        ),
        cv.Optional(CONF_RS485_CAN_DIRECTION): switch.switch_schema(
            DirectionSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_EXPORT,
            default_restore_mode="RESTORE_DEFAULT_OFF"
        ),
    }

).extend(BASE_SCHEMA)


async def to_code(config):

    powerhub = await cg.get_variable(config[CONF_POWER_HUB_ID])

    for switch_type in [
        CONF_LED_PWR, 
        CONF_USB_PWR, 
        CONF_GROVE_RED_PWR, 
        CONF_GROVE_BLUE_PWR, 
        CONF_RS485_CAN_PWR, 
        CONF_VAMETER_PWR, 
        CONF_CHARGE_PWR,
        CONF_RS485_CAN_DIRECTION
    ]:
        if conf := config.get(switch_type):
            var = await switch.new_switch(conf)
            await cg.register_component(var, conf)
            await cg.register_parented(var, powerhub)
            cg.add(getattr(powerhub, f"set_{switch_type}_switch")(var))
