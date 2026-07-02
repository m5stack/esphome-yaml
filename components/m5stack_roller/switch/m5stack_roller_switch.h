#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/switch/switch.h"
#include "../m5stack_roller.h"

namespace esphome {
namespace m5stack_roller {

class RollerMotorOutputSwitch : public switch_::Switch, public Component, public Parented<RollerBase> {
 public:
  void write_state(bool state) override {
    parent_->cmd_motor_output(state);
    publish_state(state);
  }
};

class RollerStallProtectionSwitch : public switch_::Switch, public Component, public Parented<RollerBase> {
 public:
  void write_state(bool state) override {
    parent_->cmd_stall_protection(state);
    publish_state(state);
  }
};

class RollerOverRangeProtectionSwitch : public switch_::Switch, public Component, public Parented<RollerBase> {
 public:
  void write_state(bool state) override {
    parent_->cmd_over_range_protection(state);
    publish_state(state);
  }
};

class RollerButtonSwitchingModeSwitch : public switch_::Switch, public Component, public Parented<RollerBase> {
 public:
  void write_state(bool state) override {
    parent_->cmd_button_switching_mode(state);
    publish_state(state);
  }
};

}  // namespace m5stack_roller
}  // namespace esphome
