#include "ftservo_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ftservo {

static const char *TAG = "ftservo.sensor";

#define FTSERVO_PUBLISH(name, field)                                                  \
  do {                                                                                \
    if (this->name##_sensor_ != nullptr) {                                            \
      int val = 0;                                                                    \
      bool ret = this->parent_->read_sensor_value(field, val);                        \
      if (ret) {                                                                      \
        this->name##_sensor_->publish_state(static_cast<float>(val));                 \
      } else {                                                                        \
        ESP_LOGW(TAG, "Sensor '%s' doesn't exist or not implemented", #name);         \
        this->status_set_warning();                                                   \
        this->name##_sensor_->publish_state(NAN);                                     \
      }                                                                               \
    }                                                                                 \
  } while (0)

void FTServoSensor::setup() {
}


void FTServoSensor::update() {
  FTSERVO_PUBLISH(position, ServoSensorField::POSITION);
  FTSERVO_PUBLISH(speed, ServoSensorField::SPEED);
  FTSERVO_PUBLISH(load, ServoSensorField::LOAD);

  FTSERVO_PUBLISH(current, ServoSensorField::CURRENT);
  FTSERVO_PUBLISH(temperature, ServoSensorField::TEMPERATURE);


  if (this->voltage_sensor_ != nullptr) {
    int val = 0;
    bool ret = this->parent_->read_sensor_value(ServoSensorField::VOLTAGE, val);
    if (ret) {
      float final = static_cast<float>(val) * 0.1;
      this->voltage_sensor_->publish_state(final); // voltage uses 0.1v measurement
    } else {
      ESP_LOGW(TAG, "Sensor 'voltage' doesn't exist or not implemented");
      this->status_set_warning();
      this->voltage_sensor_->publish_state(NAN);
    }
  }

  this->status_clear_warning();

}

void FTServoSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "FTServo Sensor: ");
}

} // namespace ftservo
} // namespace esphome