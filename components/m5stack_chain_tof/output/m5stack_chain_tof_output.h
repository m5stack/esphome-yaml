#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/components/m5stack_chain_tof/m5stack_chain_tof.h"

namespace esphome {
namespace m5stack_chain_tof {

enum LedChannel {
  LED_BRIGHTNESS = 0,
  RGB_BRIGHTNESS = 1,
  RGB_RED = 2,
  RGB_GREEN = 3,
  RGB_BLUE = 4,
};

class ChainToFLedOutput : public output::FloatOutput, public Component {
 public:
  void set_parent(ChainToFSensor *parent) { this->parent_ = parent; }
  void set_channel(LedChannel channel) { this->channel_ = channel; }

  void write_state(float state) override {
    if (this->parent_ == nullptr) {
      return;
    }

    uint8_t op_status = 0;

    switch (this->channel_) {
      case LED_BRIGHTNESS: {
        uint8_t brightness = static_cast<uint8_t>(roundf(clamp(state, 0.0f, 1.0f) * 100.0f));
        this->parent_->set_led_brightness(brightness, &op_status);

        if (brightness > 0) {
          this->parent_->set_rgb_color(255, 255, 255, &op_status);
        }
        break;
      }
      case RGB_BRIGHTNESS: {
        uint8_t brightness = static_cast<uint8_t>(roundf(clamp(state, 0.0f, 1.0f) * 100.0f));
        this->parent_->set_led_brightness(brightness, &op_status);
        break;
      }
      case RGB_RED:
      case RGB_GREEN:
      case RGB_BLUE: {
        uint8_t r = 0, g = 0, b = 0;
        this->parent_->get_rgb_color(&r, &g, &b, &op_status);

        uint8_t value = static_cast<uint8_t>(roundf(clamp(state, 0.0f, 1.0f) * 255.0f));

        if (this->channel_ == RGB_RED) {
          r = value;
        } else if (this->channel_ == RGB_GREEN) {
          g = value;
        } else if (this->channel_ == RGB_BLUE) {
          b = value;
        }

        this->parent_->set_rgb_color(r, g, b, &op_status);
        break;
      }
    }
  }

 protected:
  ChainToFSensor *parent_{nullptr};
  LedChannel channel_{LED_BRIGHTNESS};
};

}  // namespace m5stack_chain_tof
}  // namespace esphome
