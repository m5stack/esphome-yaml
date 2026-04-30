#include "extio2_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"


namespace esphome {
namespace extio2 {

static const char *TAG = "extio2.sensor";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(ADCChannelStrings,
  "ADC 0",
  "ADC 1",
  "ADC 2",
  "ADC 3",
  "ADC 4",
  "ADC 5",
  "ADC 6",
  "ADC 7",
  "Unknown ADC Channel"
);

PROGMEM_STRING_TABLE(ADCResolutionStrings, "8 bits", "12 bits", "Unkown Resolution");

static const LogString *adc_channel_to_string(EXTIO2ADCChannel channel) {
  return ADCChannelStrings::get_log_str(static_cast<uint8_t>(channel), -1);
}

static const LogString *adc_resolution_to_string(EXTIO2ADCResolution resolution) {
  return ADCResolutionStrings::get_log_str(static_cast<uint8_t>(resolution), -1);
}

#endif

void EXTIO2Sensor::setup() {
  uint8_t pin = static_cast<uint8_t>(this->channel_);
  if ( this->resolution_ == ADC_8BIT ) {
    this->parent_->set_pin_func(pin, PIN_ADC_8);
  } else {
    this->parent_->set_pin_func(pin, PIN_ADC_12);
  }
  delay(20);
}

void EXTIO2Sensor::update() {
  uint8_t pin = static_cast<uint8_t>(this->channel_);
  if ( this->resolution_ == ADC_8BIT ) {
    uint8_t val = this->parent_->get_adc_8(pin);
    this->publish_state(static_cast<float>(val));
  } else {
    uint16_t val = this->parent_->get_adc_12(pin);
    this->publish_state(static_cast<float>(val));
  }

}

void EXTIO2Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "EXT.IO2 ADC Sensor: "
                    "  ADC Channel %s\n"
                    "  ADC Resolution %s\n",
                    LOG_STR_ARG(adc_channel_to_string(this->channel_)),
                    LOG_STR_ARG(adc_resolution_to_string(this->resolution_)));
  LOG_SENSOR(" ", "ADC Sensor: ", this);
}


} // namespace extio2
} // namespace esphome