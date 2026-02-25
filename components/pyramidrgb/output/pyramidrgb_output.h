#pragma once

#include "esphome/components/output/float_output.h"
#include "../pyramidrgb.h"

namespace esphome {
namespace pyramidrgb {

class PyramidRGBOutput : public output::FloatOutput,
                         public Parented<PyramidRGBComponent> {
 public:
  void set_channel(uint8_t channel) { channel_ = channel; }
  void set_color(RGBColorChannel color) { color_ = color; }

  void write_state(float state) override {
    uint8_t val = this->parent_->map_level(this->color_, state);
    this->parent_->set_channel_color_component(this->channel_, this->color_, val);
  }

 protected:
  uint8_t channel_ {0};
  RGBColorChannel color_ {COLOR_R};
};

}  // namespace pyramidrgb
}  // namespace esphome
