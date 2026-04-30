#include "extio2.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"

namespace esphome {
namespace extio2 {

static const char *TAG = "extio2";

// Register base addresses (per-pin, offset by pin number 0~7)
static constexpr uint8_t REG_MODE_BASE       = 0x00;  // IO mode select (0x00~0x07)
static constexpr uint8_t REG_OUTPUT_BASE     = 0x10;  // Digital output  (0x10~0x17)
static constexpr uint8_t REG_INPUT_BASE      = 0x20;  // Digital input   (0x20~0x27)
static constexpr uint8_t REG_ADC_8BIT_BASE   = 0x30;  // ADC 8-bit      (0x30~0x37)
static constexpr uint8_t REG_ADC_12BIT_BASE  = 0x40;  // ADC 12-bit     (0x40~0x47)
static constexpr uint8_t REG_SERVO_8BIT_BASE = 0x50;  // Servo 8-bit    (0x50~0x57)
static constexpr uint8_t REG_SERVO_16BIT_BASE = 0x60; // Servo 16-bit   (0x60~0x67)
static constexpr uint8_t REG_RGB_BASE        = 0x70;  // RGB888         (0x70~0x87, 3 bytes per pin)
static constexpr uint8_t REG_PWM_BASE        = 0x90;  // PWM duty       (0x90~0x97)
static constexpr uint8_t REG_PWM_FREQ        = 0xA0;  // PWM frequency
static constexpr uint8_t REG_FIRM_VER        = 0xFE;  // Firmware version
static constexpr uint8_t REG_I2C_ADDR        = 0xFF;  // I2C address

// IO mode values written to mode registers
static constexpr uint8_t MODE_DIGITAL_INPUT  = 0x00;
static constexpr uint8_t MODE_DIGITAL_OUTPUT = 0x01;
static constexpr uint8_t MODE_ADC            = 0x02;
static constexpr uint8_t MODE_SERVO          = 0x03;
static constexpr uint8_t MODE_NEOPIXEL       = 0x04;
static constexpr uint8_t MODE_PWM            = 0x05;

// Maximum number of IO pins
static constexpr uint8_t NUM_PINS = 8;

static uint8_t pin_func_value(PinFunc func) {
  switch (func) {
    case PIN_OUTPUT:           return MODE_DIGITAL_OUTPUT;
    case PIN_ADC_8:
    case PIN_ADC_12:           return MODE_ADC;
    case PIN_SERVO_8:
    case PIN_SERVO_16:         return MODE_SERVO;
    case PIN_RGB:              return MODE_NEOPIXEL;
    case PIN_PWM:              return MODE_PWM;
    case PIN_INPUT:
    default:                   return MODE_DIGITAL_INPUT;
  }
}

#define EXTIO2_ERR_FAILED(err) \
  if (!(err)) { \
    this->mark_failed(); \
    return; \
  }

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(PWMFreqStrings,
  "2 kHz",
  "1 kHz",
  "500 Hz",
  "250 Hz",
  "125 Hz",
  "Unknown"
);

static const LogString *pwm_freq_to_string(EXTIO2PWMFrequency freq) {
  return PWMFreqStrings::get_log_str(static_cast<uint8_t>(freq), -1);
}

#endif


void EXTIO2Component::setup() {
  ESP_LOGD(TAG, "Setting up EXTIO2...");

  // Read firmware version
  uint8_t version;
  EXTIO2_ERR_FAILED(this->read_byte(REG_FIRM_VER, &version));

  delayMicroseconds(100);

  if (version == 0x00 || version == 0xFF) {
    ESP_LOGE(TAG, "Invalid firmware version 0x%02X", version);
    this->mark_failed(LOG_STR("Read invalid firmware version."));
    return;
  }
  this->firm_ver_ = version;

  ESP_LOGD(TAG, "EXTIO2 firmware version: 0x%02X", version);

  // perform software reset
  if (this->reset_) {
    // Reset all pins to digital input mode
    for (uint8_t i = 0; i < NUM_PINS; i++) {
      EXTIO2_ERR_FAILED(this->write_byte(REG_MODE_BASE + i, MODE_DIGITAL_INPUT));
    }
    this->mode_mask_ = 0;
    this->output_mask_ = 0;
  }
  
  delayMicroseconds(100);

  // set default output mode for every pin
  this->set_pin_func(PIN_OUTPUT);

  delayMicroseconds(100);

  // write pwm frequency
  if ( !this->write_byte(REG_PWM_FREQ, static_cast<uint8_t>(this->freq_)) ) {
    this->mark_failed(LOG_STR("Failed to set PWM frequency"));
    return;
  }

  ESP_LOGD(TAG, "EXTIO2 setup complete");
}

void EXTIO2Component::loop() {
  this->reset_pin_cache_();
}

void EXTIO2Component::dump_config() {
  ESP_LOGCONFIG(TAG, "EXTIO2: \n"
                     "PWM Frequency %s\n"
                     "Firmware version: %d\n",
                     LOG_STR_ARG(pwm_freq_to_string(this->freq_)),
                     this->firm_ver_);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
}

void EXTIO2Component::pin_mode(uint8_t pin, gpio::Flags flags) {
  if (pin >= NUM_PINS) {
    ESP_LOGE(TAG, "Invalid pin number %u", pin);
    return;
  }

  uint8_t mode;
  if (flags & gpio::FLAG_OUTPUT) {
    mode = MODE_DIGITAL_OUTPUT;
    this->mode_mask_ |= (1 << pin);
  } else if (flags & gpio::FLAG_INPUT) {
    mode = MODE_DIGITAL_INPUT;
    this->mode_mask_ &= ~(1 << pin);
  } else {
    ESP_LOGE(TAG, "Unsupported pin mode for pin %u", pin);
    return;
  }

  if (!this->write_byte(REG_MODE_BASE + pin, mode)) {
    this->status_set_warning(LOG_STR("Failed to set pin mode"));
    return;
  }

  ESP_LOGV(TAG, "Set pin %u mode to 0x%02X", pin, mode);
  this->status_clear_warning();
}

bool EXTIO2Component::read_gpio_modes_() {
  if (this->is_failed())
    return false;

  uint16_t mask = 0;
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    uint8_t mode;
    if (!this->read_byte(REG_MODE_BASE + i, &mode)) {
      this->status_set_warning(LOG_STR("Failed to read GPIO mode"));
      return false;
    }
    if (mode == MODE_DIGITAL_OUTPUT) {
      mask |= (1 << i);
    }
  }

  this->mode_mask_ = mask;
  this->status_clear_warning();
  return true;
}

bool EXTIO2Component::write_gpio_modes_() {
  if (this->is_failed())
    return false;

  for (uint8_t i = 0; i < NUM_PINS; i++) {
    uint8_t mode = (this->mode_mask_ & (1 << i)) ? MODE_DIGITAL_OUTPUT : MODE_DIGITAL_INPUT;
    if (!this->write_byte(REG_MODE_BASE + i, mode)) {
      this->status_set_warning(LOG_STR("Failed to write GPIO mode"));
      return false;
    }
  }

  this->status_clear_warning();
  return true;
}

bool EXTIO2Component::read_gpio_outputs_() {
  if (this->is_failed())
    return false;

  uint16_t mask = 0;
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    uint8_t val;
    if (!this->read_byte(REG_OUTPUT_BASE + i, &val)) {
      this->status_set_warning(LOG_STR("Failed to read GPIO output register"));
      return false;
    }
    if (val) {
      mask |= (1 << i);
    }
  }

  this->output_mask_ = mask;
  this->status_clear_warning();
  return true;
}

bool EXTIO2Component::digital_read_hw(uint8_t pin) {
  if (this->is_failed())
    return false;

  // Read all input pins to update cache
  uint16_t mask = 0;
  for (uint8_t i = 0; i < NUM_PINS; i++) {
    uint8_t val;
    if (!this->read_byte(REG_INPUT_BASE + i, &val)) {
      this->status_set_warning(LOG_STR("Failed to read GPIO input"));
      return false;
    }
    if (val) {
      mask |= (1 << i);
    }
  }

  this->input_mask_ = mask;
  this->status_clear_warning();
  return true;
}

bool EXTIO2Component::digital_read_cache(uint8_t pin) {
  return this->input_mask_ & (1 << pin);
}

void EXTIO2Component::digital_write_hw(uint8_t pin, bool value) {
  if (this->is_failed())
    return;

  if (value) {
    this->output_mask_ |= (1 << pin);
  } else {
    this->output_mask_ &= ~(1 << pin);
  }

  uint8_t val = value ? 1 : 0;
  if (!this->write_byte(REG_OUTPUT_BASE + pin, val)) {
    this->status_set_warning(LOG_STR("Failed to write GPIO output"));
    return;
  }

  ESP_LOGV(TAG, "Set pin %u output to %u", pin, val);
  this->status_clear_warning();
}


void EXTIO2Component::set_pin_func(uint8_t pin, PinFunc func) {

  if( !this->write_byte(REG_MODE_BASE + pin, pin_func_value(func)) ) {
    this->status_set_warning(LOG_STR("Failed to set pin functionality"));
    return;
  }

  this->status_clear_warning();
}

void EXTIO2Component::set_pin_func(PinFunc func) {
  uint8_t data[8];
  std::fill(data, data + 8, pin_func_value(func));
  if ( !this->write_bytes(REG_MODE_BASE, data, 8) ) {
    this->status_set_warning(LOG_STR("Failed to set functionality for multiple pins"));
    return;
  }

  this->status_clear_warning();
}

void EXTIO2Component::set_pin_func(std::initializer_list<uint8_t> pins, PinFunc func) {
  // iterate the given pins and set individual functionalities
  for (const uint8_t pin : pins) {
    this->set_pin_func(pin, func);
  }
}

// ========== EXTIO2Pin ==========

void EXTIO2Pin::setup() {
  this->pin_mode(this->flags_);
}

void EXTIO2Pin::pin_mode(gpio::Flags flags) {
  this->parent_->pin_mode(this->pin_, flags);
}

bool EXTIO2Pin::digital_read() {
  return this->parent_->digital_read(this->pin_) != this->inverted_;
}

void EXTIO2Pin::digital_write(bool value) {
  this->parent_->digital_write(this->pin_, value != this->inverted_);
}

size_t EXTIO2Pin::dump_summary(char *buffer, size_t len) const {
  return snprintf(buffer, len, "%u via EXTIO2", this->pin_);
}


// ========== EXTIO2 ADC ==========
uint8_t EXTIO2Component::read_adc8_(uint8_t pin) {
  uint8_t reg = REG_ADC_8BIT_BASE + pin;
  uint8_t val;
  
  if ( !this->read_byte(reg, &val) ) {
    this->status_set_warning(LOG_STR("Failed to read ADC 8 bits"));
    return 0;
  }

  this->status_clear_warning();
  return val;
}

uint16_t EXTIO2Component::read_adc12_(uint8_t pin) {
  uint8_t reg = REG_ADC_12BIT_BASE + pin * 2;

  uint8_t val[2];

  if ( !this->read_bytes(reg, val, 2) ) {
    this->status_set_warning(LOG_STR("Failed to read ADC 12 bits"));
    return 0;
  }

  this->status_clear_warning();
  return ((val[1] & 0x0F) << 8) | val[0];

}


// ========== EXTIO2 RGB ==========

// For single LED
void EXTIO2Component::write_led_(uint8_t pin, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t offset = pin * 3;
  uint8_t reg = REG_RGB_BASE + offset;
  
  uint8_t data[3] = { r, g, b };

  if ( !this->write_bytes(reg, data, 3) ) {
    this->status_set_warning(LOG_STR("Failed to write LED registers"));
    return ;
  }

  this->status_clear_warning();
}

// For continuous LED (addressable)
void EXTIO2Component::write_led_(const uint8_t *buf, size_t len) {
  
  if ( len > NUM_PINS * 3 ) {
    this->status_set_warning(LOG_STR("Maximum LED count is 8"));
    len = NUM_PINS * 3;
  }

  if ( !this->write_bytes(REG_RGB_BASE, buf, len) ) {
    this->status_set_warning(LOG_STR("Failed to write RGB data"));
    return;
  }

  this->status_clear_warning();
}


// ========== EXTIO2 PWM ==========
void EXTIO2Component::write_pwm_duty_cycle_(uint8_t pin, uint8_t cycle) {

  uint8_t reg = REG_PWM_BASE + pin;

  if ( cycle > 100 ) {
    this->status_set_warning(LOG_STR("Duty cycle must be between 0 and 100"));
    cycle = 100;
  }

  if( !this->write_byte(reg, cycle) ) {
    this->status_set_warning(LOG_STR("Failed to write duty cycle"));
    return;
  }

  this->status_clear_warning();
}

// ========== EXTIO2 Servo ========
void EXTIO2Component::write_servo8_(uint8_t pin, uint8_t angle) {

  uint8_t reg = REG_SERVO_8BIT_BASE + pin;
  
  if ( angle > 180 ) {
    this->status_set_warning(LOG_STR("Servo angle must be between 0 to 180"));
    angle = 180;
  }

  if ( !this->write_byte(reg, angle) ) {
    this->status_set_warning(LOG_STR("Failed to write servo angle register"));
    return;
  }

  this->status_clear_warning();
}

void EXTIO2Component::write_servo16_(uint8_t pin, uint16_t us) {
  uint8_t offset = pin * 2;
  uint8_t reg = REG_SERVO_16BIT_BASE + offset;

  if ( us > 4095 ) {
    this->status_set_warning(LOG_STR("Servo PWM value must be between 0 to 4095"));
    us = 4095;
  }

  if ( !this->write_bytes(reg, reinterpret_cast<uint8_t *>(&us), 2) ) {
    this->status_set_warning(LOG_STR("Failed to write servo PWM register"));
    return ;
  }
  this->status_clear_warning();
}

}  // namespace extio2
}  // namespace esphome