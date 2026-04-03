#pragma once

#include "esphome/components/output/float_output.h"
#include "../aw9523b.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace aw9523b {

class AW9523BFloatOutput : public output::FloatOutput,
                      public Component,
                      public Parented<AW9523BComponent>
{
public:
  void setup() override;
  void dump_config() override;
  void set_pin(uint8_t pin) { this->pin_ = pin; }
  
protected:
  void write_state(float state) override;
  // pin number passed by configuration
  uint8_t pin_;
};


} // namespace aw9523b
} // namespace esphome