#pragma once

#include "esphome/components/audio_dac/audio_dac.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace aw88298 {
    // AW88298 Register Addresses
static const uint8_t IDCODE = 0x00;        // ID Code Register, read only
static const uint8_t SYSST = 0x01;       // System Status Register, read only
static const uint8_t SYSINT = 0x02;      // System Interrupt Register
static const uint8_t SYSINTM = 0x03;      // System Interrupt Mask Register
static const uint8_t SYSCTRL = 0x04;      // System Control Register
static const uint8_t SYSCTRL2 = 0x05;      // System Control Register 2

static const uint8_t I2SCTRL = 0x06;     // I2S Control Register
static const uint8_t I2SCFG1 = 0x07;     // I2S Configuration Register 1
static const uint8_t HAGCCFG1 = 0x09;   // High AGC Configuration Register 1
static const uint8_t HAGCCFG2 = 0x0A;   // High AGC Configuration Register 2
static const uint8_t HAGCCFG3 = 0x0B;   // High AGC Configuration Register 3
static const uint8_t HAGCCFG4 = 0x0C;   // High AGC Configuration Register 4
static const uint8_t HAGCST = 0x10;   // High AGC Status Register, read only
static const uint8_t VDD = 0x12;        // VDD Monitor Register, read only
static const uint8_t TEMP = 0x13;       // Temperature Monitor Register, read only

static const uint8_t PVDD = 0x14;      // PVDD Monitor Register, read only

static const uint8_t BSTCTRL1 = 0x60;   // Boost Control Register 1
static const uint8_t BSTCTRL2 = 0x61;   // Boost Control Register 2

struct sr_map_t {
    uint32_t hz;    // Sample rate
    uint8_t  code;  // Corresponding bit
};
// Supported sample rate
static const sr_map_t sr_table[] = {
    {  8000,   0x0 },
    { 11025,   0x1 },
    { 12000,   0x2 },
    { 16000,   0x3 },
    { 22050,   0x4 },
    { 24000,   0x5 },
    { 32000,   0x6 },
    { 44100,   0x7 },
    { 48000,   0x8 },
};

class AW88298 : public audio_dac::AudioDac, public Component, public i2c::I2CDevice {

public:
    void setup() override;
    void loop() override;
    void dump_config() override;

    float get_setup_priority() const override { return setup_priority::DATA; }
   
    bool set_mute_off() override { return this->set_mute_state_(false); }

    /// @brief Enables mute for audio out
    /// @return True if successful and false otherwise
    bool set_mute_on() override { return this->set_mute_state_(true); }
    bool is_muted() override { return this->is_muted_; }


    float volume() override;

    bool set_volume(float volume) override;

    // Set sample rate for AW88298
    void set_sample_rate(uint32_t sample_rate) { this->sample_rate_ = sample_rate; }
protected:
    bool set_mute_state_(bool mute_state);

private:
    bool initialized_{false};
    float volume_{1.0f};
    bool is_muted_{false};
    uint32_t sample_rate_;
};

}  // namespace aw88298
}  // namespace esphome
