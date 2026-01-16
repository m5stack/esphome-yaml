#include "unit4relay.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"

namespace esphome {
namespace unit4relay {

static const char *TAG = "unit4relay";
static const uint8_t REG_MODE_CTRL = 0x10;
static const uint8_t REG_RELAY_CTRL = 0x11;

void Unit4Relay::setup() {
    // reset to default
    if ( !this->write_byte(REG_MODE_CTRL, 0x01) ) {
        ESP_LOGE(TAG,"Error when setting the LED mode");
        this->mark_failed();
    }

    if ( !this->write_byte(REG_RELAY_CTRL, 0x00) ) {
        ESP_LOGE(TAG, "Error when resetting relay status");
        this->mark_failed();
    }

    ESP_LOGD(TAG, "Setup complete...");
}

void Unit4Relay::dump_config() {
    LOG_I2C_DEVICE(this);
#ifdef USE_SWITCH
    ESP_LOGCONFIG(TAG, "Switch:");
    LOG_SWITCH("  ", "Relay 1", this->relay_1_switch_);
    LOG_SWITCH("  ", "Relay 2", this->relay_2_switch_);
    LOG_SWITCH("  ", "Relay 3", this->relay_3_switch_);
    LOG_SWITCH("  ", "Relay 4", this->relay_4_switch_);
#endif
}

uint8_t Unit4Relay::read_all_relay() {
    uint8_t val;
    if ( !this->read_byte(REG_RELAY_CTRL, &val) ) {
        ESP_LOGW(TAG, "Failed to read relay status");
    }
    // first 4 bits are LED status   ( val & 0xF0 )
    // last 4 bits are relay status  ( val & 0x0F )
    return val;
}


void Unit4Relay::set_all_relay(bool en) {
    uint8_t val = en ? 0xFF : 0x00;
    if ( !this->write_byte(REG_RELAY_CTRL, val) ) {
        ESP_LOGW(TAG, "Failed to write all relay status %s", ONOFF(en));
    }
}

void Unit4Relay::set_relay_by_index(uint8_t index, bool en) {
    // read original states
    // index is 0 - 3 ( for mask operations )
    // corresponding relay 1 - 4
    uint8_t val = this->read_all_relay();
    if ( en ) {
        val |= (1 << index);          // bit set
    } else {
        val &= ~(1 << index);         // bit clear
    }
    // write bits
    if ( !this->write_byte(REG_RELAY_CTRL, val) ) {
        ESP_LOGW(TAG, "Error when setting relay %d with state %s", index + 1, ONOFF(en));
    }

}

} // namespace unit4relay    
} // namespace esphome
