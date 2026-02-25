#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace pyramidtouch {

// STM32 Touch status registers (same as BSP header)
static const uint8_t TOUCH1_STATUS_REG_ADDR = 0x00;
static const uint8_t TOUCH2_STATUS_REG_ADDR = 0x01;
static const uint8_t TOUCH3_STATUS_REG_ADDR = 0x02;
static const uint8_t TOUCH4_STATUS_REG_ADDR = 0x03;

// Swipe event codes (published to numeric sensor if enabled)
enum SwipeEventCode : uint8_t {
  LEFT_UP = 1,     // Touch1 -> Touch2
  LEFT_DOWN = 2,   // Touch2 -> Touch1
  RIGHT_UP = 3,    // Touch4 -> Touch3
  RIGHT_DOWN = 4,  // Touch3 -> Touch4
};

class PyramidTouchComponent : public i2c::I2CDevice, public PollingComponent {
 public:
  void setup() override {}
  void dump_config() override;
  void update() override;

  // Codegen setters
  void set_touch_sensor(uint8_t idx, sensor::Sensor *s) {
    if (idx < 4) touch_sensors_[idx] = s;
  }
  void set_swipe_event_sensor(sensor::Sensor *s) { swipe_event_sensor_ = s; }
  void set_publish_swipe_event(bool v) { publish_swipe_event_ = v; }
  void set_swipe_timeout_ms(uint32_t ms) { swipe_timeout_ms_ = ms; }

 protected:
  bool read_touch_pressed_(uint8_t touch_num, bool &pressed);
  void handle_swipe_detection_(uint32_t now_ms, bool t1, bool t2, bool t3, bool t4);

  sensor::Sensor *touch_sensors_[4] = {nullptr, nullptr, nullptr, nullptr};
  sensor::Sensor *swipe_event_sensor_{nullptr};

  // swipe detection state
  bool last_state_[4] = {false, false, false, false};
  uint8_t left_swipe_first_{0};   // 1 or 2, 0 if none
  uint8_t right_swipe_first_{0};  // 3 or 4, 0 if none
  uint32_t left_swipe_time_ms_{0};
  uint32_t right_swipe_time_ms_{0};
  bool publish_swipe_event_{true};
  uint32_t swipe_timeout_ms_{500};
};

}  // namespace pyramidtouch
}  // namespace esphome
