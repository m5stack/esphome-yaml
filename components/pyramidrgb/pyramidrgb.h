#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pyramidrgb {

// 颜色通道枚举
enum RGBColorChannel {
  COLOR_R = 0,
  COLOR_G = 1,
  COLOR_B = 2,
};

// STM32 RGB 控制器寄存器与参数
static const uint8_t STM32_I2C_ADDR = 0x1A;
static const uint8_t RGB1_BRIGHTNESS_REG_ADDR = 0x10; // 灯带1亮度
static const uint8_t RGB2_BRIGHTNESS_REG_ADDR = 0x11; // 灯带2亮度
static const uint8_t RGB_CH1_I1_COLOR_REG_ADDR = 0x20; // Channel 0, 灯带1组1, LED 0, 每个LED 4字节
static const uint8_t RGB_CH2_I1_COLOR_REG_ADDR = 0x3C; // Channel 1, 灯带1组2, LED 0, 每个LED 4字节
static const uint8_t RGB_CH3_I1_COLOR_REG_ADDR = 0x60; // Channel 2, 灯带2组1, LED 0, 每个LED 4字节
static const uint8_t RGB_CH4_I1_COLOR_REG_ADDR = 0x7C; // Channel 3, 灯带2组2, LED 0, 每个LED 4字节

static const uint8_t NUM_RGB_CHANNELS = 4; // 4个通道（2条灯带 × 2组）
static const uint8_t NUM_LEDS_PER_GROUP = 7; // 每组7个LED

class PyramidRGBComponent : public i2c::I2CDevice, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

  // 在 codegen 阶段设置初始参数（在 setup 中应用）
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

  // 设置亮度：strip=1 或 2，brightness 0..100
  bool set_strip_brightness(uint8_t strip, uint8_t brightness);

  // 设置某个通道的 RGB 颜色（0..255），会更新该通道下的所有 7 个 LED
  bool set_channel_color(uint8_t channel, uint8_t r, uint8_t g, uint8_t b);

  // 设置单一颜色分量（0..255），内部保留上次 R/G/B 值
  bool set_channel_color_component(uint8_t channel, RGBColorChannel color, uint8_t value);

  // Map FloatOutput level [0..1] to device value [0..255] using dimming/scaling
  uint8_t map_level(RGBColorChannel color, float level) const;

 private:
  // 写入颜色数据到设备
  bool write_color_block_(uint8_t base_reg_addr, const uint8_t *color_bytes, size_t len);
  // 获取通道基址并处理通道映射/索引反转
  uint8_t channel_base_addr_(uint8_t channel) const;

  // 保存每个通道的当前 RGB 值
  uint8_t channel_colors_[NUM_RGB_CHANNELS][3] = {{0}};

  // 初始参数
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
