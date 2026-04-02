#pragma once

#include "esphome/components/sensor/sensor.h"
#include "../ftservo.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ftservo {

class FTServoSensor : public sensor::Sensor,
                      public PollingComponent,
                      public Parented<FTServo>
{

SUB_SENSOR(position)
SUB_SENSOR(speed)
SUB_SENSOR(load)
SUB_SENSOR(voltage)
SUB_SENSOR(current)
SUB_SENSOR(temperature)

 public:
  void setup() override;
  void update() override;
  void dump_config() override;


};


} // namespace ftservo
} // namespace esphome