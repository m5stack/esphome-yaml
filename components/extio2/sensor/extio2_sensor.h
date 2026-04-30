#pragma once

#include "esphome/components/sensor/sensor.h"
#include "../extio2.h"
#include "esphome/core/helpers.h"


namespace esphome {
namespace extio2 {

enum EXTIO2ADCChannel {
  ADC_0 = 0,
  ADC_1 = 1,
  ADC_2 = 2,
  ADC_3 = 3,
  ADC_4 = 4,
  ADC_5 = 5,
  ADC_6 = 6,
  ADC_7 = 7
};

enum EXTIO2ADCResolution {
  ADC_8BIT = 0,
  ADC_12BIT = 1
};

class EXTIO2Sensor : public sensor::Sensor,
                     public PollingComponent,
                     public Parented<EXTIO2Component>
{
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  void set_adc_channel(EXTIO2ADCChannel channel) { this->channel_ = channel; }
  void set_adc_resolution(EXTIO2ADCResolution resolution) { this->resolution_ = resolution; }
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
 protected:
  EXTIO2ADCChannel channel_;
  EXTIO2ADCResolution resolution_{ADC_8BIT};

};                    

} // namespace extio2
} // namespace esphome