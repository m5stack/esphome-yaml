#include "m5station_axp192.h"

#include "esphome/core/log.h"

namespace esphome {
namespace m5station_axp192 {

static const char *const TAG = "m5station_axp192";

// ===== High-level Component API =====

void M5StationAXP192Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M5Station AXP192...");

  // Just try to read a simple register to verify communication
  uint8_t chip_id = this->read_8_bit(0x03);  // AXP192 chip ID register
  ESP_LOGD(TAG, "AXP192 chip ID: 0x%02X", chip_id);

  this->begin();
  
  // Force backlight to maximum brightness (100%) to ensure visibility
  this->screen_breath(100);
  ESP_LOGD(TAG, "Set backlight to 100%% brightness");
  
  // Force backlight ON immediately after initialization
  // This ensures LDO3 is enabled regardless of switch restore state
  this->set_backlight(true);
  ESP_LOGD(TAG, "Forced backlight ON after setup");

  // Debug: log key power registers related to LCD and backlight
  uint8_t reg_12 = this->read_8_bit(0x12);
  uint8_t reg_28 = this->read_8_bit(0x28);
  ESP_LOGD(TAG, "AXP192 REG 0x12 (DCDC/LDO enable): 0x%02X", reg_12);
  ESP_LOGD(TAG, "AXP192 REG 0x28 (LDO2/LDO3 voltage): 0x%02X", reg_28);
}

void M5StationAXP192Component::dump_config() {
  ESP_LOGCONFIG(TAG, "M5Station AXP192:");
  LOG_I2C_DEVICE(this);
}

float M5StationAXP192Component::get_setup_priority() const { return setup_priority::HARDWARE; }

void M5StationAXP192Component::update() {
  // Publish battery level if a sensor is configured
  if (this->battery_level_sensor_ != nullptr) {
    float level = this->get_battery_level();
    ESP_LOGD(TAG, "Battery level: %.1f%%", level);
    this->battery_level_sensor_->publish_state(level);
  }

  // Optional: map brightness (0.0-1.0) to 0-100% backlight
  int bri_percent = static_cast<int>(this->brightness_ * 100.0f);
  if (bri_percent < 0)
    bri_percent = 0;
  if (bri_percent > 100)
    bri_percent = 100;
  this->screen_breath(bri_percent);
}

// ===== Low-level I2C helpers =====

void M5StationAXP192Component::write_1_byte(uint8_t reg, uint8_t value) {
  this->write_byte(reg, value);
}

uint8_t M5StationAXP192Component::read_8_bit(uint8_t reg) {
  uint8_t data = 0;
  this->read_byte(reg, &data);
  return data;
}

uint16_t M5StationAXP192Component::read_12_bit(uint8_t reg) {
  uint8_t buf[2];
  this->read_buffer(reg, 2, buf);
  return (static_cast<uint16_t>(buf[0]) << 4) | buf[1];
}

uint16_t M5StationAXP192Component::read_13_bit(uint8_t reg) {
  uint8_t buf[2];
  this->read_buffer(reg, 2, buf);
  return (static_cast<uint16_t>(buf[0]) << 5) | buf[1];
}

uint16_t M5StationAXP192Component::read_16_bit(uint8_t reg) {
  uint8_t buf[2];
  this->read_bytes(reg, buf, sizeof(buf));
  uint16_t value = 0;
  for (size_t i = 0; i < sizeof(buf); i++) {
    value <<= 8;
    value |= buf[i];
  }
  return value;
}

uint32_t M5StationAXP192Component::read_24_bit(uint8_t reg) {
  uint8_t buf[3];
  this->read_bytes(reg, buf, sizeof(buf));
  uint32_t value = 0;
  for (size_t i = 0; i < sizeof(buf); i++) {
    value <<= 8;
    value |= buf[i];
  }
  return value;
}

uint32_t M5StationAXP192Component::read_32_bit(uint8_t reg) {
  uint8_t buf[4];
  this->read_bytes(reg, buf, sizeof(buf));
  uint32_t value = 0;
  for (size_t i = 0; i < sizeof(buf); i++) {
    value <<= 8;
    value |= buf[i];
  }
  return value;
}

void M5StationAXP192Component::read_buffer(uint8_t reg, uint8_t size, uint8_t *buffer) {
  this->read_bytes(reg, buffer, size);
}

// ===== Ported configuration from M5Station AXP192 driver =====

void M5StationAXP192Component::begin() {
  // This function is adapted from M5Station/src/AXP192.cpp::begin().
  // The I2C bus (SDA/SCL pins, frequency) is configured by ESPHome.

  // AXP192 30H - disable VBUS current limit while keeping other bits
  this->write_1_byte(0x30, (this->read_8_bit(0x30) & 0x04) | 0x02);
  ESP_LOGD(TAG, "axp: vbus limit off");

  // Disable DCDC2, enable EXTEN, LDO2, LDO3, DCDC1
  // 0x4D = 0b01001101: bit6=EXTEN, bit3=LDO3, bit2=LDO2, bit0=DCDC1
  this->write_1_byte(0x12, this->read_8_bit(0x12) | 0x4D);

  // Set LDO2 & LDO3 voltages in register 0x28
  // LDO2 (high 4 bits): LCD logic power - 3.0V  → value=(3000/100)-18=12=0x0C
  // LDO3 (low  4 bits): LCD backlight      - 3.0V  → value=(3000/100)-18=12=0x0C
  // Using 3.0V for both, same as M5Station factory default 0xCC
  this->write_1_byte(0x28, 0xCC);
  ESP_LOGD(TAG, "axp: LDO2 (LCD logic) = 3.0V, LDO3 (backlight) = 3.0V  [0x28=0xCC]");

  // Set ESP32 power voltage (DCDC1) to 3.35V
  {
    uint16_t mv = 3350;
    if (mv >= 3000 && mv <= 3400) {
      // DCDC1 control register 0x26
      uint8_t step = (mv < 700) ? 0 : (mv - 700) / 25;
      this->write_1_byte(0x26, (this->read_8_bit(0x26) & 0x80) | (step & 0x7F));
    }
  }
  ESP_LOGD(TAG, "axp: esp32 power voltage set to 3.35V");

  // GPIO0-4 are used to control Groove port power
  // GPIO0: select NMOS open drain mode
  this->write_1_byte(0x90, (this->read_8_bit(0x90) & 0b11111000) | 0b00000000);
  // GPIO1: select NMOS open drain mode
  this->write_1_byte(0x92, (this->read_8_bit(0x92) & 0b11111000) | 0b00000000);
  // GPIO2: select NMOS open drain mode
  this->write_1_byte(0x93, (this->read_8_bit(0x93) & 0b11111000) | 0b00000000);
  // GPIO4/GPIO3: select NMOS open drain mode, enable GPIO functionality (MSB)
  this->write_1_byte(0x95, (this->read_8_bit(0x95) & 0b01110000) | 0b10000101);

  // Turn power on for all Groove ports by default (bitfield 0b00011111)
  uint8_t data1 = this->read_8_bit(0x94) & 0b11111000;
  data1 |= 0b00000111;
  this->write_1_byte(0x94, data1);
  uint8_t data2 = this->read_8_bit(0x96) & 0b11111100;
  data2 |= 0b00000011;
  this->write_1_byte(0x96, data2);

  // Enable battery detection
  this->write_1_byte(0x32, 0x46);

  // Set temperature protection
  this->write_1_byte(0x39, 0xFC);

  // 512ms power on time, 4s power off time, 1s long keystroke time
  // Auto shutdown when key time exceeds boot time
  this->write_1_byte(0x36, 0x4C);

  // Enable Bat, ACIN, VBUS, APS ADCs
  this->write_1_byte(0x82, 0xFF);

  // Check VBUS status and configure BUS_5V source
  if (this->read_8_bit(0x00) & 0x08) {
    // VBUS present, disable M-Bus 5V output to input
    this->write_1_byte(0x30, this->read_8_bit(0x30) | 0x80);
    this->set_bus_power_mode(1);  // external input
  } else {
    // No VBUS, enable internal 5V boost
    this->set_bus_power_mode(0);
  }
}

void M5StationAXP192Component::screen_breath(int brightness_percent) {
  if (brightness_percent < 0)
    brightness_percent = 0;
  if (brightness_percent > 100)
    brightness_percent = 100;

  // Map 0-100% to 2500-3300mV (SetLcdVoltage requires >= 2500mV)
  int vol_mv = (brightness_percent * (3300 - 2500)) / 100 + 2500;
  if (vol_mv < 2500)
    vol_mv = 2500;
  if (vol_mv > 3300)
    vol_mv = 3300;

  // value = (voltage/100) - 18, same as AXP192::SetLDOVoltage
  uint8_t value = (uint8_t)((vol_mv / 100) - 18);
  // Preserve LDO2 (high 4 bits) while updating LDO3 (low 4 bits)
  this->write_1_byte(0x28, (this->read_8_bit(0x28) & 0xF0) | (value & 0x0F));
}

void M5StationAXP192Component::set_adc_state(bool state) {
  this->write_1_byte(0x82, state ? 0xFF : 0x00);
}

float M5StationAXP192Component::get_battery_voltage() {
  // Same formula as M5Station AXP192::GetBatVoltage()
  const float lsb = 1.1f / 1000.0f;
  uint16_t raw = this->read_12_bit(0x78);
  return raw * lsb;
}

float M5StationAXP192Component::get_battery_level() {
  const float bat_voltage = this->get_battery_voltage();
  const float bat_percentage = (bat_voltage < 3.248088f) ? 0.0f : (bat_voltage - 3.120712f) * 100.0f;
  if (bat_percentage < 0.0f)
    return 0.0f;
  if (bat_percentage > 100.0f)
    return 100.0f;
  return bat_percentage;
}

void M5StationAXP192Component::set_backlight(bool on) {
  // Mirror M5Station AXP192::SetLDO3 behaviour: bit 3 of reg 0x12
  // IMPORTANT: use explicit uint8_t mask to avoid sign-extension bug with ~(1<<3)
  uint8_t reg = this->read_8_bit(0x12);
  if (on) {
    reg |= (uint8_t) 0x08;   // set bit3
  } else {
    reg &= (uint8_t) 0xF7;   // clear bit3 (0xF7 = ~0x08 as uint8_t)
  }
  this->write_1_byte(0x12, reg);
}

void M5StationAXP192Component::set_bus_power_mode(uint8_t state) {
  // Port of AXP192::SetBusPowerMode from M5Station
  uint8_t data = this->read_8_bit(0x10);
  if (state == 0) {
    // Use internal boost: set EXTEN bit
    data |= 0x04;
  } else {
    // Powered externally: clear EXTEN bit
    data &= static_cast<uint8_t>(~0x04);
  }
  this->write_1_byte(0x10, data);
}

}  // namespace m5station_axp192
}  // namespace esphome
