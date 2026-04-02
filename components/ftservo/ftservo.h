#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

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
 
 public:
  virtual void move(int position, uint16_t time = 0, uint16_t speed = 500) = 0;

  virtual bool read_sensor_value(ServoSensorField field, int &value) = 0;

  virtual bool read_torque_enable() = 0;

  virtual void write_torque_enable(bool enable) = 0;
  
  bool is_moving() const { return this->state_ == State::MOVING; }
  bool is_idle() const { return this->state_ == State::IDLE; }
  
  void add_move_callback(std::function<void()> &&move_callback);
  void add_error_callback(std::function<void(const LogString *)> &&error_callback);

 protected:
  State state_{State::IDLE};

  CallbackManager<void()> move_callbacks_ {};
  CallbackManager<void(const LogString *)> error_callbacks_ {};
};

}
}