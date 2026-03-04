#include "si12t.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"
#include <cassert>
#include <cstdint>


namespace esphome {
namespace si12t {

static const char *TAG = "si12t";

// SI12T registers
// static const uint8_t REG_SEN1 = 0x02;
// static const uint8_t REG_SEN2 = 0x03;
// static const uint8_t REG_SEN3 = 0x04;
// static const uint8_t REG_SEN4 = 0x05;
// static const uint8_t REG_SEN5 = 0x06;
// static const uint8_t REG_SEN6 = 0x07;
static const uint8_t REG_CFG = 0x08;
static const uint8_t REG_CTRL = 0x09;
static const uint8_t REG_REF_RST_1 = 0x0A;
static const uint8_t REG_REF_RST_2 = 0x0B;
static const uint8_t REG_CH_ON_1 = 0x0C;
static const uint8_t REG_CH_ON_2 = 0x0D;
static const uint8_t REG_CAL_ON_1 = 0x0E;
static const uint8_t REG_CAL_ON_2 = 0x0F;
// static const uint8_t REG_OUTPUT_1 = 0x10;
// static const uint8_t REG_OUTPUT_2 = 0x11;
// static const uint8_t REG_OUTPUT_3 = 0x12;
static const uint8_t REG_UNLOCK_CTRL_2 = 0x3B;
static const uint8_t REG_CTRL2 = 0x3D;

static const uint8_t CHANNEL_SENSITIVITY_REGS[12] = {
    0x02, 0x02,  // REG_SEN1: DATA_CH1 (Lower 4 bits), DATA_CH2 (upper 4 bits)
    0x03, 0x03,  // REG_SEN2: DATA_CH3 (Lower 4 bits), DATA_CH4 (upper 4 bits)
    0x04, 0x04,  // REG_SEN3: DATA_CH5 (Lower 4 bits), DATA_CH6 (upper 4 bits)
    0x05, 0x05,  // REG_SEN4: DATA_CH7 (Lower 4 bits), DATA_CH8  (upper 4 bits)
    0x06, 0x06,  // REG_SEN5: DATA_CH9 (Lower 4 bits), DATA_CH10  (upper 4 bits)
    0x07, 0x07   // REG_SEN6: DATA_CH11 (Lower 4 bits), DATA_CH12  (upper 4 bits)
};

static const uint8_t CHANNEL_OUTPUT_REGS[12] = {
    0x10, 0x10,  // REG_OUTPUT_1: DATA_OUT1 (0:1), DATA_OUT2 (2:3)
    0x10, 0x10,  // REG_OUTPUT_1: DATA_OUT3 (4:5), DATA_OUT4 (6:7)
    0x11, 0x11,  // REG_OUTPUT_2: DATA_OUT5 (0:1), DATA_OUT6 (2:3)
    0x11, 0x11,  // REG_OUTPUT_2: DATA_OUT7 (4:5), DATA_OUT8 (6:7)
    0x12, 0x12,  // REG_OUTPUT_3: DATA_OUT9 (0:1), DATA_OUT10 (2:3)
    0x12, 0x12   // REG_OUTPUT_3: DATA_OUT11 (4:5), DATA_OUT12 (6:7)
};

#define SI12T_ERR_FAILED(func)   \
    do {                         \
        if (!func) {             \
            this->mark_failed(); \
            return;              \
        }                        \
    } while(0)

PROGMEM_STRING_TABLE(SI12TFTTStrings,
  "FTT_5S",
  "FTT_10S",
  "FTT_15S",
  "FTT_20S",
  "FTT_UNKNOWN"
);

PROGMEM_STRING_TABLE(SI12TINTRLevelStrings,
  "OUTPUT_MEDIUM_HIGH",
  "OUTPUT_ALL",
  "OUTPUT_MEDIUM_HIGH_ALT", // same as OUTPUT_MEDIUM_HIGH, registers were different
  "OUTPUT_HIGH",
  "OUTPUT_UNKNOWN"
);

PROGMEM_STRING_TABLE(SI12TResponseTimeStrings,
  "RES_CYCLE_2",
  "RES_CYCLE_3",
  "RES_CYCLE_4",
  "RES_CYCLE_5",
  "RES_CYCLE_6",
  "RES_CYCLE_7",
  "RES_CYCLE_8",
  "RES_CYCLE_9",
  "RES_CYCLE_UNKNOWN"
);

static const LogString *first_touch_time_to_string(SI12TFirstTouchTime time) {
  return SI12TFTTStrings::get_log_str(static_cast<uint8_t>(time), SI12TFTTStrings::LAST_INDEX);
}

static const LogString *intr_level_to_string(SI12TInterruptLevel level) {
  return SI12TINTRLevelStrings::get_log_str(static_cast<uint8_t>(level), SI12TINTRLevelStrings::LAST_INDEX);
}

static const LogString *res_time_to_string(SI12TResponseTime time) {
  return SI12TResponseTimeStrings::get_log_str(static_cast<uint8_t>(time), SI12TResponseTimeStrings::LAST_INDEX);
}


void SI12TComponent::setup() {
    ESP_LOGD(TAG, "Setting up SI12T touch sensor");

    // software reset and sleep mode configuration
    if ( this->reset_ ) {
        SI12T_ERR_FAILED(this->write_byte(REG_CTRL, 0x10));
    }

    // write control register
    uint8_t ctrl_data;
    SI12T_ERR_FAILED(this->read_byte(REG_CTRL, &ctrl_data));

    if ( this->sleep_mode_ ) {
        ctrl_data |= (1 << 2);
    } else {
        ctrl_data &= ~(1 << 2);
    }

    SI12T_ERR_FAILED(this->write_byte(REG_CTRL, ctrl_data));

    // write configuration register
    uint8_t cfg_data =
        ((auto_mode_ ? 1 : 0) << 7) |
        ((ftc_ & 0x03) << 5) |
        ((ilc_ & 0x03) << 3) |
        (rtc_ & 0x07);

    SI12T_ERR_FAILED(this->write_byte(REG_CFG, cfg_data));

    ESP_LOGD(TAG, "Setup SI12T success.");
}


void SI12TComponent::loop() {

}

void SI12TComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "SI12T configuration:");
    LOG_I2C_DEVICE(this);
    ESP_LOGCONFIG(TAG, "  Auto mode: %s\n"
                       "  First touch time: %s\n"
                       "  Output interrupt level: %s\n"
                       "  Response time: %s\n"
                       "  Software reset: %s\n"
                       "  Sleep mode: %s",
                       TRUEFALSE(this->auto_mode_),
                       LOG_STR_ARG(first_touch_time_to_string(this->ftc_)),
                       LOG_STR_ARG(intr_level_to_string(this->ilc_)),
                       LOG_STR_ARG(res_time_to_string(this->rtc_)),
                       TRUEFALSE(this->reset_),
                       TRUEFALSE(this->sleep_mode_));
}


void SI12TComponent::write_channel_sensitivity(SI12TChannel channel, SI12TSensitivityLevel sensitivity, bool extend_threshold) {

    uint8_t reg = CHANNEL_SENSITIVITY_REGS[channel];

    uint8_t shift;
    if (channel % 2 == 0) {
        shift = 0;
    } else {
        shift = 4;
    }

    // read original value
    uint8_t current_value;
    if(!this->read_byte(reg, &current_value)) {
        this->status_set_warning(LOG_STR("Failed to read channel sensitivity"));
        return;
    }

    if ( shift == 0 ) {
        uint8_t l_value = (sensitivity << 1) | static_cast<uint8_t>(extend_threshold);
        current_value &= 0xF0;
        if(!this->write_byte(reg, current_value | l_value)) {
            this->status_set_warning(LOG_STR("Failed to set channel sensitivity"));
            return;
        }
    } else {
        uint8_t h_value = (sensitivity << 1) | static_cast<uint8_t>(extend_threshold);
        current_value &= 0x0F;
        if(!this->write_byte(reg, current_value | (h_value << shift))) {
            this->status_set_warning(LOG_STR("Failed to set channel sensitivity"));
            return;
        }
    }

    this->status_clear_warning();
}

uint8_t SI12TComponent::read_channel_output(SI12TChannel channel) {

    uint8_t reg = CHANNEL_OUTPUT_REGS[channel];

    uint8_t bit_offset = (channel % 4) * 2;

    uint8_t data;

    if ( !this->read_byte(reg, &data) ){
        this->status_set_warning(LOG_STR("Failed to read output register"));
        return 0;
    }

    this->status_clear_warning();

    return (data >> bit_offset) & 0x03;
}


void SI12TComponent::setup_channel(SI12TChannel channel) {

    uint8_t index = channel / 8;
    uint8_t bitpos = channel % 8;
    
    // Set reference value reset control register
    // read original value
    uint8_t ref_data[2];
    if( !this->read_bytes(REG_REF_RST_1, ref_data, 2) ) {
        this->status_set_warning(LOG_STR("Failed to read reference value reset control register."));
        return;
    }
    uint8_t ch_en_data[2];
    if ( !this->read_bytes(REG_CH_ON_1, ch_en_data, 2) ) {
        this->status_set_warning(LOG_STR("Failed to read channel sensitivity control register."));
        return;
    }

    uint8_t ch_calib_data[2];
    if ( !this->read_bytes(REG_CAL_ON_1, ch_calib_data, 2) ) {
        this->status_set_warning(LOG_STR("Failed to read calibrate control register"));
        return;
    }

    // This part follow the default settings of CH1, which is enabled by default
    // clear bit, 0 - not enable reference value reset, 1 - enable reference value reset
    ref_data[index] &= ~(1 << bitpos); 
    // clear bit, 0 - enable operation ( sensing + calibrating ), 1 - hold operations ( no sensing + stop calibrating )
    ch_en_data[index] &= ~(1 << bitpos);
    // clear bit, 0 - enable reference value reset ( sensing + calibrating ), 1 - not enable reference value reset ( no sensing + calibrating )
    ch_calib_data[index] &= ~(1 << bitpos);


    // write back values to enable corresponding channel
    if ( !this->write_bytes(REG_REF_RST_1, ref_data, 2) ){
        this->status_set_warning(LOG_STR("Failed to write calibrate control register"));
        return;
    }

    if ( !this->write_bytes(REG_CH_ON_1, ref_data, 2) ){
        this->status_set_warning(LOG_STR("Failed to write channel sensitivity control register."));
        return;
    }

    if ( !this->write_bytes(REG_CAL_ON_1, ref_data, 2) ){
        this->status_set_warning(LOG_STR("Failed to write calibrate control register"));
        return;
    }

    this->status_clear_warning();

    ESP_LOGD(TAG, "Setup channel %hhu complete.", channel);
}

} // namespace si12t
} // namespace esphome