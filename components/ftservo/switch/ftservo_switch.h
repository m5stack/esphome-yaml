#pragma once

#include "esphome/components/switch/switch.h"
#include "../ftservo.h"


namespace esphome::ftservo {

class ServoTorqueSwitch : public switch_::Switch, 
                          public Component, 
                          public Parented<FTServo> 
{
 public:
  void setup() override;
  void dump_config() override;

 protected:
  void write_state(bool state) override;

};
} // namespace esphome::ftservo
