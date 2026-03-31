#include "aw9523b_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw9523b {

static const char *TAG = "aw9523b.output";

void AW9523BFloatOutput::setup() {
  this->parent_->setup_led_mode(this->pin_);
}

void AW9523BFloatOutput::dump_config() {
  ESP_LOGCONFIG(TAG, "AW9523B Float Output:\n"
                         "  Pin: %d",
                         this->pin_);
  LOG_FLOAT_OUTPUT(this);
}

void AW9523BFloatOutput::write_state(float state) {
  uint8_t current = static_cast<uint8_t>(roundf(state * 255.0f)); // 0 - 255
  this->parent_->write_led_current(this->pin_, current);
}

} // namespace aw9523b
} // namespace esphome