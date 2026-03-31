#pragma once

#include "esphome/components/switch/switch.h"
#include "../m5ioe1.h"

namespace esphome {
namespace m5ioe1 {

static const char *TAG = "m5ioe1.switch";

class AW9737ASwitch : public switch_::Switch,
                      public Component,
                      public Parented<M5IOE1Component>
{

public:
  void setup() override { this->parent_->setup_aw8737a(this->pin_, this->pulse_num_); }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "M5IOE1 Switch: "
                       "  Pin: IO%d",
                       "  Pulse number: %d",
                      this->pin_, this->pulse_num_);
    LOG_SWITCH("", "AW8737A Switch", this);
  }

  void set_pin(uint8_t pin) { this->pin_ = pin; }

  void set_pulse_num(uint8_t pulse_num) { this->pulse_num_ = pulse_num; }

protected:
  void write_state(bool state) override { this->parent_->start_aw8737a(state); }

  uint8_t pin_;
  uint8_t pulse_num_;

};

} // namespace m5ioe1
} // namespace esphome