#pragma once

#include "esphome/components/number/number.h"
#include "../ftservo.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ftservo {

static const char *TAG = "ftservo.number";

static float map_range(float val, float in_min, float in_max, float out_min, float out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class ServoPositionNumber: public number::Number,
                           public Parented<FTServo>
{

 public:
  void control(float value) override {
    // read current pos
    int current_pos;
    bool ret = this->parent_->read_sensor_value(ServoSensorField::POSITION, current_pos);
    if ( !ret ) {
      ESP_LOGW(TAG, "Failed to read current position");
      return;
    }
    this->publish_state(value);
    // cast the target pos
    int target_pos = static_cast<int>(value);
    // get the speed and time configuration
    int speed = this->parent_->get_servo_speed();
    int time  = this->parent_->get_servo_time();

    // perform move
    this->parent_->move(target_pos, time, speed);

    // add proper delay
    delay(abs(target_pos - current_pos) * 1000 / speed + 100);

    // release the torque
    this->parent_->write_torque_enable(false);

  }
};


class ServoAngleNumber : public number::Number,
                         public Parented<FTServo>
{
 public:
  void set_use_raw_angle(bool use_raw_angle) { this->use_raw_angle_ = use_raw_angle; }
  void set_angle_offset(float offset) { this->angle_offset_ = offset; }
  void control(float value) override {
    if ( use_raw_angle_ ) {
      this->parent_->move_angle(static_cast<int>(value));
    } else {
      this->parent_->move_angle(static_cast<int>(value + this->angle_offset_));
    }
    this->publish_state(value);
  }

 protected:
  float angle_offset_{0.0f};
  bool use_raw_angle_{true};
};

class ServoSpeedNumber : public number::Number,
                         public Parented<FTServo>
{
 public:
  void control(float value) override {
    this->parent_->set_servo_speed(static_cast<int>(value));
    this->publish_state(value);
  }
};


class ServoTimeNumber : public number::Number,
                         public Parented<FTServo>
{
 public:
  void control(float value) override {
    this->parent_->set_servo_time(static_cast<int>(value));
    this->publish_state(value);
  }
};                         

} // namespace ftservo
} // namespace esphome