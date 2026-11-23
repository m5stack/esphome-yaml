#pragma once
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/components/light/light_output.h"
#include "../m5stack_roller.h"

namespace esphome {
namespace m5stack_roller {

class RollerRgbLight : public light::LightOutput, public Parented<RollerBase> {
 public:
  light::LightTraits get_traits() override {
    light::LightTraits t;
    t.set_supported_color_modes({light::ColorMode::RGB});
    return t;
  }

  void write_state(light::LightState *state) override {
    float r, g, b;
    state->current_values_as_rgb(&r, &g, &b);
    uint8_t ri = static_cast<uint8_t>(r * 255);
    uint8_t gi = static_cast<uint8_t>(g * 255);
    uint8_t bi = static_cast<uint8_t>(b * 255);
    // Mode 1 = user-defined when any colour is set, 0 = system default when all off
    uint8_t mode = (ri || gi || bi) ? 1 : 0;
    parent_->set_rgb_color(ri, gi, bi, mode);
    parent_->send_rgb();
  }
};

}  // namespace m5stack_roller
}  // namespace esphome
