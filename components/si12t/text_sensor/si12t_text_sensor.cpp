#include "si12t_text_sensor.h"
#include "esphome/core/progmem.h"
#include "../si12t.h"


namespace esphome {
namespace si12t {

static const char *TAG = "si12t.text_sensor";

// string lookup table
PROGMEM_STRING_TABLE(SI12TChannelStrings,
  "CH1",
  "CH2",
  "CH3",
  "CH4",
  "CH5",
  "CH6",
  "CH7",
  "CH8",
  "CH9",
  "CH10",
  "CH11",
  "CH12",
  "UNKNOWN"
);

PROGMEM_STRING_TABLE(SI12TSensitivityStrings,
  "Level0",
  "Level1",
  "Level2",
  "Level3",
  "Level4",
  "Level5",
  "Level6",
  "Level7",
  "INVALID"
);

PROGMEM_STRING_TABLE(SI12TOutputStrings,
  "No touch",
  "Low",
  "Medium",
  "High"
);

static const LogString *channel_to_string(SI12TChannel channel) {
  return SI12TChannelStrings::get_log_str(static_cast<uint8_t>(channel), SI12TChannelStrings::LAST_INDEX);
}

static const LogString *sensitivity_to_string(SI12TSensitivityLevel sens_level) {
  return SI12TSensitivityStrings::get_log_str(static_cast<uint8_t>(sens_level), SI12TChannelStrings::LAST_INDEX);
}

static const LogString *output_level_to_string(uint8_t level) {
  return SI12TOutputStrings::get_log_str(level, 0);
}

void SI12TTextSensor::setup() {
  this->parent_->setup_channel(this->channel_);
  this->parent_->write_channel_sensitivity(this->channel_, this->sens_level_, this->extend_threshold_);
}

void SI12TTextSensor::update() {
  uint8_t result = this->parent_->read_channel_output(this->channel_);
  this->publish_state(LOG_STR_ARG(output_level_to_string(result)));
}

void SI12TTextSensor::dump_config() {
  LOG_TEXT_SENSOR("", "SI12T Text Sensor", this);
  ESP_LOGCONFIG(TAG, "  Channel: %s\n"
             "  Sensitivity: %s\n"
             "  Extend Threshold: %s\n",
             LOG_STR_ARG(channel_to_string(this->channel_)),
             LOG_STR_ARG(sensitivity_to_string(this->sens_level_)),
             TRUEFALSE(this->extend_threshold_));
}

} // namespace si12t
} // namespace esphome 