#include "ftservo.h"
#include "esphome/core/log.h"
#include "esphome/core/progmem.h"
#include <functional>

namespace esphome {
namespace ftservo {

static const char *TAG = "ftservo";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(FTServoErrString,
  "FT_OK",
  "FT_ERR_NO_REPLY",
  "FT_ERR_CRC_CMP",
  "FT_ERR_SLAVE_ID",
	"FT_ERR_BUFF_LEN"
);

static const LogString *get_err_string(FTServoErr err) {
    return FTServoErrString::get_log_str(static_cast<uint8_t>(err), 0);
}

#endif

void FTServo::add_move_callback(std::function<void()> &&move_callback) {
  this->move_callbacks_.add(std::move(move_callback));
}


void FTServo::add_error_callback(std::function<void(const LogString *)> &&error_callback) {
  this->error_callbacks_.add(std::move(error_callback));
}


void FTServo::move_angle(int angle) {
  // a degree is around 2.844 steps
  float unit = 2.844f;
  int current_pos;
  // read current position
  bool ret = this->read_sensor_value(ServoSensorField::POSITION, current_pos);

  if ( !ret ) {
    ESP_LOGW(TAG, "Failed to read current position");
    return;
  }
  // set the target position
  float target_pos = (float)angle * unit;

  // read speed and time
  int speed = this->get_servo_speed();
  int time  = this->get_servo_time();

  this->move(target_pos, time, speed);

  // add proper delay
  delay(abs(target_pos - current_pos) * 1000 / speed + 100);

  this->write_torque_enable(false); // release the torque once the movement is finished
}

}
}