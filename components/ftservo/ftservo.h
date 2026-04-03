#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include "esphome/components/number/number.h"
#include "esphome/core/defines.h"

namespace esphome {
namespace ftservo {

typedef enum FTServoErr : uint8_t {
  FT_OK = 0,
  FT_ERR_NO_REPLY = 1,
  FT_ERR_CRC_CMP  = 2,
  FT_ERR_SLAVE_ID = 3,
	FT_ERR_BUFF_LEN = 4,
} ft_err_t;


enum class State : uint8_t {
  IDLE = 0, // idle
  MOVING,   // moving
  HOLDING,  // torque hold
  STALLED,  // to be Decided
  ERROR     // error code reported by servo
};

enum class ServoSensorField : uint8_t {
  POSITION,
  SPEED,
  LOAD,
  VOLTAGE,
  CURRENT,
  TEMPERATURE
};

class FTServo {

#ifdef USE_NUMBER
SUB_NUMBER(position)
SUB_NUMBER(angle)
SUB_NUMBER(speed)
SUB_NUMBER(time)
#endif
 
 public:
  virtual void move(int position, uint16_t time = 0, uint16_t speed = 500) = 0;

  virtual bool read_sensor_value(ServoSensorField field, int &value) = 0;

  virtual bool read_torque_enable() = 0;

  virtual void write_torque_enable(bool enable) = 0;

  
  int get_servo_position() const { return this->c_position_; }
  int get_servo_angle() const { return this->c_angle_; }
  int get_servo_speed() const { return this->c_speed_; }
  int get_servo_time() const { return this->c_time_; }

  void set_servo_position(int pos) { this->c_position_ = pos; }
  void set_servo_angle(int angle) { this->c_angle_ = angle; }
  void set_servo_speed(int speed) { this->c_speed_ = speed; }
  void set_servo_time(int time) { this->c_time_ = time; }

  void move_angle(int angle);  // this is called by Angle Number

  
  bool is_moving() const { return this->state_ == State::MOVING; }
  bool is_idle() const { return this->state_ == State::IDLE; }
  
  void add_move_callback(std::function<void()> &&move_callback);
  void add_error_callback(std::function<void(const LogString *)> &&error_callback);

 protected:
  State state_{State::IDLE};

  CallbackManager<void()> move_callbacks_ {};
  CallbackManager<void(const LogString *)> error_callbacks_ {};

  // servo control parameters
  int c_position_;
  int c_angle_;
  int c_speed_{500};
  int c_time_{0};

};

}
}