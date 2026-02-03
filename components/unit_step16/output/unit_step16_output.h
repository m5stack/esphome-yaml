#pragma once

#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"
#include "../unit_step16_sensor.h"

namespace esphome {
namespace unit_step16 {

static const char *const TAG_OUTPUT = "unit_step16.output";

// Output channel enumeration
enum OutputChannel {
  LED_BRIGHTNESS = 0,
  RGB_BRIGHTNESS = 1,
  RGB_RED = 2,
  RGB_GREEN = 3,
  RGB_BLUE = 4,
};

class UnitStep16Output : public output::FloatOutput, public Component {
 public:
  void set_parent(UnitStep16Component *parent) { parent_ = parent; }
  void set_channel(OutputChannel channel) { channel_ = channel; }

  void write_state(float state) override {
    if (parent_ == nullptr) {
      ESP_LOGW(TAG_OUTPUT, "Parent is null!");
      return;
    }

    ESP_LOGD(TAG_OUTPUT, "Channel %d: write_state(%.2f)", channel_, state);

    // Convert 0.0-1.0 to appropriate range
    switch (channel_) {
      case LED_BRIGHTNESS: {
        // Convert to 0-100
        uint8_t brightness = static_cast<uint8_t>(state * 100.0f);
        
        ESP_LOGD(TAG_OUTPUT, "Setting LED: config=0xFF, brightness=%d", brightness);
        
        // IMPORTANT: Enable LED display when setting brightness
        // 0xFF = always on (according to source code)
        if (!parent_->set_led_config(0xFF)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set LED config");
        }
        if (!parent_->set_led_brightness(brightness)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set LED brightness");
        }
        break;
      }
      case RGB_BRIGHTNESS: {
        // Convert to 0-100
        uint8_t brightness = static_cast<uint8_t>(state * 100.0f);
        
        ESP_LOGD(TAG_OUTPUT, "Setting RGB brightness: config=1, brightness=%d", brightness);
        
        // IMPORTANT: Enable RGB when setting brightness
        // 1 = on (according to source code)
        if (!parent_->set_rgb_config(1)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set RGB config");
        }
        if (!parent_->set_rgb_brightness(brightness)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set RGB brightness");
        }
        break;
      }
      case RGB_RED:
      case RGB_GREEN:
      case RGB_BLUE: {
        // Convert to 0-255 and update the corresponding color component
        uint8_t value = static_cast<uint8_t>(state * 255.0f);
        
        // IMPORTANT: Enable RGB before setting color
        if (!parent_->set_rgb_config(1)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set RGB config for color");
        }
        
        // Get current RGB values
        uint8_t r, g, b;
        if (!parent_->get_rgb(&r, &g, &b)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to get current RGB, using defaults");
          r = g = b = 0;
        }
        
        // Update the specific channel
        if (channel_ == RGB_RED) {
          r = value;
        } else if (channel_ == RGB_GREEN) {
          g = value;
        } else if (channel_ == RGB_BLUE) {
          b = value;
        }
        
        ESP_LOGD(TAG_OUTPUT, "Setting RGB color: R=%d, G=%d, B=%d", r, g, b);
        
        // Write back all three values
        if (!parent_->set_rgb(r, g, b)) {
          ESP_LOGW(TAG_OUTPUT, "Failed to set RGB color");
        }
        break;
      }
    }
  }

 protected:
  UnitStep16Component *parent_{nullptr};
  OutputChannel channel_{LED_BRIGHTNESS};
};

}  // namespace unit_step16
}  // namespace esphome
