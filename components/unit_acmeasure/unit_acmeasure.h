#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace unit_acmeasure {

static const uint8_t UNIT_ACMEASURE_ADDR = 0x42;
static const uint8_t UNIT_ACMEASURE_VOLTAGE_REG = 0x60;
static const uint8_t UNIT_ACMEASURE_CURRENT_REG = 0x70;
static const uint8_t UNIT_ACMEASURE_POWER_REG = 0x80;
static const uint8_t UNIT_ACMEASURE_APPARENT_POWER_REG = 0x90;
static const uint8_t UNIT_ACMEASURE_POWER_FACTOR_REG = 0xA0;
static const uint8_t UNIT_ACMEASURE_KWH_REG = 0xB0;
static const uint8_t UNIT_ACMEASURE_GET_READY_REG = 0xFC;
static const uint8_t FIRMWARE_VERSION_REG = 0xFE;

class UnitACMeasureComponent : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_voltage_sensor(sensor::Sensor *sensor) { this->voltage_sensor_ = sensor; }
  void set_current_sensor(sensor::Sensor *sensor) { this->current_sensor_ = sensor; }
  void set_power_sensor(sensor::Sensor *sensor) { this->power_sensor_ = sensor; }
  void set_apparent_power_sensor(sensor::Sensor *sensor) { this->apparent_power_sensor_ = sensor; }
  void set_power_factor_sensor(sensor::Sensor *sensor) { this->power_factor_sensor_ = sensor; }
  void set_energy_sensor(sensor::Sensor *sensor) { this->energy_sensor_ = sensor; }

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void update() override;

 protected:
  bool read_register_(uint8_t reg, uint8_t *data, uint8_t len);
  bool read_u16_(uint8_t reg, uint16_t *value);
  bool read_u32_(uint8_t reg, uint32_t *value);

  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *apparent_power_sensor_{nullptr};
  sensor::Sensor *power_factor_sensor_{nullptr};
  sensor::Sensor *energy_sensor_{nullptr};
};

}  // namespace unit_acmeasure
}  // namespace esphome
