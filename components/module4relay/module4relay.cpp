#include "module4relay.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"

namespace esphome {
namespace module4relay {

static const char *TAG = "module4relay";
static const uint8_t REG_RELAY_CTRL = 0x10;

void Module4Relay::setup() {
  ESP_LOGD(TAG, "Setup complete...");
}

void Module4Relay::dump_config() {
  LOG_I2C_DEVICE(this);
#ifdef USE_SWITCH
  ESP_LOGCONFIG(TAG, "Switch:");
  LOG_SWITCH("  ", "Relay 1", this->relay_1_switch_);
  LOG_SWITCH("  ", "Relay 2", this->relay_2_switch_);
  LOG_SWITCH("  ", "Relay 3", this->relay_3_switch_);
  LOG_SWITCH("  ", "Relay 4", this->relay_4_switch_);
#endif
}

uint8_t Module4Relay::read_all_relay() {
  uint8_t val = 0;
  if (!this->read_byte(REG_RELAY_CTRL, &val)) {
    ESP_LOGW(TAG, "Failed to read relay status");
  }
  return val;
}

void Module4Relay::set_all_relay(bool en) {
  // Match M5Module-4Relay behavior: write 0xFF for on, 0x00 for off.
  uint8_t val = en ? 0xFF : 0x00;
  if (!this->write_byte(REG_RELAY_CTRL, val)) {
    ESP_LOGW(TAG, "Failed to write all relay status %s", ONOFF(en));
  }
}

void Module4Relay::set_relay_by_index(uint8_t index, bool en) {
  // Index is 0-3, mapping to relay channels 1-4.
  if (index > 3) {
    ESP_LOGE(TAG, "Relay index %u out of range (valid: 0-3)", index);
    return;
  }
  uint8_t val = this->read_all_relay();
  if (en) {
    val |= (1 << index);
  } else {
    val &= ~(1 << index);
  }

  if (!this->write_byte(REG_RELAY_CTRL, val)) {
    ESP_LOGW(TAG, "Error when setting relay %d with state %s", index + 1, ONOFF(en));
  }
}

}  // namespace module4relay
}  // namespace esphome
