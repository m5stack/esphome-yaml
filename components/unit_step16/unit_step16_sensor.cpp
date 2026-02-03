#include "unit_step16_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace unit_step16 {

static const char *const TAG = "unit_step16";

void UnitStep16Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Unit Step16...");
  
  // Test if device is connected
  if (!this->read_register_(STEP16_VERSION_REG, nullptr, 0)) {
    ESP_LOGE(TAG, "Communication with Unit Step16 failed!");
    this->mark_failed();
    return;
  }
  
  uint8_t version = this->get_version();
  ESP_LOGD(TAG, "Unit Step16 firmware version: 0x%02X", version);
  
  // Apply initial configuration
  ESP_LOGD(TAG, "Applying initial configuration...");
  
  // Configure LED
  if (this->initial_led_enabled_) {
    this->set_led_config(0xFF);  // Always on
    this->set_led_brightness(this->initial_led_brightness_);
    ESP_LOGD(TAG, "LED enabled with brightness: %d%%", this->initial_led_brightness_);
  } else {
    this->set_led_config(0x00);  // Off
    ESP_LOGD(TAG, "LED disabled");
  }
  
  // Configure RGB
  if (this->initial_rgb_enabled_) {
    this->set_rgb_config(1);  // On
    this->set_rgb_brightness(this->initial_rgb_brightness_);
    this->set_rgb(this->initial_rgb_r_, this->initial_rgb_g_, this->initial_rgb_b_);
    ESP_LOGD(TAG, "RGB enabled: brightness=%d%%, color=(%d,%d,%d)", 
             this->initial_rgb_brightness_, 
             this->initial_rgb_r_, this->initial_rgb_g_, this->initial_rgb_b_);
  } else {
    this->set_rgb_config(0);  // Off
    ESP_LOGD(TAG, "RGB disabled");
  }
}

void UnitStep16Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Unit Step16:");
  LOG_I2C_DEVICE(this);
  
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with Unit Step16 failed!");
    return;
  }
  
  uint8_t version = this->get_version();
  ESP_LOGCONFIG(TAG, "  Firmware Version: 0x%02X", version);
}

bool UnitStep16Component::read_register_(uint8_t reg_addr, uint8_t *data, uint8_t len) {
  if (len == 0) {
    // Just test communication
    return this->write(&reg_addr, 1) == i2c::ERROR_OK;
  }
  
  if (this->write(&reg_addr, 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write register address 0x%02X", reg_addr);
    return false;
  }
  
  if (this->read(data, len) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read %d bytes from register 0x%02X", len, reg_addr);
    return false;
  }
  
  return true;
}

bool UnitStep16Component::write_register_(uint8_t reg_addr, uint8_t *data, uint8_t len) {
  uint8_t buffer[len + 1];
  buffer[0] = reg_addr;
  memcpy(&buffer[1], data, len);
  
  if (this->write(buffer, len + 1) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to write %d bytes to register 0x%02X", len, reg_addr);
    return false;
  }
  
  return true;
}

uint8_t UnitStep16Component::get_value() {
  uint8_t value = 0;
  this->read_register_(STEP16_VALUE_REG, &value, 1);
  return value;
}

bool UnitStep16Component::set_led_config(uint8_t config) {
  return this->write_register_(STEP16_LED_CONFIG_REG, &config, 1);
}

uint8_t UnitStep16Component::get_led_config() {
  uint8_t config = 0;
  this->read_register_(STEP16_LED_CONFIG_REG, &config, 1);
  return config;
}

bool UnitStep16Component::set_led_brightness(uint8_t brightness) {
  if (brightness > 100) brightness = 100;
  return this->write_register_(STEP16_LED_BRIGHTNESS_REG, &brightness, 1);
}

uint8_t UnitStep16Component::get_led_brightness() {
  uint8_t brightness = 0;
  this->read_register_(STEP16_LED_BRIGHTNESS_REG, &brightness, 1);
  return brightness;
}

bool UnitStep16Component::set_switch_state(uint8_t state) {
  return this->write_register_(STEP16_SWITCH_REG, &state, 1);
}

uint8_t UnitStep16Component::get_switch_state() {
  uint8_t state = 0;
  this->read_register_(STEP16_SWITCH_REG, &state, 1);
  return state;
}

bool UnitStep16Component::set_rgb_config(uint8_t config) {
  return this->write_register_(STEP16_RGB_CONFIG_REG, &config, 1);
}

uint8_t UnitStep16Component::get_rgb_config() {
  uint8_t config = 0;
  this->read_register_(STEP16_RGB_CONFIG_REG, &config, 1);
  return config;
}

bool UnitStep16Component::set_rgb_brightness(uint8_t brightness) {
  if (brightness > 100) brightness = 100;
  return this->write_register_(STEP16_RGB_BRIGHTNESS_REG, &brightness, 1);
}

uint8_t UnitStep16Component::get_rgb_brightness() {
  uint8_t brightness = 0;
  this->read_register_(STEP16_RGB_BRIGHTNESS_REG, &brightness, 1);
  return brightness;
}

bool UnitStep16Component::set_rgb(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t rgb[3] = {r, g, b};
  return this->write_register_(STEP16_RGB_VALUE_REG, rgb, 3);
}

bool UnitStep16Component::get_rgb(uint8_t *r, uint8_t *g, uint8_t *b) {
  uint8_t rgb[3];
  if (!this->read_register_(STEP16_RGB_VALUE_REG, rgb, 3)) {
    return false;
  }
  *r = rgb[0];
  *g = rgb[1];
  *b = rgb[2];
  return true;
}

bool UnitStep16Component::save_to_flash(uint8_t save) {
  bool result = this->write_register_(STEP16_SAVE_FLASH_REG, &save, 1);
  delay(50);  // Wait for the save operation to complete
  return result;
}

uint8_t UnitStep16Component::get_version() {
  uint8_t version = 0;
  this->read_register_(STEP16_VERSION_REG, &version, 1);
  return version;
}

// UnitStep16Sensor implementation
void UnitStep16Sensor::update() {
  if (this->parent_->is_failed()) {
    ESP_LOGW(TAG, "Unit Step16 component failed, skipping sensor update");
    return;
  }
  
  uint8_t value = this->parent_->get_value();
  ESP_LOGD(TAG, "Unit Step16 encoder value: %d", value);
  this->publish_state(value);
}

void UnitStep16Sensor::dump_config() {
  LOG_SENSOR("", "Unit Step16 Sensor", this);
  ESP_LOGCONFIG(TAG, "  Update Interval: %u ms", this->get_update_interval());
}

}  // namespace unit_step16
}  // namespace esphome
