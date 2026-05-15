#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"
#include "../m5unit_encoder.h"

namespace esphome {
namespace m5unit_encoder {

static const char *const TAG_ENCODER_OUTPUT = "m5unit_encoder.output";

class M5UnitEncoderLedOutput : public output::FloatOutput {
 public:
  void set_parent(M5UnitEncoder *parent) { parent_ = parent; }
  void set_led_index(uint8_t index) { led_index_ = index; }
  void set_channel(LedColorChannel channel) { channel_ = channel; }

  void write_state(float state) override {
    if (parent_ == nullptr) {
      ESP_LOGW(TAG_ENCODER_OUTPUT, "Parent is null");
      return;
    }
    parent_->set_led_channel(led_index_, channel_, state);
  }

 protected:
  M5UnitEncoder *parent_{nullptr};
  uint8_t led_index_{0};
  LedColorChannel channel_{LED_RED};
};

}  // namespace m5unit_encoder
}  // namespace esphome
