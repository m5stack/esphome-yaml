#pragma once

#include "esphome/components/number/number.h"
#include "../ftservo.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace ftservo {

class ServoControlNumber : public number::Number,
                           public Component,
                           public Parented<FTServo>
{

SUB_NUMBER(position)
SUB_NUMBER(speed)
SUB_NUMBER(time)
 public:
  void setup() override {
    // read initial pos doesn't seems to work here
    // delay(100);  // wait for servo
    
    // bool ret = this->parent_->read_sensor_value(ServoSensorField::POSITION, this->initial_pos_);
    // if (ret && this->initial_pos_ != 0) {
    //     this->publish_state(static_cast<float>(this->initial_pos_));
    // } else {
    //     // retry
    //     delay(150);
    //     ret = this->parent_->read_sensor_value(ServoSensorField::POSITION, this->initial_pos_);
    //     if (ret) {
    //         this->publish_state(static_cast<float>(this->initial_pos_));
    //     } else {
    //         this->initial_pos_ = 0;
    //         this->publish_state(0.0f);
    //     }
    // }
  }

  void control(float value) override {

    bool ret = this->parent_->read_sensor_value(ServoSensorField::POSITION, this->current_pos_);
    this->target_pos_ = static_cast<int>(value);
    this->parent_->move(this->target_pos_, 0, 500);

    this->publish_state(value);
    // release the torque so user can operate
    // add a proper delay
    delay(abs(this->target_pos_ - this->current_pos_) * 1000 / 500 + 100);
    this->parent_->write_torque_enable(false);
  }

  void sync_from_hardware(float position) {
    // publish value only
    if (this->state != position) {
      this->publish_state(position);
    }
  }

 protected:
  int initial_pos_;

  int current_pos_{0};
  int target_pos_{0};


};

} // namespace ftservo
} // namespace esphome