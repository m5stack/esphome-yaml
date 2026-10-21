#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/number/number.h"
#include "../m5stack_roller.h"

namespace esphome {
namespace m5stack_roller {

class RollerSpeedNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_speed_setpoint(value);
    parent_->send_speed();
    publish_state(value);
  }
};

class RollerSpeedMaxCurrentNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_speed_max_current(value);
    parent_->send_speed();
    publish_state(value);
  }
};

class RollerPositionNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_position_setpoint(value);
    parent_->send_position();
    publish_state(value);
  }
};

class RollerPositionMaxCurrentNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_position_max_current(value);
    parent_->send_position();
    publish_state(value);
  }
};

class RollerCurrentNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_current_setpoint(value);
    parent_->send_current();
    publish_state(value);
  }
};

class RollerSpeedKpNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_speed_kp(value);
    parent_->send_speed_pid();
    publish_state(value);
  }
};

class RollerSpeedKiNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_speed_ki(value);
    parent_->send_speed_pid();
    publish_state(value);
  }
};

class RollerSpeedKdNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_speed_kd(value);
    parent_->send_speed_pid();
    publish_state(value);
  }
};

class RollerPositionKpNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_position_kp(value);
    parent_->send_position_pid();
    publish_state(value);
  }
};

class RollerPositionKiNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_position_ki(value);
    parent_->send_position_pid();
    publish_state(value);
  }
};

class RollerPositionKdNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_position_kd(value);
    parent_->send_position_pid();
    publish_state(value);
  }
};

class RollerRgbBrightnessNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->set_rgb_brightness(static_cast<uint8_t>(value));
    parent_->send_rgb();
    publish_state(value);
  }
};

class RollerEncoderNumber : public number::Number, public Component, public Parented<RollerBase> {
 public:
  void control(float value) override {
    parent_->cmd_encoder(static_cast<int32_t>(value));
    publish_state(value);
  }
};

}  // namespace m5stack_roller
}  // namespace esphome
