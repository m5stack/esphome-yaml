#include "extio2_number.h"
#include "esphome/components/number/number.h"
#include "esphome/core/progmem.h"

namespace esphome {
namespace extio2 {

static const char *TAG = "extio2.number";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(ServoChannelStrings, 
  "Servo 0",
  "Servo 1",
  "Servo 2",
  "Servo 3",
  "Servo 4",
  "Servo 5",
  "Servo 6",
  "Servo 7",
  "Unknown Servo Channel"
);

static const LogString *servo_channel_to_string(EXTIO2ServoChannel channel) {
  return ServoChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}

#endif

void EXTIO2AngleNumber::setup() {
  uint8_t pin = static_cast<uint8_t>(this->channel_);
  this->parent_->set_pin_func(pin, PIN_SERVO_8);
  delay(20);
}

void EXTIO2AngleNumber::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 Servo Angle Number: \n"
                     "  Servo channel %s\n",
                     LOG_STR_ARG(servo_channel_to_string(this->channel_)));
  LOG_NUMBER(" ", "Servo Angle Number", this);
}

void EXTIO2AngleNumber::control(float value) {
  uint8_t angle = static_cast<uint8_t>(value);
  if ( angle > 180 ) {
    angle = 180;
  }
  this->parent_->set_servo_angle(static_cast<uint8_t>(this->channel_), angle);
}

void EXTIO2PulseNumber::setup() {
  uint8_t pin = static_cast<uint8_t>(this->channel_);
  this->parent_->set_pin_func(pin, PIN_SERVO_16);
  delay(20);
}

void EXTIO2PulseNumber::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 Servo Pulse Number: \n"
                     "  Servo channel %s\n",
                     LOG_STR_ARG(servo_channel_to_string(this->channel_)));
  LOG_NUMBER(" ", "Servo Pulse Number", this);
}

void EXTIO2PulseNumber::control(float value) {
  uint16_t us = static_cast<uint16_t>(value);
  if ( us > 2500 ) {
    us = 2500;
  }
  this->parent_->set_servo_pulse(static_cast<uint8_t>(this->channel_), us);
}

}  // namespace extio2
}  // namespace esphome