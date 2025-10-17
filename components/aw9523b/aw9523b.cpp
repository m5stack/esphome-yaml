#include "aw9523b.h"

namespace esphome {
namespace aw9523b{

static const uint8_t AW9523B_INPUT_P0 = 0x00;      // P0 Input Status, read only
static const uint8_t AW9523B_INPUT_P1 = 0x01;      // P1 Input Status, read only
static const uint8_t AW9523B_OUTPUT_P0 = 0x02;     // P0 Output Status, R/W
static const uint8_t AW9523B_OUTPUT_P1 = 0x03;     // P1 Output Status, R/W
static const uint8_t AW9523B_CONFIG_P0 = 0x04;     // P0 Input/Output configuration, R/W
static const uint8_t AW9523B_CONFIG_P1 = 0x05;     // P1 Input/Output configuration, R/W
static const uint8_t AW9523B_INT_0 = 0x06;        // P0 Interrupt, R/W
static const uint8_t AW9523B_INT_1 = 0x07;        // P1 Interrupt, R/W
static const uint8_t AW9523B_ID = 0x10;           // ID chip, read only
static const uint8_t AW9523B_CTL = 0x11;          // Global Interrupt Control Register
// LED modes are not used on M5CoreS3, but defined here
static const uint8_t AW9523B_LEDMODE_P0 = 0x12; // LED Mode Switch for P0
static const uint8_t AW9523B_LEDMODE_P1 = 0x13; // LED Mode Switch for P1
static const uint8_t AW9523B_DIM0  = 0x20; // P1_0 LED current control
static const uint8_t AW9523B_DIM1  = 0x21; // P1_1 LED current control
static const uint8_t AW9523B_DIM2  = 0x22; // P1_2 LED current control
static const uint8_t AW9523B_DIM3  = 0x23; // P1_3 LED current control
static const uint8_t AW9523B_DIM4  = 0x24; // P0_0 LED current control
static const uint8_t AW9523B_DIM5  = 0x25; // P0_1 LED current control
static const uint8_t AW9523B_DIM6  = 0x26; // P0_2 LED current control
static const uint8_t AW9523B_DIM7  = 0x27; // P0_3 LED current control
static const uint8_t AW9523B_DIM8  = 0x28; // P0_4 LED current control
static const uint8_t AW9523B_DIM9  = 0x29; // P0_5 LED current control
static const uint8_t AW9523B_DIM10 = 0x2a; // P0_6 LED current control
static const uint8_t AW9523B_DIM11 = 0x2b; // P0_7 LED current control
static const uint8_t AW9523B_DIM12 = 0x2c; // P1_4 LED current control
static const uint8_t AW9523B_DIM13 = 0x2d; // P1_5 LED current control
static const uint8_t AW9523B_DIM14 = 0x2e; // P1_6 LED current control
static const uint8_t AW9523B_DIM15 = 0x2f; // P1_7 LED current control
static const uint8_t AW9523B_SW_RST = 0x7F; // software reset


static const char *TAG = "aw9523b";


#define AW9523B_ERROR_FAILED(err) \
  if (!(err)) { \
    this->mark_failed(); \
    return; \
  }

void AW9523BComponent::setup()
{
    ESP_LOGD(TAG, "Setting up AW9523B IO expander.");

    uint8_t chipID;
    AW9523B_ERROR_FAILED(this->read_byte(AW9523B_ID, &chipID));

    if ( chipID != 0x23 ) {
      ESP_LOGE(TAG, "Error reading chip ID, expect 0x23, get 0x%x", chipID);
      this->mark_failed();
      return;
    }

    AW9523B_ERROR_FAILED(this->write_byte(AW9523B_SW_RST, 0x00)); // Reset AW9523
    // AW9523B_ERROR_FAILED(this->write_byte(0x04, 0x18)); // CONFIG P0
    // AW9523B_ERROR_FAILED(this->write_byte(0x05, 0x0c)); // CONFIG P1

    uint8_t data;
    AW9523B_ERROR_FAILED(this->read_byte(AW9523B_CTL, &data));
    if (this->p0_push_pull_mode_) {
        data |= 0x10;   //  Push-Pull
    } else {
        data &= ~0x10;  //  Open-Drain
    }

    AW9523B_ERROR_FAILED(this->write_byte(AW9523B_CTL, data));

    // AW9523B_ERROR_FAILED(this->write_byte(0x12, 0xFF)); // LEDMODE_P0
    // AW9523B_ERROR_FAILED(this->write_byte(0x13, 0xFF)); // LEDMODE_P1  

}

// Clear the cache at the start of each loop.
void AW9523BComponent::loop() { this->reset_pin_cache_();}

void AW9523BComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "AW9523B:");
  LOG_I2C_DEVICE(this)
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
}

// AW9523 only support INPUT and OUTPUT in GPIO mode
void AW9523BComponent::pin_mode(uint8_t pin, gpio::Flags flags) {
  // Store which bit to output
  uint16_t mask = 1 << (pin % 8);
  if (pin < 8) {
     if (flags & gpio::FLAG_INPUT)
      this->mode_mask_ |= mask;   
    else if (flags & gpio::FLAG_OUTPUT)
      this->mode_mask_ &= ~mask; 
  } else {
    if (flags & gpio::FLAG_INPUT)
      this->mode_mask_ |= (mask << 8);
    else if (flags & gpio::FLAG_OUTPUT)
      this->mode_mask_ &= ~(mask << 8);
  }
  // Write GPIO to enable input mode

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  ESP_LOGV(TAG, "Setting pin mode %d as %s (mask=0x%04x, mode_mask_=0x%04x)",
         pin,
         (flags & gpio::FLAG_INPUT) ? "INPUT" : "OUTPUT",
         mask, this->mode_mask_);
#endif
  this->write_gpio_modes_();
}


bool AW9523BComponent::read_gpio_outputs_() {
  uint8_t data_p0, data_p1;
  if ( !this->read_byte(AW9523B_OUTPUT_P0, &data_p0) ) {
    this->status_set_warning(LOG_STR("Failed to read output register P0"));
    return false;
  }

  if ( !this->read_byte(AW9523B_OUTPUT_P1, &data_p1) ) {
    this->status_set_warning(LOG_STR("Failed to read output register P1"));
    return false;
  }
  this->mode_mask_ = data_p0 | (data_p1 << 8);
  return true;
}

bool AW9523BComponent::read_gpio_modes_() {
  uint8_t data_p0, data_p1;
  if ( !this->read_byte(AW9523B_CONFIG_P0, &data_p0) ) {
    this->status_set_warning(LOG_STR("Failed to read GPIO modes for P0"));
    return false;
  }

  if ( !this->read_byte(AW9523B_CONFIG_P1, &data_p1) ) {
    this->status_set_warning(LOG_STR("Failed to read GPIO modes for P1"));
    return false;
  }

  this->mode_mask_ = data_p0 | (data_p1 << 8);
  return true;
}

bool AW9523BComponent::write_gpio_modes_() {

  if ( !this->write_byte(AW9523B_CONFIG_P0, this->mode_mask_ & 0x00FF) ) {
    this->status_set_warning(LOG_STR("Failed to write GPIO modes for P0"));
    return false;
  }
  
  if ( !this->write_byte(AW9523B_CONFIG_P1, this->mode_mask_ >> 8) ) {
    this->status_set_warning(LOG_STR("Failed to write GPIO modes for P1"));
    return false;
  }

  this->status_clear_warning();
  return true;
}

bool AW9523BComponent::digital_read_hw(uint8_t pin){
  uint8_t addr = pin < 8 ? AW9523B_INPUT_P0 : AW9523B_INPUT_P1;
  uint8_t data;

  if ( !this->read_byte(addr, &data) ) {
    return false;
  }

  // Update the appropriate part of input_mask_
  if (pin < 8) {
    this->input_mask_ = (this->input_mask_ & 0xFF00) | data;
  } else {
    this->input_mask_ = (this->input_mask_ & 0x00FF) | (uint16_t(data) << 8);
  }

  this->status_clear_warning();
  return true;
}

bool AW9523BComponent::digital_read_cache(uint8_t pin) { return (this->input_mask_ & (1 << pin)); }

void AW9523BComponent::digital_write_hw(uint8_t pin, bool value) {

  uint16_t mask = 1 << (pin % 8);

  if ( pin < 8 ) {
    if (value)
      this->output_mask_ |= mask;
    else
      this->output_mask_ &= ~mask;

    if (!this->write_byte(AW9523B_OUTPUT_P0, this->output_mask_ & 0xFF)) {
      this->status_set_warning(LOG_STR("Failed to write P0 output register"));
      return;
    }
  } else {
    // P1
    if (value)
      this->output_mask_ |= (mask << 8);
    else
      this->output_mask_ &= ~(mask << 8);

    if (!this->write_byte(AW9523B_OUTPUT_P1, (this->output_mask_ >> 8) & 0xFF)) {
      this->status_set_warning(LOG_STR("Failed to write P1 output register"));
      return;
    }
  }
  // if (value) {
  //   this->output_mask_ |= (1 << pin);
  // } else {
  //   this->output_mask_ &= ~(1 << pin);
  // }

  // Write registers to P0 and P1
  // uint8_t out_p0 = static_cast<uint8_t>(this->output_mask_ & 0xFF);
  // uint8_t out_p1 = static_cast<uint8_t>((this->output_mask_ >> 8) & 0xFF);

  // if (!this->write_byte(AW9523B_OUTPUT_P0, out_p0)) {
  //   this->status_set_warning(LOG_STR("Failed to write P0 output register"));
  //   return;
  // }
  // if (!this->write_byte(AW9523B_OUTPUT_P1, out_p1)) {
  //   this->status_set_warning(LOG_STR("Failed to write P1 output register"));
  //   return;
  // }

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  ESP_LOGV(TAG, "Wrote GPIO output: 0b" BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN,
           BYTE_TO_BINARY(this->output_mask_ & 0xFF), BYTE_TO_BINARY(this->output_mask_ >> 8));
#endif

  this->status_clear_warning();
}

void AW9523BGPIOPin::setup() { this->pin_mode(this->flags_); }
void AW9523BGPIOPin::pin_mode(gpio::Flags flags) { this->parent_->pin_mode(this->pin_, flags); }
bool AW9523BGPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void AW9523BGPIOPin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}
std::string AW9523BGPIOPin::dump_summary() const { return str_sprintf("%u via AW9523B", this->pin_); }

} // namespace aw8523b
      
} // namespace esphome