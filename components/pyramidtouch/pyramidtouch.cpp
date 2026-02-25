#include "pyramidtouch.h"

namespace esphome {
namespace pyramidtouch {

static const char *const TAG = "pyramidtouch";

void PyramidTouchComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "PyramidTouch Component (STM32 touch @ 0x%02X)", this->address_);
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "publish_swipe_event: %s, swipe_timeout_ms: %u",
                publish_swipe_event_ ? "true" : "false", (unsigned) swipe_timeout_ms_);
}

bool PyramidTouchComponent::read_touch_pressed_(uint8_t touch_num, bool &pressed) {
  uint8_t reg;
  switch (touch_num) {
    case 1: reg = TOUCH1_STATUS_REG_ADDR; break;
    case 2: reg = TOUCH2_STATUS_REG_ADDR; break;
    case 3: reg = TOUCH3_STATUS_REG_ADDR; break;
    case 4: reg = TOUCH4_STATUS_REG_ADDR; break;
    default: return false;
  }
  uint8_t data = 0;
  if (!this->read_byte(reg, &data)) {
    return false;
  }
  pressed = (data & 0x01) != 0;
  return true;
}

void PyramidTouchComponent::handle_swipe_detection_(uint32_t now_ms, bool t1, bool t2, bool t3, bool t4) {
  if (!publish_swipe_event_ || swipe_event_sensor_ == nullptr) {
    // keep state but do not publish
  }

  // Left side (Touch1 & Touch2)
  if ((t1 && !last_state_[0]) || (t2 && !last_state_[1])) {
    if (left_swipe_first_ == 0) {
      if (t1 && !last_state_[0]) { left_swipe_first_ = 1; left_swipe_time_ms_ = now_ms; }
      else if (t2 && !last_state_[1]) { left_swipe_first_ = 2; left_swipe_time_ms_ = now_ms; }
    } else {
      if (now_ms - left_swipe_time_ms_ <= swipe_timeout_ms_) {
        if (left_swipe_first_ == 1 && t2 && !last_state_[1]) {
          if (publish_swipe_event_ && swipe_event_sensor_) swipe_event_sensor_->publish_state((int) LEFT_UP);
          left_swipe_first_ = 0;
        } else if (left_swipe_first_ == 2 && t1 && !last_state_[0]) {
          if (publish_swipe_event_ && swipe_event_sensor_) swipe_event_sensor_->publish_state((int) LEFT_DOWN);
          left_swipe_first_ = 0;
        }
      } else {
        left_swipe_first_ = 0;
      }
    }
  }
  if (!t1 && !t2 && left_swipe_first_ != 0) left_swipe_first_ = 0;
  if (left_swipe_first_ != 0 && (now_ms - left_swipe_time_ms_ > swipe_timeout_ms_)) left_swipe_first_ = 0;

  // Right side (Touch3 & Touch4)
  if ((t3 && !last_state_[2]) || (t4 && !last_state_[3])) {
    if (right_swipe_first_ == 0) {
      if (t3 && !last_state_[2]) { right_swipe_first_ = 3; right_swipe_time_ms_ = now_ms; }
      else if (t4 && !last_state_[3]) { right_swipe_first_ = 4; right_swipe_time_ms_ = now_ms; }
    } else {
      if (now_ms - right_swipe_time_ms_ <= swipe_timeout_ms_) {
        if (right_swipe_first_ == 3 && t4 && !last_state_[3]) {
          if (publish_swipe_event_ && swipe_event_sensor_) swipe_event_sensor_->publish_state((int) RIGHT_DOWN);
          right_swipe_first_ = 0;
        } else if (right_swipe_first_ == 4 && t3 && !last_state_[2]) {
          if (publish_swipe_event_ && swipe_event_sensor_) swipe_event_sensor_->publish_state((int) RIGHT_UP);
          right_swipe_first_ = 0;
        }
      } else {
        right_swipe_first_ = 0;
      }
    }
  }
  if (!t3 && !t4 && right_swipe_first_ != 0) right_swipe_first_ = 0;
  if (right_swipe_first_ != 0 && (now_ms - right_swipe_time_ms_ > swipe_timeout_ms_)) right_swipe_first_ = 0;
}

void PyramidTouchComponent::update() {
  bool t_state[4] = {false, false, false, false};
  for (uint8_t i = 0; i < 4; i++) {
    bool pressed = false;
    bool ok = this->read_touch_pressed_(i + 1, pressed);
    if (!ok) {
      ESP_LOGW(TAG, "Read touch%u failed", (unsigned) (i + 1));
      continue;
    }
    t_state[i] = pressed;
    if (this->touch_sensors_[i] != nullptr) {
      this->touch_sensors_[i]->publish_state(pressed ? 1 : 0);
    }
  }

  uint32_t now = millis();
  this->handle_swipe_detection_(now, t_state[0], t_state[1], t_state[2], t_state[3]);

  // update last states
  for (uint8_t i = 0; i < 4; i++) last_state_[i] = t_state[i];
}

}  // namespace pyramidtouch
}  // namespace esphome
