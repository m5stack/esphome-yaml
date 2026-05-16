#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/button/button.h"
#include "../m5stack_roller.h"

namespace esphome {
namespace m5stack_roller {

class RollerSaveToFlashButton : public button::Button, public Component, public Parented<RollerBase> {
 public:
  void press_action() override { parent_->cmd_save_to_flash(); }
};

class RollerReleaseStallButton : public button::Button, public Component, public Parented<RollerBase> {
 public:
  void press_action() override { parent_->cmd_release_stall_protection(); }
};

}  // namespace m5stack_roller
}  // namespace esphome
