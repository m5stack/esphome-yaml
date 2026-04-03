#include "m5ioe1_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/progmem.h"

namespace esphome {
namespace m5ioe1 {

static const char *TAG = "m5ioe1.adc_sensor";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(ADCChannelStrings,
  "ADC_NONE",
  "ADC_1",
  "ADC_2",
  "ADC_3",
  "ADC_4"
);

static const LogString *get_adc_channel_string(M5IOE1ADCChannel channel) {
    return ADCChannelStrings::get_log_str(static_cast<uint8_t>(channel), 0);
}

#endif

void M5IOE1Sensor::update() {
    float val = this->parent_->get_adc_value(this->channel_);
    this->publish_state(val);
}

void M5IOE1Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "M5IOE1 Sensor: \n"
                     "  ADC Channel: %s",
                     LOG_STR_ARG(get_adc_channel_string(this->channel_)));
  LOG_SENSOR(" ", "ADC Sensor: ", this);

}


}  // namespace m5ioe1
}  // namespace esphome