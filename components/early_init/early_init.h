#pragma once

#include <vector>
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace early_init {

struct EarlyInitPinConfig {
  GPIOPin *pin{nullptr};
  bool level{false};
};

class EarlyInitComponent : public Component {
 public:
  void add_pin(GPIOPin *pin, bool level) {
    this->pins_.push_back({pin, level});
  }

  void set_delay(uint32_t delay_ms) { this->delay_ms_ = delay_ms; }

  float get_setup_priority() const override;
  void setup() override;

 protected:
  std::vector<EarlyInitPinConfig> pins_;
  uint32_t delay_ms_{0};
};

}  // namespace early_init
}  // namespace esphome