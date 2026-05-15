#include "m5unit_encoder.h"

#include "esphome/core/log.h"

namespace esphome {
namespace m5unit_encoder {

static const char *const TAG = "m5unit_encoder";

void M5UnitEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M5Unit Encoder...");
}

void M5UnitEncoder::dump_config() {
  LOG_SENSOR("", "M5Unit Encoder", this);
  LOG_I2C_DEVICE(this);
}

void M5UnitEncoder::update() {
  int16_t value = this->read_encoder_value_();
  this->publish_state(value);

  if (this->button_sensor_ != nullptr) {
    bool pressed = this->read_button_status_();
    this->button_sensor_->publish_state(pressed);
  }
}

int16_t M5UnitEncoder::read_encoder_value_() {
  uint8_t data[2];
  if (!this->read_bytes(ENCODER_REG, data, 2)) {
    ESP_LOGW(TAG, "Failed to read encoder value");
    return 0;
  }
  return (int16_t)((data[0]) | (data[1]) << 8);
}

bool M5UnitEncoder::read_button_status_() {
  uint8_t data;
  if (!this->read_byte(BUTTON_REG, &data)) {
    ESP_LOGW(TAG, "Failed to read button status");
    return false;
  }
  return data != 0;
}

void M5UnitEncoder::set_led_color(uint8_t index, uint32_t color) {
  uint8_t data[4];
  data[0] = index;
  data[1] = (color >> 16) & 0xFF;
  data[2] = (color >> 8) & 0xFF;
  data[3] = color & 0xFF;
  if (!this->write_bytes(RGB_LED_REG, data, 4)) {
    ESP_LOGW(TAG, "Failed to set LED color");
  }
}

void M5UnitEncoder::set_led_channel(uint8_t led_index, LedColorChannel channel, float state) {
  if (led_index > 1)
    return;

  if (state < 0.0f)
    state = 0.0f;
  if (state > 1.0f)
    state = 1.0f;

  this->led_color_[led_index][channel] = (uint8_t)(state * 255.0f);
  this->write_led_color_(led_index);
}

// Firmware I2C index mapping: 1=LED0, 2=LED1 (0=broadcast)
static const uint8_t LED_I2C_INDEX[2] = {1, 2};

void M5UnitEncoder::write_led_color_(uint8_t index) {
  uint8_t i2c_index = LED_I2C_INDEX[index];
  uint8_t data[4];
  data[0] = i2c_index;
  data[1] = this->led_color_[index][LED_RED];
  data[2] = this->led_color_[index][LED_GREEN];
  data[3] = this->led_color_[index][LED_BLUE];
  if (!this->write_bytes(RGB_LED_REG, data, 4)) {
    ESP_LOGW(TAG, "Failed to write LED color for index %u", index);
  }
}

void M5UnitEncoder::set_work_mode(uint8_t mode) {
  if (!this->write_byte(MODE_REG, mode)) {
    ESP_LOGW(TAG, "Failed to set work mode");
  }
}

}  // namespace m5unit_encoder
}  // namespace esphome
