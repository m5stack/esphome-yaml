#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace i2c_joystick_2 {

enum Axis : uint8_t {
  AXIS_X = 0,
  AXIS_Y = 1,
};

enum OutputMode : uint8_t {
  MODE_ADC_16BIT = 0,
  MODE_ADC_8BIT = 1,
  MODE_OFFSET_12BIT = 2,
  MODE_OFFSET_8BIT = 3,
};

class I2CJoystick2Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;

  bool read_axis_value(uint8_t axis, uint8_t mode, float *value);
  bool read_button_pressed(bool *pressed);

 protected:
  bool read_u8_(uint8_t reg, uint8_t *value);
  bool read_le_u16_(uint8_t reg, uint16_t *value);
  bool read_le_i16_(uint8_t reg, int16_t *value);
  bool read_i8_(uint8_t reg, int8_t *value);

  uint8_t firmware_version_{0};
};

class I2CJoystick2Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_parent(I2CJoystick2Component *parent) { this->parent_ = parent; }
  void set_axis(uint8_t axis) { this->axis_ = axis; }
  void set_mode(uint8_t mode) { this->mode_ = mode; }

  void update() override;
  void dump_config() override;

 protected:
  I2CJoystick2Component *parent_{nullptr};

  uint8_t axis_{AXIS_X};
  uint8_t mode_{MODE_OFFSET_12BIT};
};

class I2CJoystick2BinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void set_parent(I2CJoystick2Component *parent) { this->parent_ = parent; }

  void update() override;
  void dump_config() override;

 protected:
  I2CJoystick2Component *parent_{nullptr};
};

}  // namespace i2c_joystick_2
}  // namespace esphome
