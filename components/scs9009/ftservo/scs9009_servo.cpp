#include "scs9009_servo.h"

namespace esphome {
namespace scs9009 {

static const char *TAG = "ftservo.scs9009";

void SCS9009Servo::setup() {
  ESP_LOGD(TAG, "Setting up servo %d", this->addr_);

  // servo may take serval seconds to respond
  if ( this->parent_->ping(this->addr_) == -1 ) {
    this->status_set_warning(LOG_STR("Initial ping with servo failed, will try in 5 seconds"));
  }

  // second ping (async ping)
  this->set_timeout(5000, [this]() {
    int res = this->parent_->ping(this->addr_);
    if ( res != -1 ) {
      this->servo_ready_ = true; // mark servo ready
      ESP_LOGD(TAG, "Servo %d is ready", this->addr_);
    } else {
      this->mark_failed(LOG_STR("Communication with servo timeout (after 5s)"));
    }
  });

  this->status_clear_warning();

}

void SCS9009Servo::loop() {

}

bool SCS9009Servo::read_torque_enable() {
  int ret = this->parent_->read_torque_enable(this->addr_);
  return static_cast<bool>(ret);
}

void SCS9009Servo::write_torque_enable(bool enable) {
  this->parent_->enable_torque(this->addr_, enable);
}


bool SCS9009Servo::read_sensor_value(ftservo::ServoSensorField field, int &value) {
  switch (field) {
    case ftservo::ServoSensorField::POSITION:
      value = this->parent_->read_pos(this->addr_);
      return true;
    case ftservo::ServoSensorField::SPEED:
      value = this->parent_->read_speed(this->addr_);
      return true;
    case ftservo::ServoSensorField::LOAD:
      value = this->parent_->read_load(this->addr_);
      return true;
    case ftservo::ServoSensorField::VOLTAGE:
      value = this->parent_->read_voltage(this->addr_);
      return true;
    case ftservo::ServoSensorField::CURRENT:
      value = this->parent_->read_current(this->addr_);
      return true;
    case ftservo::ServoSensorField::TEMPERATURE:
      value = this->parent_->read_temperature(this->addr_);
      return true;
    default:
      return false;
  }
}


void SCS9009Servo::move(int position, uint16_t time, uint16_t speed) {
  if (!this->servo_ready_) {
    return;
  }

  this->parent_->write_pos(this->addr_, position, time, speed);
  delay(200);
}


}
}