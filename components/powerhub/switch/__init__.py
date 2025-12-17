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
LEDSwitch = powerhub_ns.class_("LEDSwitch", switch.Switch)
USBSwitch = powerhub_ns.class_("USBSwitch", switch.Switch)
GroveRedSwitch = powerhub_ns.class_("GroveRedSwitch", switch.Switch)
GroveBlueSwitch = powerhub_ns.class_("GroveBlueSwitch", switch.Switch)
RS485CANSwitch = powerhub_ns.class_("RS485CANSwitch", switch.Switch)
VAmeterSwitch = powerhub_ns.class_("VAMeterSwitch", switch.Switch)
ChargeSwitch = powerhub_ns.class_("ChargeSwitch", switch.Switch)

# RS485 CAN interface power direction control
# ON:  RS485 / CAN interface will power the external device
# OFF: RS485 / CAN interface won't power the external device
DirectionSwitch = powerhub_ns.class_("DirectionSwitch", switch.Switch)

# wake up source
RTCWakeUpSwitch = powerhub_ns.class_("RTCWakeUpSwitch", switch.Switch)
VINWakeUpSwitch = powerhub_ns.class_("VINWakeUpSwitch", switch.Switch)
VUSBWakeUpSwitch = powerhub_ns.class_("VUSBWakeUpSwitch", switch.Switch)


CONFIG_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_LED_PWR): switch.switch_schema(
            LEDSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            icon=ICON_POWER,
            default_restore_mode="RESTORE_DEFAULT_OFF"
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

    if CONF_LED_PWR in config:
        var = await switch.new_switch(config[CONF_LED_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_led_pwr_switch(var))
        cg.add(var.publish_state(True))

    if CONF_USB_PWR in config:
        var = await switch.new_switch(config[CONF_USB_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_usb_pwr_switch(var))

    if CONF_GROVE_RED_PWR in config:
        var = await switch.new_switch(config[CONF_GROVE_RED_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_grove_red_pwr_switch(var))

    if CONF_GROVE_BLUE_PWR in config:
        var = await switch.new_switch(config[CONF_GROVE_BLUE_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_grove_blue_pwr_switch(var))

    if CONF_RS485_CAN_PWR in config:
        var = await switch.new_switch(config[CONF_RS485_CAN_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_rs485_can_pwr_switch(var))

    if CONF_VAMETER_PWR in config:
        var = await switch.new_switch(config[CONF_VAMETER_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_vameter_pwr_switch(var))
        cg.add(var.publish_state(True))

    if CONF_CHARGE_PWR in config:
        var = await switch.new_switch(config[CONF_CHARGE_PWR])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_charge_pwr_switch(var))
        cg.add(var.publish_state(True))

    if CONF_RS485_CAN_DIRECTION in config:
        var = await switch.new_switch(config[CONF_RS485_CAN_DIRECTION])
        await cg.register_parented(var, config[CONF_POWER_HUB_ID])
        cg.add(powerhub.set_rs485_can_direction_switch(var))
        



