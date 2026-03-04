#pragma once

#include "esphome/components/text_sensor/text_sensor.h"
#include "../si12t.h"

namespace esphome {
namespace si12t {

class SI12TTextSensor : public text_sensor::TextSensor,
                        public PollingComponent,
                        public Parented<SI12TComponent>
{
public:
  void setup() override;
  void update() override;
  void dump_config() override;

  void set_channel(SI12TChannel channel) { this->channel_ = channel; }
  void set_sensitivity(SI12TSensitivityLevel sens_level) { this->sens_level_ = sens_level; }
  void set_extend_threshold(bool extend_threshold) { this->extend_threshold_ = extend_threshold; }
protected:
  SI12TChannel channel_;
  SI12TSensitivityLevel sens_level_;
  bool extend_threshold_;
};


} // namespace si12t
} // namespace esphome