#include "powerhub_light.h"
#include "esphome/core/progmem.h"


namespace esphome {
namespace powerhub {

static const char *TAG = "powerhub.light";

/// @brief convert the float input to (0 - scale_factor) uint8_t value
/// @param value input float value
/// @param scale_factor scaling factor
/// @return uint8_t value
static uint8_t to_uint8_t(float value, int scale_factor) {
    if ( value < -1e-6f || value > 1.0f + 1e-6f ) {
        ESP_LOGW(TAG, "Invalid range, support 0.0 - 1.0 only");
    }
    return static_cast<uint8_t>(value * scale_factor);
}

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG
PROGMEM_STRING_TABLE(LightChannelStrings,
  "USB Type A Light",
  "USB Type C Light",
  "Grove Red Light",
  "Grove Blue Light",
  "RS485 CAN Light",
  "Charge Light",
  "Power Left Light",
  "Power Right Light",
  "Unknown"
);

static const LogString *power_channel_to_str(LightChannel channel) {
  return LightChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}

#endif

light::LightTraits PowerHubLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void PowerHubLight::write_state(light::LightState *state) {
  float r, g, b, brightness = 0.0f;
  state->current_values_as_rgb(&r, &g, &b);
  state->current_values_as_brightness(&brightness);
  BGR_t color {
    .b = to_uint8_t(b, 255),
    .g = to_uint8_t(g, 255),
    .r = to_uint8_t(r, 255)
  };
  switch (this->channel_) {
    case LightChannel::LED_USB_A:
      this->parent_->set_led_usb_a_color(color);
      this->parent_->set_brightness_usb_a(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_USB_C:
      this->parent_->set_led_usb_c_color(color);
      this->parent_->set_brightness_usb_c(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_GROVE_BLUE:
      this->parent_->set_led_grove_blue_color(color);
      this->parent_->set_brightness_grove_blue(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_GROVE_RED:
      this->parent_->set_led_grove_red_color(color);
      this->parent_->set_brightness_grove_red(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_RS485_CAN:
      this->parent_->set_led_rs485_can_color(color);
      this->parent_->set_brightness_rs485_can(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_CHARGE:
      this->parent_->set_led_bat_charge_color(color);
      this->parent_->set_brightness_bat_charge(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_POWER_LEFT:
      this->parent_->set_led_pwr_l_color(color);
      this->parent_->set_brightness_pwr_l(to_uint8_t(brightness, 100));
      break;
    case LightChannel::LED_POWER_RIGHT:
      this->parent_->set_led_pwr_r_color(color);
      this->parent_->set_brightness_pwr_r(to_uint8_t(brightness, 100));
      break;
    default:
      ESP_LOGW(TAG, "Unknown light channel");
      break;
  }
  state->publish_state();
}



} // namespace powerhub
} // namespace esphome
