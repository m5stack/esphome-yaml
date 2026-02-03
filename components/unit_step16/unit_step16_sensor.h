#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace unit_step16 {

// Register addresses
static const uint8_t STEP16_VALUE_REG = 0x00;
static const uint8_t STEP16_LED_CONFIG_REG = 0x10;
static const uint8_t STEP16_LED_BRIGHTNESS_REG = 0x20;
static const uint8_t STEP16_SWITCH_REG = 0x30;
static const uint8_t STEP16_RGB_CONFIG_REG = 0x40;
static const uint8_t STEP16_RGB_BRIGHTNESS_REG = 0x41;
static const uint8_t STEP16_RGB_VALUE_REG = 0x50;
static const uint8_t STEP16_R_VALUE_REG = 0x50;
static const uint8_t STEP16_G_VALUE_REG = 0x51;
static const uint8_t STEP16_B_VALUE_REG = 0x52;
static const uint8_t STEP16_SAVE_FLASH_REG = 0xF0;
static const uint8_t STEP16_VERSION_REG = 0xFE;
static const uint8_t STEP16_ADDRESS_REG = 0xFF;

class UnitStep16Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Set initial values (called during configuration)
  void set_initial_led_brightness(uint8_t brightness) { initial_led_brightness_ = brightness; }
  void set_initial_led_enabled(bool enabled) { initial_led_enabled_ = enabled; }
  void set_initial_rgb_brightness(uint8_t brightness) { initial_rgb_brightness_ = brightness; }
  void set_initial_rgb_enabled(bool enabled) { initial_rgb_enabled_ = enabled; }
  void set_initial_rgb_color(uint8_t r, uint8_t g, uint8_t b) {
    initial_rgb_r_ = r;
    initial_rgb_g_ = g;
    initial_rgb_b_ = b;
  }

  // Read encoder value (0-15)
  uint8_t get_value();

  // LED control
  bool set_led_config(uint8_t config);
  uint8_t get_led_config();
  bool set_led_brightness(uint8_t brightness);
  uint8_t get_led_brightness();

  // Rotation direction control
  bool set_switch_state(uint8_t state);
  uint8_t get_switch_state();

  // RGB control
  bool set_rgb_config(uint8_t config);
  uint8_t get_rgb_config();
  bool set_rgb_brightness(uint8_t brightness);
  uint8_t get_rgb_brightness();
  bool set_rgb(uint8_t r, uint8_t g, uint8_t b);
  bool get_rgb(uint8_t *r, uint8_t *g, uint8_t *b);

  // Flash operations
  bool save_to_flash(uint8_t save);

  // Device information
  uint8_t get_version();

 protected:
  bool read_register_(uint8_t reg_addr, uint8_t *data, uint8_t len);
  bool write_register_(uint8_t reg_addr, uint8_t *data, uint8_t len);

  // Initial configuration values
  uint8_t initial_led_brightness_{50};
  bool initial_led_enabled_{true};
  uint8_t initial_rgb_brightness_{50};
  bool initial_rgb_enabled_{true};
  uint8_t initial_rgb_r_{0};
  uint8_t initial_rgb_g_{255};
  uint8_t initial_rgb_b_{0};
};

class UnitStep16Sensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_parent(UnitStep16Component *parent) { this->parent_ = parent; }
  void update() override;
  void dump_config() override;

 protected:
  UnitStep16Component *parent_;
};

}  // namespace unit_step16
}  // namespace esphome
