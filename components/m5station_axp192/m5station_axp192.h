#ifndef M5STATION_AXP192_H
#define M5STATION_AXP192_H

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace m5station_axp192 {

class M5StationAXP192Component : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) {
    this->battery_level_sensor_ = battery_level_sensor;
  }

  void set_brightness(float brightness) { this->brightness_ = brightness; }

  // Control LCD backlight (LDO3 enable)
  void set_backlight(bool on);

  // ========== INTERNAL METHODS ==========
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

 protected:
  sensor::Sensor *battery_level_sensor_{nullptr};
  float brightness_{1.0f};

  // Low-level register access using ESPHome I2CDevice helpers
  void write_1_byte(uint8_t reg, uint8_t value);
  uint8_t read_8_bit(uint8_t reg);
  uint16_t read_12_bit(uint8_t reg);
  uint16_t read_13_bit(uint8_t reg);
  uint16_t read_16_bit(uint8_t reg);
  uint32_t read_24_bit(uint8_t reg);
  uint32_t read_32_bit(uint8_t reg);
  void read_buffer(uint8_t reg, uint8_t size, uint8_t *buffer);

  // Configuration helpers (ported from M5Station AXP192 driver)
  void begin();
  void screen_breath(int brightness_percent);

  // ADC & battery helpers
  void set_adc_state(bool state);
  float get_battery_voltage();
  float get_battery_level();

  // Groove / bus power helpers
  void set_bus_power_mode(uint8_t state);
};

}  // namespace m5station_axp192
}  // namespace esphome

#endif  // M5STATION_AXP192_H
