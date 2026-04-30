#include "extio2_output.h"
#include "esphome/core/hal.h"
#include <cstdint>


namespace esphome {
namespace extio2 {

static const char *TAG = "extio2.output";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(PWMChannelStrings,
  "PWM 0",
  "PWM 1",
  "PWM 2",
  "PWM 3",
  "PWM 4",
  "PWM 5",
  "PWM 6",
  "PWM 7",
  "Unknown PWM Channel"
);

static const LogString *pwm_channel_to_string(EXTIO2PWMChannel channel) {
  return PWMChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}

#endif


void EXTIO2FloatOutput::setup() {
  // configure the pin to PWM mode
  this->parent_->set_pin_func(static_cast<uint8_t>(this->channel_), PIN_PWM);
  delay(20);
}


void EXTIO2FloatOutput::write_state(float state) {
  // write duty cycle
  uint8_t duty_cycle = static_cast<uint8_t>(state * 100);

  if ( duty_cycle > 100 ) {
    duty_cycle = 100;
  }

  this->parent_->set_pwm_duty_cycle(static_cast<uint8_t>(this->channel_), duty_cycle);
}

void EXTIO2FloatOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 Float Output:\n"
                     "  PWM Channel: %s\n",
                     LOG_STR_ARG(pwm_channel_to_string(this->channel_)));
  LOG_FLOAT_OUTPUT(this);
}


} // namespace extio2
} // namespace esphome