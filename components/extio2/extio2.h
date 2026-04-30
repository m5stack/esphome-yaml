// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// protocol: https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/789/EXTIO2_V3_Protocol_page_01.png
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/gpio_expander/cached_gpio.h"

namespace esphome {
namespace extio2 {

enum EXTIO2PWMFrequency {
  FREQ_2KHZ = 0,
  FREQ_1KHZ = 1,
  FREQ_500HZ = 2,
  FREQ_250HZ = 3,
  FREQ_125HZ = 4
};

enum PinFunc {
  PIN_INPUT,
  PIN_OUTPUT,
  PIN_ADC_8,
  PIN_ADC_12,
  PIN_SERVO_8,
  PIN_SERVO_16,
  PIN_RGB,
  PIN_PWM
};

class EXTIO2Component : public i2c::I2CDevice,
                        public Component,
                        public gpio_expander::CachedGpioExpander<uint8_t, 8> 
{
 public:
  void setup() override;
  void pin_mode(uint8_t pin, gpio::Flags flags);

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const override { return setup_priority::IO; }

  /// Indicate if the component should reset the state during setup
  void set_reset(bool reset) { this->reset_ = reset; }
  /// Set PWM frequency
  void set_pwm_freq(EXTIO2PWMFrequency freq) { this->freq_ = freq; }

  /// Set Pin function by specific pin number, by default it's GPIO input mode
  void set_pin_func(uint8_t pin, PinFunc func);

  /// Set all pins to a specific function, reserved
  void set_pin_func(PinFunc func);

  /// Set selected pin to a specific function
  void set_pin_func(std::initializer_list<uint8_t> pins, PinFunc func);

  /// set pwm duty cycle for designate pin
  void set_pwm_duty_cycle(uint8_t pin, uint8_t duty_cycle) { this->write_pwm_duty_cycle_(pin, duty_cycle); }

  /// adc sensor
  uint8_t get_adc_8(uint8_t pin) { return this->read_adc8_(pin); }
  uint16_t get_adc_12(uint8_t pin) { return this->read_adc12_(pin); }

  /// servo 
  void set_servo_angle(uint8_t pin, uint8_t angle) { this->write_servo8_(pin, angle); }
  void set_servo_pulse(uint8_t pin, uint16_t us) { this->write_servo16_(pin, us); }

  /// single led
  void set_led_color(uint8_t pin, uint8_t r, uint8_t g, uint8_t b) { this->write_led_(pin, r, g, b); }

  /// addressable led
  void set_led_color(const uint8_t *buf, size_t len) { this->write_led_(buf, len); }

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
  /// Mask for drive mode - 1 means open-drain, 0 means push-pull
  uint16_t drv_mode_mask_{0};
  /// The mask to write as pullup state 
  uint16_t pull_up_mask_{0};
  /// The mask to write as pulldown state
  uint16_t pull_down_mask_{0};

  
  bool read_gpio_modes_();
  bool write_gpio_modes_();
  bool read_gpio_outputs_();


  uint8_t read_adc8_(uint8_t pin);
  uint16_t read_adc12_(uint8_t pin);

  void write_led_(uint8_t pin, uint8_t r, uint8_t g, uint8_t b);

  void write_led_(const uint8_t *buf, size_t len);

  void write_pwm_duty_cycle_(uint8_t pin, uint8_t cycle);

  void write_servo8_(uint8_t pin, uint8_t angle);

  void write_servo16_(uint8_t pin, uint16_t us);

  EXTIO2PWMFrequency freq_{FREQ_1KHZ};
  bool reset_{true};
  uint8_t firm_ver_;
};

class EXTIO2Pin : public GPIOPin, 
                  public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void pin_mode(gpio::Flags flags) override;
  bool digital_read() override;
  void digital_write(bool value) override;
  size_t dump_summary(char *buffer, size_t len)  const override;

  void set_pin(uint8_t pin) { this->pin_ = pin; }
  void set_inverted(bool inverted) { this->inverted_ = inverted; }
  void set_flags(gpio::Flags flags) { this->flags_ = flags; }

  gpio::Flags get_flags() const override { return this->flags_; }

 protected:
  uint8_t pin_;
  bool inverted_;
  gpio::Flags flags_;
};

} // namespace extio2
} // namespace esphome 