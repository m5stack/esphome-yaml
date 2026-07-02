#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/select/select.h"
#include "../m5stack_roller.h"

namespace esphome {
namespace m5stack_roller {

class RollerModeSelect : public select::Select, public Component, public Parented<RollerBase> {
 public:
  void control(const std::string &value) override {
    uint8_t mode = 1;
    if (value == "Speed") mode = 1;
    else if (value == "Position") mode = 2;
    else if (value == "Current") mode = 3;
    else if (value == "Encoder") mode = 4;
    parent_->cmd_set_mode(mode);
    publish_state(value);
  }
};

}  // namespace m5stack_roller
}  // namespace esphome
