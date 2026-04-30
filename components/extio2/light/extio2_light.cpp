#include "extio2_light.h"
#include "esphome/core/progmem.h"
#include "../extio2.h"

namespace esphome {
namespace extio2 {

static const char *TAG = "extio2.light";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(LightChannelStrings,
  "Light 0",
  "Light 1",
  "Light 2",
  "Light 3",
  "Light 4",
  "Light 5",
  "Light 6",
  "Light 7",
  "Unknown Light"
);

static const LogString *light_channel_to_string(EXTIO2LightChannel channel) {
  return LightChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}

#endif

static uint8_t to_uint8_t(float value, int scale_factor) {
    if ( value < -1e-6f || value > 1.0f + 1e-6f ) {
        ESP_LOGW(TAG, "Invalid range, support 0.0 - 1.0 only");
        return 0;
    }
    return static_cast<uint8_t>(value * scale_factor);
}


void EXTIO2Light::setup() {
  // enable RGB mode for selected pin
  this->parent_->set_pin_func(static_cast<uint8_t>(this->channel_), PIN_RGB);
  delay(20);
}

void EXTIO2Light::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 Light: \n"
                     "  Light Channel %s\n",
                     LOG_STR_ARG(light_channel_to_string(this->channel_)));
}

light::LightTraits EXTIO2Light::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

void EXTIO2Light::write_state(light::LightState *state) {
    float r, g, b;
    float brightness;

    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    uint8_t pin, u8_r, u8_g, u8_b;
    pin  = static_cast<uint8_t>(this->channel_);
    u8_r = to_uint8_t(r, 255);
    u8_g = to_uint8_t(g, 255);
    u8_b = to_uint8_t(b, 255);

    this->parent_->set_led_color(pin, u8_r, u8_g, u8_b);
    // there is no brightness control register
    state->publish_state();
}


/**
 * @brief Initializes the addressable light component by allocating memory and configuring pins.
 * 
 * This function performs the following operations:
 * 1. Allocates memory for the LED color buffer (RGB channels for each LED)
 * 2. Allocates memory for effect data storage
 * 3. Configures pin functionality for addressable LEDs
 * 
 * Memory Allocation:
 * - LED buffer size: num_leds_ * 3 bytes (for R, G, B channels)
 * - Effect data buffer size: num_leds_ bytes
 * - If allocation fails, the component is marked as failed
 * 
 * Pin Configuration:
 * - Addressable LEDs must start from pin 0 and use consecutive pins
 * - If num_leds_ < 8: individually configure each pin (0 to num_leds_ - 1 ) to RGB mode
 * - If num_leds_ >= 8: configure all pins to RGB mode
 * 
 * @note This function should be called during component initialization.
 * @note Addressable LED pins must be contiguous starting from pin 0.
 * 
 * @return void
 */
void EXTIO2AddressableLight::setup() {
  RAMAllocator<uint8_t> allocator;
  // Allocate memory for addressable light buffer
  // with the size of (number_of_led * RGB channel)
  this->buf_ = allocator.allocate(this->num_leds_ * 3);
  if (this->buf_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate buffer.");
    this->mark_failed();
    return;
  }
  memset(this->buf_, 0x00, this->num_leds_ * 3);

  this->effect_data_ = allocator.allocate(this->num_leds_);

  if (this->effect_data_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate effect data.");
    this->mark_failed();
    return;
  }

  memset(this->effect_data_, 0x00, this->num_leds_);
  
  // set pin functionality
  if ( this->num_leds_ < 8 ) {
    // set the given pins to led mode
    for (size_t i = 0; i < this->num_leds_; ++i) {
      this->parent_->set_pin_func(static_cast<uint8_t>(i), PIN_RGB);
    }
  } else {
    // set all pins to led mode
    this->parent_->set_pin_func(PIN_RGB);
  }
}

void EXTIO2AddressableLight::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 Addressable Light: \n"
                     "  Number of LEDs %d\n",
                     this->num_leds_);
}

light::LightTraits EXTIO2AddressableLight::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

void EXTIO2AddressableLight::write_state(light::LightState *state) {
  uint8_t *buf = this->buf_;
  size_t len = this->num_leds_ * 3;
  // burst write data
  this->parent_->set_led_color(buf, len);
}

void EXTIO2AddressableLight::clear_effect_data() {
  memset(this->effect_data_, 0x00, this->num_leds_);
}

light::ESPColorView EXTIO2AddressableLight::get_view_internal(int32_t index) const {
  size_t pos = index * 3;

  return {
      this->buf_ + pos,
      this->buf_ + pos + 1,
      this->buf_ + pos + 2,
      nullptr,
      this->effect_data_ + index,
      &this->correction_,
  };
}

}  // namespace extio2
}  // namespace esphome