/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#include "dual_kmeter_sensor.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cstddef>
#include <cstdint>

namespace esphome::dual_kmeter {

static constexpr uint8_t REG_TEMP_CELSIUS                    = 0x00;
static constexpr uint8_t REG_TEMP_FAHRENHEIT                 = 0x04;
static constexpr uint8_t REG_INTERNAL_TEMP_CELSIUS           = 0x10;
static constexpr uint8_t REG_INTERNAL_TEMP_FAHRENHEIT        = 0x14;
static constexpr uint8_t REG_KMETER_SEL                      = 0x20;
static constexpr uint8_t REG_KMETER_READY_FLAG               = 0x30;
static constexpr uint8_t REG_TEMP_CELSIUS_STRING             = 0x40;
static constexpr uint8_t REG_TEMP_FAHRENHEIT_STRING          = 0x50;
static constexpr uint8_t REG_INTERNAL_TEMP_CELSIUS_STRING    = 0x60;
static constexpr uint8_t REG_INTERNAL_TEMP_FAHRENHEIT_STRING = 0x70;
static constexpr uint8_t REG_FIRMWARE_VERSION                = 0xFE;

static const char *TAG = "dual_kmeter";

void DualKmeterComponent::setup() {
  // read firmware version
  uint8_t firm_ver;
  if ( !this->read_byte(REG_FIRMWARE_VERSION, &firm_ver) ) {
    ESP_LOGW(TAG, "Failed to read firmware version, there might be communication issue.");
  }
  delay(20);

}

void DualKmeterComponent::update() {

  // Select the measuring channel
  float val = NAN;
  this->write_kmeter_select_();

  if (this->channel_ == KmeterChannel::INTERNAL) {
    val = this->get_internal_temp();
  } else {
    // wait for sampling
    uint32_t start = millis();
    while ( !this->is_data_ready() ) {
    if ( millis() - start > 300 ) {
      ESP_LOGW(TAG, "Kmeter sampling timeout");
      return;
    }
      delay(1);
    }
    val = this->get_thermocouple_temp();
  }

  // publish result
  this->publish_state(val);

}

void DualKmeterComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Module Dual Kmeter: \n");
}

bool DualKmeterComponent::is_data_ready() {
  return this->read_ready_flag_();
}

float DualKmeterComponent::get_thermocouple_temp() {
  auto temp = this->read_thermocouple_temp_();
  
  if ( !temp.has_value() ) {
    return NAN;
  }
  
  return temp.value() * 0.01f;
}

float DualKmeterComponent::get_internal_temp() {
  auto temp = this->read_internal_temp_();

  if ( !temp.has_value() ) {
    return NAN;
  }

  return temp.value() * 0.01f;
}

const char* DualKmeterComponent::get_thermocouple_temp_str()
{
  if (!this->read_thermocouple_temp_str_()) {
    return "Unknown";
  }
  return this->temp_str_buf_;
}

const char* DualKmeterComponent::get_internal_temp_str()
{
  if (!this->read_internal_temp_str_()) {
    return "Unknown";
  }
  return this->internal_temp_str_buf_;
}

optional<int32_t> DualKmeterComponent::read_thermocouple_temp_() {

  uint8_t reg_start = ( this->unit_ == KmeterUnit::CELSIUS) ? 
                      REG_TEMP_CELSIUS : REG_TEMP_FAHRENHEIT;

  uint8_t data[4];
  if (!this->read_bytes(reg_start, data, 4)) {
    this->status_set_warning(LOG_STR("Failed to read temperature register"));
    return {};
  }

  this->status_clear_warning();

  return
      (int32_t(data[3]) << 24) |
      (int32_t(data[2]) << 16) |
      (int32_t(data[1]) << 8) |
      int32_t(data[0]);
}

optional<int32_t> DualKmeterComponent::read_internal_temp_() {

  uint8_t reg_start = ( this->unit_ == KmeterUnit::CELSIUS) ? 
                      REG_INTERNAL_TEMP_CELSIUS : REG_INTERNAL_TEMP_FAHRENHEIT;
      
  uint8_t data[4];
  if (!this->read_bytes(reg_start, data, 4)) {
    this->status_set_warning(LOG_STR("Failed to read temperature register"));
    return {};
  }

  this->status_clear_warning();

  return
      (int32_t(data[3]) << 24) |
      (int32_t(data[2]) << 16) |
      (int32_t(data[1]) << 8) |
      int32_t(data[0]);
}

bool DualKmeterComponent::write_kmeter_select_() {

  // internal channel has no value
  if ( this->channel_ == KmeterChannel::INTERNAL ) {
    return true; 
  }

  uint8_t data = this->channel_;

  if ( !this->write_byte(REG_KMETER_SEL, data) ) {
    this->status_set_warning(LOG_STR("Failed to write Kmeter select register"));
    return false;
  }

  this->status_clear_warning();
  return true;
}

bool DualKmeterComponent::read_ready_flag_() {
  uint8_t data;

  if ( !this->read_byte(REG_KMETER_READY_FLAG, &data) ) {
    this->status_set_warning(LOG_STR("Failed to read ready flag register"));
    return false;
  }

  this->status_clear_warning();

  return data == 0;
}

bool DualKmeterComponent::read_thermocouple_temp_str_() {

  uint8_t reg_start = (this->unit_ == KmeterUnit::CELSIUS) ? 
                      REG_TEMP_CELSIUS_STRING : REG_TEMP_FAHRENHEIT_STRING;

  uint8_t read_buf[8];

  if ( !this->read_bytes(reg_start, read_buf, 8) ) {
    this->status_set_warning(LOG_STR("Failed to read temperature string registers"));
    return false;
  }

  this->format_temperature_string_(read_buf, this->temp_str_buf_);

  return true;
}

bool DualKmeterComponent::read_internal_temp_str_() {

  uint8_t reg_start = (this->unit_ == KmeterUnit::CELSIUS) ? 
                      REG_INTERNAL_TEMP_CELSIUS_STRING : REG_INTERNAL_TEMP_FAHRENHEIT_STRING;

  uint8_t read_buf[8];
  if ( !this->read_bytes(reg_start, read_buf, 8) ) {
    this->status_set_warning(LOG_STR("Failed to read temperature string registers"));
    return false;
  }


  this->format_temperature_string_(read_buf, this->internal_temp_str_buf_);

  this->status_clear_warning();
  return true;
}


void DualKmeterComponent::format_temperature_string_(const uint8_t *src, char *dst) {
  size_t out = 0;

  // if sign is '+', remove the sign
  if ( src[0] == '-' ) {
    dst[out++] = '-';
  }

  size_t first_digit = 1;
  while ( first_digit < 5 && src[first_digit] == '0' ) {
    first_digit++;
  }

  if ( first_digit == 5 ) {
    dst[out++] = '0';
  } else {
    for ( size_t i = first_digit; i < 5; i++ ) {
      dst[out++] = src[i];
    }
  }  

  dst[out++] = src[5];
  dst[out++] = src[6];
  dst[out++] = src[7];
  dst[out] = '\0';
}

} // namespace esphome::dual_kmeter