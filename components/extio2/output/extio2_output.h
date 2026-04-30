#pragma once
#include "../extio2.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/helpers.h"
#include "esphome/core/progmem.h"

namespace esphome {
namespace extio2 {

enum EXTIO2PWMChannel {
  PWM_0 = 0,
  PWM_1 = 1,
  PWM_2 = 2,
  PWM_3 = 3,
  PWM_4 = 4,
  PWM_5 = 5,
  PWM_6 = 6,
  PWM_7 = 7
};

class EXTIO2FloatOutput : public output::FloatOutput,
                          public Component,
                          public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void dump_config() override;
  void set_pwm_channel(EXTIO2PWMChannel channel) { this->channel_ = channel; }
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
 protected:
  void write_state(float state) override;
  EXTIO2PWMChannel channel_;

};                          


} // namespace extio2
} // namespace esphome