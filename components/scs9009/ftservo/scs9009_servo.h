#pragma once

#include "../scs9009.h"
#include "../../ftservo/ftservo.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"


namespace esphome {
namespace scs9009 {

class SCS9009Servo : public ftservo::FTServo,
                     public Component,
                     public Parented<SCS9009Component>
{
 public:
  void setup() override;
  void loop() override;
  void move(int position, uint16_t time, uint16_t speed) override;
  bool read_sensor_value(ftservo::ServoSensorField field, int &value) override;
  bool read_torque_enable() override;
  void write_torque_enable(bool enable) override;
  void set_address(uint8_t addr) { this->addr_ = addr; }
  bool can_proceed() override { return this->servo_ready_; }
 protected:
  uint8_t addr_;
  bool servo_ready_{false};
};

}
}