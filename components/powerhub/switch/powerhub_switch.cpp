
#include "powerhub_switch.h"
#include "esphome/core/progmem.h"
#include "esphome/core/log.h"

namespace esphome {
namespace powerhub {

static const char *TAG = "powerhub.switch";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG
PROGMEM_STRING_TABLE(PowerChannelStrings,
  "LED",
  "USB",
  "Grove Red Port",
  "Grove Blue Port",
  "RS485 CAN",
  "VAMeter",
  "Charge",
  "Unknown"
);

static const LogString *power_channel_to_str(PowerChannel channel) {
  return PowerChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}
#endif

void PowerSwitch::setup() {
  bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
  if (initial_state) {
    this->turn_on();
  } else {
    this->turn_off();
  }
}


void PowerSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Channel %s\n"
                      "State %s", 
                      LOG_STR_ARG(power_channel_to_str(this->channel_)),
                      ONOFF(this->state));
  LOG_SWITCH("  ",  "Power Switch", this);
}

void PowerSwitch::write_state(bool state) {
  switch (this->channel_) {
    case PowerChannel::POWER_LED:
      this->parent_->set_led_pwr_en(state);
      break;
    case PowerChannel::POWER_USB:
      this->parent_->set_usb_pwr_en(state);
      break;
    case PowerChannel::POWER_GROVE_RED:
      this->parent_->set_grove_red_pwr_en(state);
      break;
    case PowerChannel::POWER_GROVE_BLUE:
      this->parent_->set_grove_blue_pwr_en(state);
      break;
    case PowerChannel::POWER_RS485_CAN:
      this->parent_->set_rs485_can_pwr_en(state);
      break;
    case PowerChannel::POWER_VAMETER:
      this->parent_->set_vameter_pwr_en(state);
      break;
    case PowerChannel::POWER_CHARGE:
      this->parent_->set_charge_pwr_en(state);
      break;
    default:
      ESP_LOGW(TAG, "Unknown power channel");
  }
  this->publish_state(state);
}


void RS485CANDirSwitch::setup() {
  bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);
  if (initial_state) {
    this->turn_on();
  } else {
    this->turn_off();
  }
}

void RS485CANDirSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "RS485 CAN Output Direction: %s", ONOFF(this->state));
  LOG_SWITCH("  ", "RS485 CAN Direction Switch", this);  
}

void RS485CANDirSwitch::write_state(bool state) {
  this->parent_->set_rs485_can_pwr_direction(state);
  this->publish_state(state);
}

} // namespace powerhub
} // namespace esphome

