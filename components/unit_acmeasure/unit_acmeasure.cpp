#include "unit_acmeasure.h"

#include "esphome/core/log.h"

namespace esphome {
namespace unit_acmeasure {

static const char *const TAG = "unit_acmeasure";

void UnitACMeasureComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Unit ACMeasure...");

  uint8_t fw = 0;
  if (!this->read_byte(FIRMWARE_VERSION_REG, &fw)) {
    ESP_LOGE(TAG, "Communication with Unit ACMeasure failed!");
    this->mark_failed();
    return;
  }

  ESP_LOGD(TAG, "Unit ACMeasure firmware version: 0x%02X", fw);
}

void UnitACMeasureComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Unit ACMeasure:");
  LOG_I2C_DEVICE(this);

  if (this->is_failed()) {
    ESP_LOGE(TAG, "  Communication with Unit ACMeasure failed!");
    return;
  }
}

void UnitACMeasureComponent::update() {
  if (this->is_failed()) {
    ESP_LOGW(TAG, "Device marked as failed, skipping update");
    return;
  }

  uint8_t ready = 0;
  if (!this->read_byte(UNIT_ACMEASURE_GET_READY_REG, &ready) || ready == 0) {
    ESP_LOGD(TAG, "Measurement data not ready, waiting...");
    return;
  }

  if (this->voltage_sensor_ != nullptr) {
    uint16_t raw = 0;
    if (this->read_u16_(UNIT_ACMEASURE_VOLTAGE_REG, &raw)) {
      float value = raw / 100.0f;  // 0.01 V resolution
      this->voltage_sensor_->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to read voltage");
    }
  }

  if (this->current_sensor_ != nullptr) {
    uint16_t raw = 0;
    if (this->read_u16_(UNIT_ACMEASURE_CURRENT_REG, &raw)) {
      float value = raw / 100.0f;  // 0.01 A resolution
      this->current_sensor_->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to read current");
    }
  }

  if (this->power_sensor_ != nullptr) {
    uint32_t raw = 0;
    if (this->read_u32_(UNIT_ACMEASURE_POWER_REG, &raw)) {
      float value = raw / 100.0f;  // 0.01 W resolution
      this->power_sensor_->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to read power");
    }
  }

  if (this->apparent_power_sensor_ != nullptr) {
    uint32_t raw = 0;
    if (this->read_u32_(UNIT_ACMEASURE_APPARENT_POWER_REG, &raw)) {
      float value = raw / 100.0f;  // 0.01 VA resolution
      this->apparent_power_sensor_->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to read apparent power");
    }
  }

  if (this->power_factor_sensor_ != nullptr) {
    uint8_t raw = 0;
    if (this->read_byte(UNIT_ACMEASURE_POWER_FACTOR_REG, &raw)) {
      float value = raw / 100.0f;  // 0.01 power factor (0.00-1.00)
      this->power_factor_sensor_->publish_state(value * 100.0f);  // publish as percent
    } else {
      ESP_LOGW(TAG, "Failed to read power factor");
    }
  }

  if (this->energy_sensor_ != nullptr) {
    uint32_t raw = 0;
    if (this->read_u32_(UNIT_ACMEASURE_KWH_REG, &raw)) {
      float value = raw / 100.0f;  // assume 0.01 kWh resolution
      this->energy_sensor_->publish_state(value);
    } else {
      ESP_LOGW(TAG, "Failed to read energy (kWh)");
    }
  }
}

bool UnitACMeasureComponent::read_register_(uint8_t reg, uint8_t *data, uint8_t len) {
  // Use ESPHome's read_bytes which performs a proper repeated-start
  // write-then-read (matching the original library's endTransmission(false)).
  return this->read_bytes(reg, data, len);
}

bool UnitACMeasureComponent::read_u16_(uint8_t reg, uint16_t *value) {
  uint8_t buf[2];
  if (!this->read_bytes(reg, buf, sizeof(buf))) {
    return false;
  }
  *value = static_cast<uint16_t>(buf[0]) | (static_cast<uint16_t>(buf[1]) << 8);
  return true;
}

bool UnitACMeasureComponent::read_u32_(uint8_t reg, uint32_t *value) {
  uint8_t buf[4];
  if (!this->read_bytes(reg, buf, sizeof(buf))) {
    return false;
  }
  *value = static_cast<uint32_t>(buf[0]) | (static_cast<uint32_t>(buf[1]) << 8) |
           (static_cast<uint32_t>(buf[2]) << 16) | (static_cast<uint32_t>(buf[3]) << 24);
  return true;
}

}  // namespace unit_acmeasure
}  // namespace esphome
