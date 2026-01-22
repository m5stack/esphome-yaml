#pragma once

#include "esphome/components/output/float_output.h"
#include "../lp5562.h"

namespace esphome {
namespace lp5562 {

static const char *TAG = "lp5562.output";

class LP5562Output : public output::FloatOutput,
                     public Parented<LP5562Component>
{
public:
    void set_channel(LED_Channel_t channel) { channel_ = channel; }
    void write_state(float state) override {
        uint8_t val = (uint8_t) roundf(255.0f * state);
        this->parent_->set_led_brightness_by_channel(this->channel_, val);
    }

protected:
    LED_Channel_t channel_;
};

} // namespace lp5562
} // namespace esphome