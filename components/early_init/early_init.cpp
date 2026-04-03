#include "early_init.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace early_init {

static const char *const TAG = "early_init";

float EarlyInitComponent::get_setup_priority() const {
  return setup_priority::BUS + 1.0f;
}

void EarlyInitComponent::setup() {
  ESP_LOGI(TAG, "Running early initialization");

  for (auto &cfg : this->pins_) {
    if (cfg.pin == nullptr)
      continue;
    cfg.pin->setup();
    cfg.pin->digital_write(cfg.level);
    ESP_LOGI(TAG, "Set early GPIO to %s", cfg.level ? "HIGH" : "LOW");
  }

  if (this->delay_ms_ > 0) {
    ESP_LOGI(TAG, "Delaying %u ms", this->delay_ms_);
    delay(this->delay_ms_);
  }
}

}  // namespace early_init
}  // namespace esphome