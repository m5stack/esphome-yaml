#include "si5351_esphome.h"

namespace esphome {
namespace si5351 {

#define SI5351_ERROR_FAILED(func) \
  if (!(func)) {                 \
    this->mark_failed();         \
    return;                      \
  }

static const char *const TAG = "si5351";

void Si5351Component::setup() {
  delay(1000);
  ESP_LOGD(TAG, "Setting up Si5351 Clock Generator...");

  uint8_t buffer[8];

  // Disable all outputs
  SI5351_ERROR_FAILED(this->write_byte(3, 0xFF));

  // Power down output drivers (CLK0~CLK2)
  buffer[0] = 0x80;
  buffer[1] = 0x80;
  buffer[2] = 0x80;
  SI5351_ERROR_FAILED(this->write_bytes(16, buffer, 3));

  // Crystal Internal Load Capacitance
  // Internal CL = 10 pF (default)
  SI5351_ERROR_FAILED(this->write_byte(183, 0xC0));

  // Multisynth NA Parameters
  buffer[0] = 0xFF;
  buffer[1] = 0xFD;
  buffer[2] = 0x00;
  buffer[3] = 0x09;
  buffer[4] = 0x26;
  buffer[5] = 0xF7;
  buffer[6] = 0x4F;
  buffer[7] = 0x72;
  SI5351_ERROR_FAILED(this->write_bytes(26, buffer, 8));

  // Multisynth1 Parameters
  buffer[0] = 0x00;
  buffer[1] = 0x01;
  buffer[2] = 0x00;
  buffer[3] = 0x2F;
  buffer[4] = 0x00;
  buffer[5] = 0x00;
  buffer[6] = 0x00;
  buffer[7] = 0x00;
  SI5351_ERROR_FAILED(this->write_bytes(50, buffer, 8));

  // CLK1 Control
  // Bit6: Integer mode
  // Bits5-4: Multisynth1
  SI5351_ERROR_FAILED(this->write_byte(17, (3 << 2) | (1 << 6)));  // 0x4C

  // PLL Reset
  SI5351_ERROR_FAILED(this->write_byte(177, 0xA0));

  // Enable all outputs
  SI5351_ERROR_FAILED(this->write_byte(3, 0x00));

  ESP_LOGI(TAG, "Si5351 Clock Generator initialized @0x%02X", this->address_);
  this->setup_complete_ = true;
}

void Si5351Component::dump_config() {
  ESP_LOGCONFIG(TAG, "Si5351 Clock Generator:");

  LOG_I2C_DEVICE(this);

  ESP_LOGCONFIG(TAG, "  Setup Status: %s",
                this->setup_complete_ ? "OK" : "FAILED");
}

}  // namespace si5351
}  // namespace esphome