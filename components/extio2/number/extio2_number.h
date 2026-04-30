#pragma once

#include "esphome/components/number/number.h"
#include "../extio2.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace extio2 {

enum EXTIO2ServoChannel {
  SERVO_0 = 0,
  SERVO_1 = 1,
  SERVO_2 = 2,
  SERVO_3 = 3,
  SERVO_4 = 4,
  SERVO_5 = 5,
  SERVO_6 = 6,
  SERVO_7 = 7
};

class EXTIO2AngleNumber : public number::Number,
                          public Component,
                          public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void dump_config() override;
  void set_servo_channel(EXTIO2ServoChannel channel) { this->channel_ = channel; }
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
 protected:
  void control(float value) override;
  EXTIO2ServoChannel channel_;

};


class EXTIO2PulseNumber : public number::Number,
                          public Component,
                          public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void dump_config() override;
  void set_servo_channel(EXTIO2ServoChannel channel) { this->channel_ = channel; }
 protected:
  void control(float value) override;
  EXTIO2ServoChannel channel_;  
};

}  // namespace extio2
}  // namespace esphome