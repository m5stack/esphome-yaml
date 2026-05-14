/*
 * SPDX-FileCopyrightText: 2026 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"


namespace esphome::dual_kmeter {

enum KmeterChannel : uint8_t {
  KMETER_1 = 0,
  KMETER_2 = 1,
  INTERNAL = 2
};

enum KmeterUnit : uint8_t {
  CELSIUS,
  FAHRENHEIT
};


class DualKmeterComponent : public sensor::Sensor,
                            public i2c::I2CDevice,
                            public PollingComponent
{

 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  // Check if the measurement is ready
  bool is_data_ready();

  // set the unit of measurement sensor would display
  void set_kmeter_unit(KmeterUnit unit) { this->unit_ = unit; }

  // set kmeter channel
  void set_kmeter_channel(KmeterChannel channel) { this->channel_ = channel; }

  // Methods for numeric sensors
  float get_thermocouple_temp();
  float get_internal_temp();

  // Useful if you want to print the text results on LCD/OLED
  // Alternatively you can create template text sensor to read these values
  // Here we just stick to numeric sensors
  const char* get_thermocouple_temp_str();
  const char* get_internal_temp_str();

 protected:
  optional<int32_t> read_thermocouple_temp_();
  optional<int32_t> read_internal_temp_();
  
  bool write_kmeter_select_();

  bool read_ready_flag_();

  bool read_thermocouple_temp_str_();
  bool read_internal_temp_str_();
  void format_temperature_string_(const uint8_t *src, char *dst);


  char temp_str_buf_[9]{};
  char internal_temp_str_buf_[9]{};

  // indicate current channel
  KmeterUnit unit_{CELSIUS};
  KmeterChannel channel_{KMETER_1};
  uint8_t firm_ver_{0};

};
  
} // namespace esphome::dual_kmeter


