#include "aw88298.h"

namespace esphome {
namespace aw88298 {

static const char *TAG = "aw88298";

#define AW88298_ERROR_CHECK(err, msg) \
  if (!(err)) { \
    ESP_LOGE(TAG, msg); \
    this->mark_failed(); \
    return; \
  }

void AW88298::setup() {
    ESP_LOGCONFIG(TAG, "Setting up AW88298...");

    uint16_t id_code;
    AW88298_ERROR_CHECK(this->read_byte_16(IDCODE, &id_code), "Failed to read AW88298 ID Code, please check your connections.");
    // uint8_t id_code;
    // AW88298_ERROR_CHECK(this->read_byte(IDCODE, &id_code), "Failed to read AW88298 ID Code, please check your connections.");
    // ESP_LOGI(TAG, "AW88298 IDCODE: 0x%04X", id_code);

    // Reset the device, all configuration registers will be reset to default value after 0x55aa is written
    AW88298_ERROR_CHECK(this->write_byte_16(IDCODE, 0x55aa), "Failed to reset AW88298");
    
    // I2SEN=1 AMPPD=0 PWDN=0
    AW88298_ERROR_CHECK(this->write_byte_16(SYSCTRL,  0x4040), "Failed to enable I2S and power the device.");
    // RMSE=0 HAGCE=0 HDCCE=0 HMUTE=0
    AW88298_ERROR_CHECK(this->write_byte_16(SYSCTRL2, 0x0008), "Failed to set SYSCTRL2.");
    // I2S Control
    AW88298_ERROR_CHECK(this->write_byte_16(I2SCTRL, 0x0673), "Failed to set I2S.");
    // boost mode disabled 
    AW88298_ERROR_CHECK(this->write_byte_16(BSTCTRL2, 0x0673), "Failed to disable the boost mode for AW88298");

    // Configure Sample Rate
    uint8_t i2ssr_code = 0x0; // Default 8kHz

    for (auto &entry : sr_table) {
        if (this->sample_rate_ == entry.hz) {
            i2ssr_code = entry.code;
            break;
        }
    }

    // Other combinations
    // Bit13 INPLEV=0, Bit12 I2SRXEN=1, Bit11:10 CHSEL=01, Bit9:8 I2SMD=00,
    // Bit7:6 I2SFS=3(32bit), Bit5:4 I2SBCK=00

    uint16_t reg06 = (0 << 13) // INPLEV
                        | (1 << 12) // I2SRXEN
                        | (1 << 10) // CHSEL=01 Default
                        | (0 <<  8) // I2SMD=00 Default
                        | (3 <<  6) // I2SFS=32bit Default
                        | (0 <<  4) // I2SBCK=32×fs
                        | (i2ssr_code);

    AW88298_ERROR_CHECK(this->write_byte_16(I2SCTRL, reg06), "Failed to configure sample rate and clock.");


    // Set the maximum volume by default.
    this->set_volume(1.0f);

    ESP_LOGCONFIG(TAG, "Setup finished.");
}


void AW88298::loop() {
    // Nothing to do here for now
}


void AW88298::dump_config() {
    ESP_LOGCONFIG(TAG, "AW88298:");
    LOG_I2C_DEVICE(this);
    if (this->initialized_) {
        ESP_LOGCONFIG(TAG, "  Initialized: YES");
    } else {
        ESP_LOGCONFIG(TAG, "  Initialized: NO");
    }
}



float AW88298::volume() {
    uint16_t reg0c;
    this->read_byte_16(HAGCCFG4, &reg0c);
    uint8_t vol_reg = (reg0c >> 8) & 0xFF; // Volume

    // Calculate the real volume
    int coarse = (vol_reg >> 4) & 0x0F;
    int fine   = vol_reg & 0x0F;
    float dB = -((float)coarse * 6.0f + (float)fine * 0.5f);

    // normalization
    float norm = 1.0f - (fabsf(dB) / 96.0f);
    norm = std::clamp(norm, 0.0f, 1.0f);

    return norm;
}

bool AW88298::set_volume(float volume) {

    this->volume_ = volume;

    float norm = std::clamp(volume, 0.0f, 1.0f);

    float target_dB = -(1.0f - norm) * 96.0f;
    float abs_db = fabsf(target_dB);

    int coarse = (int)(abs_db / 6.0f);
    int fine = static_cast<int>(std::roundf((abs_db - coarse * 6.0f) / 0.5f)); 

    uint16_t reg0c = (coarse << 12) | (fine << 8) | 0x64;

    return this->write_byte_16(HAGCCFG4, reg0c);
}

bool AW88298::set_mute_state_(bool mute_state) {
    this->is_muted_ = mute_state;
    uint16_t reg05;

    this->read_byte_16(SYSCTRL2, &reg05);

    if ( mute_state ) {
        reg05 |= (1 << 5);   // Enable Hardware mute
        return this->write_byte_16(SYSCTRL2, reg05);
    }
    return true;
}


} // namespace aw88298
} // namespace esphome