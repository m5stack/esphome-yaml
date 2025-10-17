#pragma once

#include "esphome/components/gpio_expander/cached_gpio.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace aw9523b {
class AW9523BComponent : public Component,
                              public i2c::I2CDevice,
                              public gpio_expander::CachedGpioExpander<uint8_t, 16> {
 public:
  AW9523BComponent() = default;

  void setup() override;
  void pin_mode(uint8_t pin, gpio::Flags flags);

  float get_setup_priority() const { return setup_priority::IO; }
  void dump_config() override;
  void loop() override;

  /// Indicate if the component should reset the state during setup
  void set_reset(bool reset) { this->reset_ = reset; }

  // AW9523B P0 output mode, either open drain or push pull
  void set_p0_push_pull_mode(bool mode) { this->p0_push_pull_mode_ = mode; }
 protected:
  bool digital_read_hw(uint8_t pin) override;
  bool digital_read_cache(uint8_t pin) override;
  void digital_write_hw(uint8_t pin, bool value) override;

  /// Mask for the pin mode - 1 means output, 0 means input
  uint16_t mode_mask_{0};
  /// The mask to write as output state - 1 means HIGH, 0 means LOW
  uint16_t output_mask_{0};
  /// The state read in digital_read_hw - 1 means HIGH, 0 means LOW
  uint16_t input_mask_{0};
  /// The mask to write as input buffer state - 1 means enabled, 0 means disabled
  uint16_t pull_enable_mask_{0};
  /// The mask to write as pullup state - 1 means pullup, 0 means pulldown
  uint16_t pull_up_down_mask_{0};

  bool p0_push_pull_mode_{false};  // false = open-drain, true = push-pull

  bool reset_{true};

  bool read_gpio_modes_();
  bool write_gpio_modes_();
  bool read_gpio_outputs_();
};

class AW9523BGPIOPin : public GPIOPin, public Parented<AW9523BComponent> {
 public:
  void setup() override;
  void pin_mode(gpio::Flags flags) override;
  bool digital_read() override;
  void digital_write(bool value) override;
  std::string dump_summary() const override;

  void set_pin(uint8_t pin) { this->pin_ = pin; }
  void set_inverted(bool inverted) { this->inverted_ = inverted; }
  void set_flags(gpio::Flags flags) { this->flags_ = flags; }

  gpio::Flags get_flags() const override { return this->flags_; }

 protected:
  uint8_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

}  // namespace aw9523b
}  // namespace esphome