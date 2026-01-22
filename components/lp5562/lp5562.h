// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// Original register reference:
// https://www.ti.com/lit/ds/symlink/lp5562.pdf

#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"

namespace esphome {
namespace lp5562 {

typedef enum Channel {
    CHANNEL_B = 0,
    CHANNEL_G = 1,
    CHANNEL_R = 2,
    CHANNEL_W = 3
} LED_Channel_t;


class LP5562Component : public i2c::I2CDevice,
                        public Component
{

public:
    void setup() override;
    void dump_config() override;
    
    void set_use_internal_clk(bool use_internal_clk) { this->use_internal_clk_ = use_internal_clk; }
    void set_power_save_mode(bool power_save_enable) { this->power_save_enable_ = power_save_enable; }
    void set_high_pwm_freq(bool high_pwm_freq_enable) { this->high_pwm_freq_enable_ = high_pwm_freq_enable; }
    void set_logarithmic_dimming(bool logarithmic_enable) {this->logarithmic_dimming_ = logarithmic_enable; }
    void set_red_current(float current) { this->red_current_ = current; }
    void set_green_current(float current) { this->green_current_ = current; }
    void set_blue_current(float current) { this->blue_current_ = current; }
    void set_white_current(float current) { this->white_current_ = current; }
    
    void set_led_brightness_by_channel(LED_Channel_t channel, uint8_t brightness);
    void set_led_current_by_channel(LED_Channel_t channel, uint8_t current);
    void map_led_2_ctrl_src(LED_Channel_t channel, uint8_t source); // map led to control source

protected:
    void set_engine_mode_(uint8_t engine, uint8_t mode);
    void set_all_engine_mode_(uint8_t mode);
    void set_all_led_mapping_(uint8_t source);

    bool use_internal_clk_ {false};
    bool power_save_enable_{false};
    bool high_pwm_freq_enable_ {false};
    bool logarithmic_dimming_ {false};

    float red_current_ {0};
    float green_current_ {0};
    float blue_current_ {0};
    float white_current_ {0};
};

} // namespace lp5562
} // namespace esphome