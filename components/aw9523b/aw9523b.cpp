#include "aw9523b.h"
#include "esphome/core/gpio.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"

namespace esphome {
namespace aw9523b {

static const uint8_t AW9523B_INPUT_P0    = 0x00;      // P0 Input Status, read only
static const uint8_t AW9523B_INPUT_P1    = 0x01;      // P1 Input Status, read only
static const uint8_t AW9523B_OUTPUT_P0   = 0x02;      // P0 Output Status, R/W
static const uint8_t AW9523B_OUTPUT_P1   = 0x03;      // P1 Output Status, R/W
static const uint8_t AW9523B_CONFIG_P0   = 0x04;      // P0 Input/Output configuration, R/W
static const uint8_t AW9523B_CONFIG_P1   = 0x05;      // P1 Input/Output configuration, R/W
static const uint8_t AW9523B_INT_P0      = 0x06;      // P0 Interrupt, R/W
static const uint8_t AW9523B_INT_P1      = 0x07;      // P1 Interrupt, R/W
static const uint8_t AW9523B_ID          = 0x10;      // ID chip, read only
static const uint8_t AW9523B_CTL         = 0x11;      // Global Control Register
static const uint8_t AW9523B_LED_MODE_P0 = 0x12;      // LED Mode Switch for P0
static const uint8_t AW9523B_LED_MODE_P1 = 0x13;      // LED Mode Switch for P1
static const uint8_t AW9523B_DIM0        = 0x20;      // P1_0 LED current control
static const uint8_t AW9523B_DIM1        = 0x21;      // P1_1 LED current control
static const uint8_t AW9523B_DIM2        = 0x22;      // P1_2 LED current control
static const uint8_t AW9523B_DIM3        = 0x23;      // P1_3 LED current control
static const uint8_t AW9523B_DIM4        = 0x24;      // P0_0 LED current control
static const uint8_t AW9523B_DIM5        = 0x25;      // P0_1 LED current control
static const uint8_t AW9523B_DIM6        = 0x26;      // P0_2 LED current control
static const uint8_t AW9523B_DIM7        = 0x27;      // P0_3 LED current control
static const uint8_t AW9523B_DIM8        = 0x28;      // P0_4 LED current control
static const uint8_t AW9523B_DIM9        = 0x29;      // P0_5 LED current control
static const uint8_t AW9523B_DIM10       = 0x2a;      // P0_6 LED current control
static const uint8_t AW9523B_DIM11       = 0x2b;      // P0_7 LED current control
static const uint8_t AW9523B_DIM12       = 0x2c;      // P1_4 LED current control
static const uint8_t AW9523B_DIM13       = 0x2d;      // P1_5 LED current control
static const uint8_t AW9523B_DIM14       = 0x2e;      // P1_6 LED current control
static const uint8_t AW9523B_DIM15       = 0x2F;      // P1_7 LED current control

// Lookup table for pin to DIM register mapping
static const uint8_t AW9523B_DIM_REGS[16] = {
  AW9523B_DIM4,   // Pin 0 (P0_0)
  AW9523B_DIM5,   // Pin 1 (P0_1)
  AW9523B_DIM6,   // Pin 2 (P0_2)
  AW9523B_DIM7,   // Pin 3 (P0_3)
  AW9523B_DIM8,   // Pin 4 (P0_4)
  AW9523B_DIM9,   // Pin 5 (P0_5)
  AW9523B_DIM10,  // Pin 6 (P0_6)
  AW9523B_DIM11,  // Pin 7 (P0_7)
  AW9523B_DIM0,   // Pin 8 (P1_0)
  AW9523B_DIM1,   // Pin 9 (P1_1)
  AW9523B_DIM2,   // Pin 10 (P1_2)
  AW9523B_DIM3,   // Pin 11 (P1_3)
  AW9523B_DIM12,  // Pin 12 (P1_4)
  AW9523B_DIM13,  // Pin 13 (P1_5)
  AW9523B_DIM14,  // Pin 14 (P1_6)
  AW9523B_DIM15   // Pin 15 (P1_7)
};

static const uint8_t AW9523B_SW_RST = 0x7F;           // software reset

static const char *TAG = "aw9523b";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG
PROGMEM_STRING_TABLE(P0DriveModeStrings, "OPEN_DRAIN", "PUSH_PULL");

static const LogString *p0_drive_mode_to_string(AW9523BP0DriveMode mode) {
  return  P0DriveModeStrings::get_log_str(mode, 0);
}

PROGMEM_STRING_TABLE(LEDMaxCurrentStrings, "37mA", "27.75mA", "18.5mA", "9.25mA");

static const LogString *led_max_current_to_string(AW9523BLEDMaxCurrent current) {
  return LEDMaxCurrentStrings::get_log_str(current, 0);
}
#endif

#define AW9523B_ERROR_FAILED(err) \
  if (!(err)) { \
    this->mark_failed(); \
    return; \
  }

void AW9523BComponent::setup()
{
    ESP_LOGD(TAG, "Setting up AW9523B IO expander.");

    // software reset
    if ( this->reset_ ) {
      AW9523B_ERROR_FAILED(this->write_byte(AW9523B_SW_RST, 0x00));
      delay(2);
    }

    // read chip id for validation
    uint8_t chip_id;
    AW9523B_ERROR_FAILED(this->read_byte(AW9523B_ID, &chip_id));

    if ( chip_id != 0x23 ) {
      ESP_LOGE(TAG, "Error reading chip id, expect '0x23', got '0x%X'", chip_id);
      this->mark_failed();
      return;
    }

    ESP_LOGD(TAG, "Read chip id: 0x%X", chip_id);

    // read global control register
    uint8_t ctl_data;
    AW9523B_ERROR_FAILED(this->read_byte(AW9523B_CTL, &ctl_data));

    // configure pad 0 open-drain or push-pull mode
    if ( this->p0_drive_mode_ == AW9523BP0DriveMode::PUSH_PULL ) {
        ctl_data |= (1 << 4);   //  push-pull
    } else {
        ctl_data &= ~(1 << 4);  //  open-drain
    }

    // set led max current
    ctl_data |= static_cast<uint8_t>(this->led_max_current_);

    // write global control register
    AW9523B_ERROR_FAILED(this->write_byte(AW9523B_CTL, ctl_data));

    // read gpio modes
    if ( !this->read_gpio_modes_() ) {
      this->mark_failed();
      return;
    }

    // read gpio outputs
    if ( !this->read_gpio_outputs_() ) {
      this->mark_failed();
      return;
    }

    // read led modes
    ESP_LOGI(TAG, "AW9523B setup complete.");
}

// Clear the cache at the start of each loop.
void AW9523BComponent::loop() { this->reset_pin_cache_();}

void AW9523BComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "AW9523B: \n"
                     "  P0 drive mode: %s\n", 
                     "  LED Max Current: %s",
                     LOG_STR_ARG(p0_drive_mode_to_string(this->p0_drive_mode_)),
                     LOG_STR_ARG(led_max_current_to_string(this->led_max_current_)));
  LOG_I2C_DEVICE(this)
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
}

// AW9523 only support INPUT and OUTPUT in GPIO mode
void AW9523BComponent::pin_mode(uint8_t pin, gpio::Flags flags) {

  uint16_t mask = 1 << pin;

  if ( flags & gpio::FLAG_INPUT ) {
    this->mode_mask_ |= mask;
  } else if ( flags & gpio::FLAG_OUTPUT ) {
    this->mode_mask_ &= ~mask;
  }

  this->write_gpio_modes_();
}


bool AW9523BComponent::read_gpio_outputs_() {

  if ( this->is_failed() ) {
    return false;
  }

  uint8_t data[2];

  if ( !this->read_bytes(AW9523B_OUTPUT_P0, data, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read output registers"));
    return false;
  }

  this->status_clear_warning();

  this->output_mask_ = data[0] | (data[1] << 8);

  return true;
}

bool AW9523BComponent::read_gpio_modes_() {

  if ( this->is_failed() ) {
    return false;
  }

  uint8_t data[2];

  if ( !this->read_bytes(AW9523B_CONFIG_P0, data, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read GPIO modes"));
    return false;
  }

  this->status_clear_warning();

  this->mode_mask_ = data[0] | (data[1] << 8);

  return true;
}

bool AW9523BComponent::read_gpio_interrupts_() {
  if ( this->is_failed() ) {
    return false;
  }

  uint8_t data[2];

  if ( !this->read_bytes(AW9523B_INT_P0, data, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read gpio interrupts"));
  }

  this->status_clear_warning();

  this->interrupt_mask_ = data[0] | (data[1] << 8);

  return true;
}

bool AW9523BComponent::write_gpio_modes_() {

  if ( this->is_failed() ) {
    return false;
  }

  if ( !this->write_bytes(AW9523B_CONFIG_P0, reinterpret_cast<uint8_t *>(&this->mode_mask_), 2) ) {
    this->status_set_warning(LOG_STR("Failed to write GPIO modes"));
  }

  this->status_clear_warning();

  return true;
}

bool AW9523BComponent::digital_read_hw(uint8_t pin){

  if (this->is_failed()) {
    return false;
  }

  uint8_t data[2];

  if ( !this->read_bytes(AW9523B_INPUT_P0, data, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read input registers"));
    return false;
  }

  this->status_clear_warning();

  this->input_mask_ = data[0] | (data[1] << 8);
  return true;
}

bool AW9523BComponent::digital_read_cache(uint8_t pin) { return (this->input_mask_ & (1 << pin)); }

void AW9523BComponent::digital_write_hw(uint8_t pin, bool value) {

  if (this->is_failed()) {
    return;
  }

  uint16_t new_mask = this->output_mask_;

  if (value)
    new_mask |= (1 << pin); // bit set 
  else
    new_mask &= ~(1 << pin); // bit clear

  if ( !this->write_bytes(AW9523B_OUTPUT_P0, reinterpret_cast<uint8_t *>(&new_mask), 2) ) {
    this->status_set_warning(LOG_STR("Failed to set pin for output"));
  }

  this->output_mask_ = new_mask;

  this->status_clear_warning();

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_VERBOSE
  ESP_LOGV(TAG, "Wrote GPIO output: 0b" BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN,
           BYTE_TO_BINARY(this->output_mask_ >> 8), BYTE_TO_BINARY(this->output_mask_ & 0x00FF));
#endif

  this->status_clear_warning();
}


bool AW9523BComponent::setup_led_mode(uint8_t pin) {

  if (this->is_failed()) {
    return false;
  }

  this->led_mode_mask_ |= (1 << pin);

  if ( !this->write_bytes(AW9523B_LED_MODE_P0, reinterpret_cast<uint8_t *>(&led_mode_mask_), 2) ) {
    this->status_set_warning(LOG_STR("Failed to write LED mode"));
    return false;
  }

  this->status_clear_warning();
  return true;
}

void AW9523BComponent::write_led_current(uint8_t pin, uint8_t current) {

  if (this->is_failed()) {
    return;
  }

  if ( !this->write_byte(AW9523B_DIM_REGS[pin], current)) {
    this->status_set_warning(LOG_STR("Failed to set LED current"));
    return;
  }
  this->status_clear_warning();
  
}

bool AW9523BComponent::read_led_modes_() {
  if ( this->is_failed() ) {
    return false;
  }

  uint8_t data[2];

  if ( !this->read_bytes(AW9523B_LED_MODE_P0, data, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read led mode registers"));
    return false;
  }

  this->led_mode_mask_ = data[0] | (data[1] << 8);
  this->status_clear_warning();

  return true;
}



void AW9523BGPIOPin::setup() { this->pin_mode(this->flags_); }
void AW9523BGPIOPin::pin_mode(gpio::Flags flags) { this->parent_->pin_mode(this->pin_, flags); }
bool AW9523BGPIOPin::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
void AW9523BGPIOPin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}

size_t AW9523BGPIOPin::dump_summary(char *buffer, size_t len) const {
  return snprintf(buffer, len, "%u via AW9523B", this->pin_);
}

} // namespace aw8523b
} // namespace esphome