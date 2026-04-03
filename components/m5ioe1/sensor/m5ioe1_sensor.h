#pragma once

#include "esphome/components/sensor/sensor.h"
#include "../m5ioe1.h"


namespace esphome {
namespace m5ioe1 {

class M5IOE1Sensor : public sensor::Sensor,
                     public PollingComponent,
                     public Parented<M5IOE1Component>
{

public:
  void update() override;
  void dump_config() override;

  void set_channel(M5IOE1ADCChannel channel) { this->channel_ = channel; }

protected:
  M5IOE1ADCChannel channel_{ADC_NONE};

};


} // namespace m5ioe1
} // namespace esphome