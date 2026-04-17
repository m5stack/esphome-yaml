#pragma once


#include "esphome/components/output/float_output.h"
#include "../i2c_joystick_2.h"
#include "esphome/core/helpers.h"


namespace esphome {
namespace i2c_joystick_2 {

class JoyStick2FloatOutput : public output::FloatOutput,
                            public Parented<I2CJoystick2Component>
{
 public:
  void set_channel(RGBChannel channel) { this->channel_ = channel; }
 protected:
  void write_state(float state) override {
    uint8_t val = static_cast<uint8_t>(state * 255.0f);
    this->parent_->write_rgb_channel(this->channel_, val);
  }

  RGBChannel channel_;


};                            

}
}