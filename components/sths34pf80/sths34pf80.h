#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace sths34pf80 {

class STHS34PF80Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;

  void set_presence_sensor(sensor::Sensor *s) { presence_sensor_ = s; }
  void set_motion_sensor(sensor::Sensor *s) { motion_sensor_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }
  void set_pres_flag_sensor(binary_sensor::BinarySensor *s) { pres_flag_sensor_ = s; }
  void set_mot_flag_sensor(binary_sensor::BinarySensor *s) { mot_flag_sensor_ = s; }

  void set_presence_threshold(uint16_t v) { presence_threshold_ = v; }
  void set_motion_threshold(uint16_t v) { motion_threshold_ = v; }
  void set_presence_hysteresis(uint8_t v) { presence_hysteresis_ = v; }
  void set_motion_hysteresis(uint8_t v) { motion_hysteresis_ = v; }
  // ODR register value: 1=0.25Hz, 2=0.5Hz, 3=1Hz, 4=2Hz, 5=4Hz, 6=8Hz, 7=15Hz, 8=30Hz
  void set_odr(uint8_t v) { odr_ = v; }

 protected:
  bool read_bytes_(uint8_t reg, uint8_t *data, uint8_t len);
  bool write_byte_(uint8_t reg, uint8_t value);
  bool func_cfg_write_(uint8_t embedded_addr, const uint8_t *data, uint8_t len);
  bool algo_reset_();

  sensor::Sensor *presence_sensor_{nullptr};
  sensor::Sensor *motion_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  binary_sensor::BinarySensor *pres_flag_sensor_{nullptr};
  binary_sensor::BinarySensor *mot_flag_sensor_{nullptr};

  uint16_t presence_threshold_{200};  // default 0xC8 as in Arduino example
  uint16_t motion_threshold_{200};
  uint8_t presence_hysteresis_{50};   // default 0x32
  uint8_t motion_hysteresis_{50};
  uint8_t odr_{8};  // default 30 Hz – matches Arduino example; keeps frame deltas small
};

}  // namespace sths34pf80
}  // namespace esphome
