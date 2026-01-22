#include "lp5562.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <cstdint>


#define LP5562_ERROR_CHECK(expr) \
    do {                         \
        if (!(expr)) { \
            this->mark_failed();       \
            return;                    \
        }                              \
    } while(0)

#define LP5562_WARN_CHECK(expr) \
    do {                         \
        if (!(expr)) { \
            ESP_LOGW(TAG, "Error when read/write register."); \
        }                              \
    } while(0)


namespace esphome {
namespace lp5562 {

static const char *TAG = "lp5562";
static const uint8_t REG_ENABLE       = 0x00;
static const uint8_t REG_OP_MODE      = 0x01;
static const uint8_t REG_B_PWM        = 0x02;
static const uint8_t REG_G_PWM        = 0x03;
static const uint8_t REG_R_PWM        = 0x04;
static const uint8_t REG_B_CURRENT    = 0x05;
static const uint8_t REG_G_CURRENT    = 0x06;
static const uint8_t REG_R_CURRENT    = 0x07;
static const uint8_t REG_CONFIG       = 0x08;
static const uint8_t REG_ENG_1_PC     = 0x09;
static const uint8_t REG_ENG_2_PC     = 0x0A;
static const uint8_t REG_ENG_3_PC     = 0x0B;
static const uint8_t REG_STATUS       = 0x0C;
static const uint8_t REG_RESET        = 0x0D;
static const uint8_t REG_W_PWM        = 0x0E;
static const uint8_t REG_W_CURRENT    = 0x0F;
static const uint8_t REG_LED_MAP      = 0x70;



void LP5562Component::setup() {
    // software reset
    LP5562_ERROR_CHECK(this->write_byte(REG_RESET, 0xFF));

    // chip enable
    LP5562_ERROR_CHECK(this->write_byte(REG_ENABLE, 0x40));

    delayMicroseconds(500); // enable operation takes > 488us to update

    // LP5562 operation mode configurations
    uint8_t config_val;

    if (this->use_internal_clk_) {
        config_val |= BIT(0); // Bit 0 = 1 to enable the internal clock
    }

    if (this->power_save_enable_) {
        config_val |= BIT(5); // Bit 5 = 1 to enable the power save mode
    }

    if (this->high_pwm_freq_enable_) {
        config_val |= BIT(6); // Bit 6 = 1 for 588 Hz
    }

    LP5562_ERROR_CHECK(this->write_byte(REG_CONFIG, config_val));

    delayMicroseconds(200); // According to datasheet, operation mode change takes around 153 us

    // LP5562 logarithmic dimming
    uint8_t curr_enable;
    LP5562_ERROR_CHECK(this->read_byte(REG_ENABLE, &curr_enable));
    if (this->logarithmic_dimming_) {
        curr_enable |= BIT(7); // Bit 7 = 1
    } else {
        curr_enable &= ~BIT(7); // Clear bit
    }

    LP5562_ERROR_CHECK(this->write_byte(REG_ENABLE, curr_enable));
    delayMicroseconds(500); // enable operation takes > 488us to update

    // Controlled via I2C PWM registers
    // The LP5562 reserved the engine to run
    // custom program but it's not used here
    this->set_all_led_mapping_(0);

    delayMicroseconds(200);

    // set current for each channel
    this->set_led_current_by_channel(CHANNEL_B, (uint8_t)(this->blue_current_ * 10));
    this->set_led_current_by_channel(CHANNEL_G, (uint8_t)(this->green_current_ * 10));
    this->set_led_current_by_channel(CHANNEL_R, (uint8_t)(this->red_current_ * 10));
    this->set_led_current_by_channel(CHANNEL_W, (uint8_t)(this->white_current_ * 10));
}

void LP5562Component::dump_config() {
    ESP_LOGCONFIG(TAG, "LP5562 Config");
    LOG_I2C_DEVICE(this);
    ESP_LOGCONFIG(TAG, 
                  "  use internal clock: %s\n"
                  "  power save mode: %s\n"
                  "  high pwm frequency: %s\n"
                  "  logarithmic dimming: %s\n"
                  "  red channel current: %.1f mA\n"
                  "  green channel current: %.1f mA\n"
                  "  blue channel current: %.1f mA\n"
                  "  white channel current: %.1f mA\n",
                TRUEFALSE(this->use_internal_clk_),
                TRUEFALSE(this->power_save_enable_),
                TRUEFALSE(this->high_pwm_freq_enable_),
                TRUEFALSE(this->logarithmic_dimming_),
                this->red_current_, this->green_current_, this->blue_current_, this->white_current_);
}


void LP5562Component::set_led_brightness_by_channel(LED_Channel_t channel, uint8_t brightness) {
    uint8_t reg; // which register

    switch (channel) {
        case CHANNEL_B: reg = REG_B_PWM; break;
        case CHANNEL_G: reg = REG_G_PWM; break;
        case CHANNEL_R: reg = REG_R_PWM; break;
        case CHANNEL_W: reg = REG_W_PWM; break;
        default:
            ESP_LOGW(TAG, "Unspecified RGBW channel..");
            return;
    }
    LP5562_WARN_CHECK(this->write_byte(reg, brightness));
}


void LP5562Component::set_led_current_by_channel(LED_Channel_t channel, uint8_t current) {
    uint8_t reg; // which register

    switch (channel) {
        case CHANNEL_B: reg = REG_B_CURRENT; break;
        case CHANNEL_G: reg = REG_G_CURRENT; break;
        case CHANNEL_R: reg = REG_R_CURRENT; break;
        case CHANNEL_W: reg = REG_W_CURRENT; break;
        default:
            ESP_LOGW(TAG, "Unspecified RGBW channel..");
            return;
    }
    LP5562_WARN_CHECK(this->write_byte(reg, current));
}


void LP5562Component::map_led_2_ctrl_src(LED_Channel_t channel, uint8_t source) {
    if (source > 3) return; // Invalid source
    uint8_t current_mapping;
    LP5562_WARN_CHECK(this->read_byte(REG_LED_MAP, &current_mapping));

    uint8_t shift = 0;
    uint8_t mask  = 0x03;

    switch (channel) {
        case CHANNEL_B: shift = 0; break;
        case CHANNEL_G: shift = 2; break;
        case CHANNEL_R: shift = 4; break;
        case CHANNEL_W: shift = 6; break;
        default: return;
    }

    mask = mask << shift;         // Position mask correctly
    source = source << shift;     // Position source value correctly

    current_mapping = (current_mapping & ~mask) | source; // Clear old bits, set new bits
    LP5562_WARN_CHECK(this->write_byte(REG_LED_MAP, current_mapping));
    delayMicroseconds(200); // OP Mode change delay > 153us (mapping affects control source)
}

// Sets the mode for a single engine (0=Eng1, 1=Eng2, 2=Eng3)
void LP5562Component::set_engine_mode_(uint8_t engine, uint8_t mode) {
    if (engine > 2 || mode > 3) return; // There are only 3 engines and 4 modes, index start with 0

    uint8_t current_op_mode;
    LP5562_WARN_CHECK(this->read_byte(REG_OP_MODE, &current_op_mode));

    // Engine 1: shift 4, Engine 2: shift 2, Engine 3: shift 0
    uint8_t shift = ( 2 - engine ) * 2;

    uint8_t mask = 0x03 << shift;

    uint8_t new_mode = mode << shift;

    current_op_mode = (current_op_mode & ~mask) |  new_mode;
    LP5562_WARN_CHECK(this->write_byte(REG_OP_MODE, current_op_mode));
    delayMicroseconds(200); // OP Mode change delay > 153us

}


void LP5562Component::set_all_engine_mode_(uint8_t mode) {
    if ( mode > 3 ) return;
    uint8_t mode_val = (mode << 4) | (mode << 2) | mode;
    LP5562_WARN_CHECK(this->write_byte(REG_OP_MODE, mode_val));
    delayMicroseconds(200); // OP Mode change delay > 153us
}

void LP5562Component::set_all_led_mapping_(uint8_t source) {
    if ( source > 3 ) return;
    uint8_t map_val = (source << 6) | (source << 4) | (source << 2) | source;
    LP5562_WARN_CHECK(this->write_byte(REG_LED_MAP, map_val));
    delayMicroseconds(200); // OP Mode change delay > 153us (mapping affects control source)
}


} // namespace lp5562
} // namespace esphome