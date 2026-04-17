#include "i2c_joystick_2.h"

#include "esphome/core/log.h"

namespace esphome {
namespace i2c_joystick_2 {

static const char *const TAG = "i2c_joystick_2";

static const uint8_t JOYSTICK2_ADC_VALUE_12BITS_REG = 0x00;
static const uint8_t JOYSTICK2_ADC_VALUE_8BITS_REG = 0x10;
static const uint8_t JOYSTICK2_BUTTON_REG = 0x20;
static const uint8_t JOYSTICK2_OFFSET_ADC_VALUE_12BITS_REG = 0x50;
static const uint8_t JOYSTICK2_OFFSET_ADC_VALUE_8BITS_REG = 0x60;
static const uint8_t JOYSTICK2_FIRMWARE_VERSION_REG = 0xFE;
static const uint8_t JOYSTICK2_LIGHT_B = 0x30;
static const uint8_t JOYSTICK2_LIGHT_G = 0x31;
static const uint8_t JOYSTICK2_LIGHT_R = 0x32;

void I2CJoystick2Component::setup() {
  ESP_LOGD(TAG, "Setting up I2C Joystick2...");
  // Read firmware version
  if ( !read_u8_(JOYSTICK2_FIRMWARE_VERSION_REG, &this->firmware_version_) ) {
    this->mark_failed(LOG_STR("Fail to communicate with device"));
    return;
  }

}

bool I2CJoystick2Component::read_axis_value(uint8_t axis, uint8_t mode, float *value) {
  if (value == nullptr) {
    return false;
  }

  if (mode == MODE_ADC_16BIT) {
    uint16_t raw = 0;
    uint8_t reg = (axis == AXIS_X) ? JOYSTICK2_ADC_VALUE_12BITS_REG : (JOYSTICK2_ADC_VALUE_12BITS_REG + 2);
    if (!this->read_le_u16_(reg, &raw)) {
      return false;
    }
    *value = static_cast<float>(raw);
    return true;
  }

  if (mode == MODE_ADC_8BIT) {
    uint8_t raw = 0;
    uint8_t reg = (axis == AXIS_X) ? JOYSTICK2_ADC_VALUE_8BITS_REG : (JOYSTICK2_ADC_VALUE_8BITS_REG + 1);
    if (!this->read_u8_(reg, &raw)) {
      return false;
    }
    *value = static_cast<float>(raw);
    return true;
  }

  if (mode == MODE_OFFSET_8BIT) {
    int8_t mapped = 0;
    uint8_t reg = (axis == AXIS_X) ? JOYSTICK2_OFFSET_ADC_VALUE_8BITS_REG
                                          : (JOYSTICK2_OFFSET_ADC_VALUE_8BITS_REG + 1);
    if (!this->read_i8_(reg, &mapped)) {
      return false;
    }
    *value = static_cast<float>(mapped);
    return true;
  }

  if (mode == MODE_OFFSET_12BIT) {
    int16_t mapped = 0;
    uint8_t reg = (axis == AXIS_X) ? JOYSTICK2_OFFSET_ADC_VALUE_12BITS_REG
                                          : (JOYSTICK2_OFFSET_ADC_VALUE_12BITS_REG + 2);
    if (!this->read_le_i16_(reg, &mapped)) {
      return false;
    }
    *value = static_cast<float>(mapped);
    return true;
  }

  return false;
}

bool I2CJoystick2Component::read_button_pressed(bool *pressed) {
  if (pressed == nullptr) {
    return false;
  }

  uint8_t button = 1;
  if (!this->read_u8_(JOYSTICK2_BUTTON_REG, &button)) {
    return false;
  }

  // M5Unit Joystick2 returns 0 when pressed and 1 when released.
  *pressed = (button == 0);
  return true;
}

void I2CJoystick2Component::write_rgb_channel(RGBChannel channel, uint8_t value) {
  uint8_t reg = 0;
  switch (channel) {
    case CHANNEL_R:
      reg = JOYSTICK2_LIGHT_R;
      break;
    case CHANNEL_G:
      reg = JOYSTICK2_LIGHT_G;
      break;
    case CHANNEL_B:
      reg = JOYSTICK2_LIGHT_B;
      break;
    default:
      ESP_LOGW(TAG, "No RGB Channel specified");
      break;
  }

  bool ret = this->write_u8_(reg, value);

  if ( !ret ) {
    ESP_LOGW(TAG, "Failed to write data to RGB channel");
    return;
  } 

}

bool I2CJoystick2Component::read_u8_(uint8_t reg, uint8_t *value) {
  return this->read_byte(reg, value);
}

bool I2CJoystick2Component::write_u8_(uint8_t reg, uint8_t value) {
  return this->write_byte(reg, value);
}

bool I2CJoystick2Component::read_le_u16_(uint8_t reg, uint16_t *value) {
  uint8_t data[2] = {0};
  if (this->read_register(reg, data, 2) != i2c::ERROR_OK) {
    return false;
  }
  *value = static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8);
  return true;
}

bool I2CJoystick2Component::read_le_i16_(uint8_t reg, int16_t *value) {
  uint16_t raw = 0;
  if (!this->read_le_u16_(reg, &raw)) {
    return false;
  }
  *value = static_cast<int16_t>(raw);
  return true;
}

bool I2CJoystick2Component::read_i8_(uint8_t reg, int8_t *value) {
  uint8_t raw = 0;
  if (!this->read_u8_(reg, &raw)) {
    return false;
  }
  *value = static_cast<int8_t>(raw);
  return true;
}

void I2CJoystick2Component::dump_config() {
  ESP_LOGCONFIG(TAG, "I2C Joystick2:");
  LOG_I2C_DEVICE(this);

  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with Joystick2 failed");
    return;
  }

  ESP_LOGCONFIG(TAG, "I2C Joystick2: \n"
                     "  Firmware version: %u", 
                     this->firmware_version_);
}

void I2CJoystick2Sensor::update() {
  if (this->parent_ == nullptr || this->parent_->is_failed()) {
    ESP_LOGW(TAG, "Joystick2 parent component unavailable");
    return;
  }

  float value = 0.0f;
  if (!this->parent_->read_axis_value(this->axis_, this->mode_, &value)) {
    ESP_LOGW(TAG, "Failed reading joystick axis value");
    return;
  }

  this->publish_state(value);
}

void I2CJoystick2Sensor::dump_config() {
  LOG_SENSOR("", "Joystick2 Axis", this);

  ESP_LOGCONFIG(TAG, "  Axis: %s\n"
                     "  Mode: %u", 
                     this->axis_ == AXIS_X ? "X" : "Y",
                     this->mode_);
}

void I2CJoystick2BinarySensor::update() {
  if (this->parent_ == nullptr || this->parent_->is_failed()) {
    ESP_LOGW(TAG, "Joystick2 parent component unavailable");
    return;
  }

  bool pressed = false;
  if (!this->parent_->read_button_pressed(&pressed)) {
    ESP_LOGW(TAG, "Failed reading joystick button value");
    return;
  }

  this->publish_state(pressed);
}

void I2CJoystick2BinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Joystick2 Button", this);
}

}  // namespace i2c_joystick_2
}  // namespace esphome
