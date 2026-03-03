#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"
#include "../m5stack_chain_encoder.h"

namespace esphome {
namespace m5stack_chain_encoder {

static const char *const TAG_ENCODER_OUTPUT = "m5stack_chain_encoder.output";

// 通道定义参照 unit_step16 OutputChannel：亮度 + RGB 颜色
enum LedChannel : uint8_t {
  LED_BRIGHTNESS = 0,
  RGB_BRIGHTNESS = 1,
  RGB_RED = 2,
  RGB_GREEN = 3,
  RGB_BLUE = 4,
};

class ChainEncoderLedOutput : public output::FloatOutput, public Component {
 public:
  void set_parent(ChainEncoderSensor *parent) { parent_ = parent; }
  void set_channel(LedChannel channel) { channel_ = channel; }

  void write_state(float state) override {
    if (parent_ == nullptr) {
      ESP_LOGW(TAG_ENCODER_OUTPUT, "Parent is null!");
      return;
    }

    if (state < 0.0f)
      state = 0.0f;
    if (state > 1.0f)
      state = 1.0f;

    uint8_t op_status = 0;

    ESP_LOGD(TAG_ENCODER_OUTPUT, "Channel %d: write_state(%.2f)", static_cast<int>(channel_), state);

    switch (channel_) {
      case LED_BRIGHTNESS: {
        uint8_t brightness = static_cast<uint8_t>(state * 100.0f);

        ChainStatus status = parent_->set_led_brightness(brightness, &op_status);

        ESP_LOGD(TAG_ENCODER_OUTPUT, "Set LED brightness= %u, status=0x%02X, op_status=%u", brightness,
                 static_cast<uint8_t>(status), op_status);

        if (status != CHAIN_OK) {
          ESP_LOGW(TAG_ENCODER_OUTPUT, "Failed to set LED brightness (status=0x%02X)", static_cast<uint8_t>(status));
        }

        // 设置亮度时，默认给一个白色，保证能看见
        if (brightness > 0) {
          parent_->set_rgb_color(255, 255, 255, &op_status);
        }
        break;
      }
      case RGB_BRIGHTNESS: {
        uint8_t brightness = static_cast<uint8_t>(state * 100.0f);

        ChainStatus status = parent_->set_led_brightness(brightness, &op_status);

        ESP_LOGD(TAG_ENCODER_OUTPUT, "Set RGB brightness= %u, status=0x%02X, op_status=%u", brightness,
                 static_cast<uint8_t>(status), op_status);

        if (status != CHAIN_OK) {
          ESP_LOGW(TAG_ENCODER_OUTPUT, "Failed to set RGB brightness (status=0x%02X)", static_cast<uint8_t>(status));
        }
        break;
      }
      case RGB_RED:
      case RGB_GREEN:
      case RGB_BLUE: {
        uint8_t value = static_cast<uint8_t>(state * 255.0f);

        // 每次都读取设备当前颜色（和 unit_step16 一样），避免多个 output 实例之间的竞态条件
        uint8_t r = 0, g = 0, b = 0;
        if (parent_->get_rgb_color(&r, &g, &b, &op_status) != CHAIN_OK) {
          ESP_LOGW(TAG_ENCODER_OUTPUT, "Failed to get current RGB, using defaults");
          r = g = b = 0;
        }

        // 只修改当前通道
        if (channel_ == RGB_RED) {
          r = value;
        } else if (channel_ == RGB_GREEN) {
          g = value;
        } else if (channel_ == RGB_BLUE) {
          b = value;
        }

        ESP_LOGD(TAG_ENCODER_OUTPUT, "Setting RGB color: R=%u, G=%u, B=%u", r, g, b);

        ChainStatus status = parent_->set_rgb_color(r, g, b, &op_status);

        if (status != CHAIN_OK) {
          ESP_LOGW(TAG_ENCODER_OUTPUT, "Failed to set RGB color (status=0x%02X)", static_cast<uint8_t>(status));
        }
        break;
      }
      default:
        break;
    }
  }

 protected:
  ChainEncoderSensor *parent_{nullptr};
  LedChannel channel_{LED_BRIGHTNESS};
};

}  // namespace m5stack_chain_encoder
}  // namespace esphome
