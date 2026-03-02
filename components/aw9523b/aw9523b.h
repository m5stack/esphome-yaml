// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// datasheet: https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/products/core/CoreS3/AW9523B-EN.pdf
#pragma once

#include "esphome/components/gpio_expander/cached_gpio.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"

namespace esphome {
namespace aw9523b {
class AW9523BComponent : public Component,
                              public i2c::I2CDevice,
                              public gpio_expander::CachedGpioExpander<uint8_t, 16> {
public:
  AW9523BComponent() = default;

  void setup() override;
  void loop() override;
  void dump_config() override;
  void pin_mode(uint8_t pin, gpio::Flags flags);
  
  float get_setup_priority() const override { return setup_priority::IO; }
  float get_loop_priority() const override { return 9.0f; }

  /// Indicate if the component should reset the state during setup
  void set_reset(bool reset) { this->reset_ = reset; }

  /// Set P0 output mode: 1 for push-pull, 0 for open-drain
  void set_p0_drive_mode(uint8_t mode) { this->p0_drive_mode_ = mode; }

  /// Set global max current for LED
  void set_led_max_current(uint8_t current) { this->led_max_current_ = current; }

  /// Setup LED mode by pin number, called by 'output'
  bool setup_led_mode(uint8_t pin);

  /// LED current configuration
  void write_led_current(uint8_t pin, uint8_t current);

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
  /// Interrupt mask - 0: enable interrupt, 1: disable interrupt
  uint8_t interrupt_mask_{0};
  /// LED mode register - 1: GPIO mode, 0: LED mode
  uint16_t led_mode_mask_{0};
  /// P0 output drive mode - 0: open-drain, 1: push-pull
  uint8_t p0_drive_mode_{0};
  /// Chip reset flag
  bool reset_{true};
  /// Global Imax for LED control, typical Imax is 37 mA 
  /// Register value | Max current    | Configure range
  /// 0              | Imax * 1       | [0 ~ Imax]
  /// 1              | Imax * 0.75    | [0 ~ Imax * 0.75]
  /// 2              | Imax * 0.5     | [0 ~ Imax * 0.5]
  /// 3              | Imax * 0.25    | [0 ~ Imax * 0.25]
  uint8_t led_max_current_{0};

  bool read_gpio_modes_();
  bool write_gpio_modes_();
  bool read_gpio_outputs_();
  bool read_gpio_interrupts_();

  bool read_led_modes_();

};

class AW9523BGPIOPin : public GPIOPin, public Parented<AW9523BComponent> {
 public:
  void setup() override;
  void pin_mode(gpio::Flags flags) override;
  bool digital_read() override;
  void digital_write(bool value) override;
  size_t dump_summary(char *buffer, size_t len) const override;

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