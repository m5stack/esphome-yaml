#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pyramidrgb {


enum RGBColorChannel {
  COLOR_R = 0,
  COLOR_G = 1,
  COLOR_B = 2,
};


static const uint8_t STM32_I2C_ADDR = 0x1A;
static const uint8_t RGB1_BRIGHTNESS_REG_ADDR = 0x10; 
static const uint8_t RGB2_BRIGHTNESS_REG_ADDR = 0x11; 
static const uint8_t RGB_CH1_I1_COLOR_REG_ADDR = 0x20; 
static const uint8_t RGB_CH2_I1_COLOR_REG_ADDR = 0x3C; 
static const uint8_t RGB_CH3_I1_COLOR_REG_ADDR = 0x60; 
static const uint8_t RGB_CH4_I1_COLOR_REG_ADDR = 0x7C; 

static const uint8_t NUM_RGB_CHANNELS = 4; 
static const uint8_t NUM_LEDS_PER_GROUP = 7; 

class PyramidRGBComponent : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }


  void set_initial_strip(uint8_t strip) { initial_strip_ = strip; }
  void set_initial_brightness(uint8_t brightness) { initial_brightness_ = brightness; }
  void set_initial_white(uint8_t white) { initial_white_level_ = white; }
  // Dimming/scaling configuration setters
  void set_logarithmic_dimming(bool v) { logarithmic_dimming_ = v; }
  void set_gamma(float v) { gamma_ = v; }
  void set_use_internal_clk(bool v) { use_internal_clk_ = v; }
  void set_power_save_mode(bool v) { power_save_mode_ = v; }
  void set_high_pwm_freq(bool v) { high_pwm_freq_ = v; }
  void set_ref_current(float v) { ref_current_ = v; }
  void set_color_currents(float r, float g, float b, float w) {
    red_current_ = r; green_current_ = g; blue_current_ = b; white_current_ = w;
    // Compute scales vs reference
    red_scale_ = (ref_current_ > 0) ? (red_current_ / ref_current_) : 1.0f;
    green_scale_ = (ref_current_ > 0) ? (green_current_ / ref_current_) : 1.0f;
    blue_scale_ = (ref_current_ > 0) ? (blue_current_ / ref_current_) : 1.0f;
    white_scale_ = (ref_current_ > 0) ? (white_current_ / ref_current_) : 1.0f;
  }


  bool set_strip_brightness(uint8_t strip, uint8_t brightness);


  bool set_channel_color(uint8_t channel, uint8_t r, uint8_t g, uint8_t b);


  bool set_channel_color_component(uint8_t channel, RGBColorChannel color, uint8_t value);

  // Map FloatOutput level [0..1] to device value [0..255] using dimming/scaling
  uint8_t map_level(RGBColorChannel color, float level) const;

 private:

  bool write_color_block_(uint8_t base_reg_addr, const uint8_t *color_bytes, size_t len);
 
  uint8_t channel_base_addr_(uint8_t channel) const;


  uint8_t channel_colors_[NUM_RGB_CHANNELS][3] = {{0}};


  uint8_t initial_strip_ {1};
  uint8_t initial_brightness_ {0};
  uint8_t initial_white_level_ {0};

  // Dimming/scaling config
  bool logarithmic_dimming_ {false};
  float gamma_ {1.0f};
  bool use_internal_clk_ {false};
  bool power_save_mode_ {false};
  bool high_pwm_freq_ {false};
  float ref_current_ {22.5f};
  float red_current_ {22.5f}, green_current_ {22.5f}, blue_current_ {22.5f}, white_current_ {22.5f};
  float red_scale_ {1.0f}, green_scale_ {1.0f}, blue_scale_ {1.0f}, white_scale_ {1.0f};
};

}  // namespace pyramidrgb
}  // namespace esphome
