#pragma once

#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <string>
#include <vector>

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_LIGHT
#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_state.h"
#endif

namespace esphome {
namespace m5stack_roller {

// Motor error bit flags
static constexpr uint8_t ROLLER_ERROR_OVERVOLTAGE = 1;
static constexpr uint8_t ROLLER_ERROR_STALLED = 2;
static constexpr uint8_t ROLLER_ERROR_OVERRANGE = 4;

// Scaling factors
static constexpr int32_t ROLLER_SCALE_100 = 100;
static constexpr uint32_t ROLLER_SPEED_KP_SCALE = 100000;
static constexpr uint32_t ROLLER_SPEED_KI_SCALE = 10000000;
static constexpr uint32_t ROLLER_SPEED_KD_SCALE = 100000;

static inline int32_t to_int32_le(const uint8_t *b) {
  return (int32_t)(b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
}
static inline uint32_t to_uint32_le(const uint8_t *b) {
  return (uint32_t)(b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24));
}
static inline void from_int32_le(int32_t v, uint8_t *b) {
  b[0] = v & 0xFF; b[1] = (v >> 8) & 0xFF; b[2] = (v >> 16) & 0xFF; b[3] = (v >> 24) & 0xFF;
}
static inline void from_uint32_le(uint32_t v, uint8_t *b) {
  b[0] = v & 0xFF; b[1] = (v >> 8) & 0xFF; b[2] = (v >> 16) & 0xFF; b[3] = (v >> 24) & 0xFF;
}

class RollerBase {
 public:
  // ---- Abstract transport commands ----
  virtual void cmd_motor_output(bool enable) = 0;
  virtual void cmd_set_mode(uint8_t mode) = 0;
  virtual void cmd_speed(int32_t speed_x100, int32_t max_current_x100) = 0;
  virtual void cmd_position(int32_t pos_x100, int32_t max_current_x100) = 0;
  virtual void cmd_current(int32_t current_x100) = 0;
  virtual void cmd_speed_pid(uint32_t kp, uint32_t ki, uint32_t kd) = 0;
  virtual void cmd_position_pid(uint32_t kp, uint32_t ki, uint32_t kd) = 0;
  virtual void cmd_save_to_flash() = 0;
  virtual void cmd_release_stall_protection() = 0;
  virtual void cmd_stall_protection(bool enable) = 0;
  virtual void cmd_over_range_protection(bool enable) = 0;
  virtual void cmd_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t mode, uint8_t brightness) = 0;
  virtual void cmd_encoder(int32_t value) = 0;
  virtual void cmd_button_switching_mode(bool enable) = 0;

  // ---- Configuration ----
  void set_motor_id(uint8_t id) { motor_id_ = id; }
  uint8_t get_motor_id() const { return motor_id_; }

  // ---- Cached setpoints (updated by number/switch entities before sending) ----
  void set_speed_setpoint(float rpm) { speed_x100_ = static_cast<int32_t>(rpm * 100.0f); }
  void set_speed_max_current(float ma) { speed_max_current_x100_ = static_cast<int32_t>(ma * 100.0f); }
  void set_position_setpoint(float deg) { position_x100_ = static_cast<int32_t>(deg * 100.0f); }
  void set_position_max_current(float ma) { position_max_current_x100_ = static_cast<int32_t>(ma * 100.0f); }
  void set_current_setpoint(float ma) { current_x100_ = static_cast<int32_t>(ma * 100.0f); }

  void set_speed_kp(float v) { speed_kp_ = static_cast<uint32_t>(v * ROLLER_SPEED_KP_SCALE); }
  void set_speed_ki(float v) { speed_ki_ = static_cast<uint32_t>(v * ROLLER_SPEED_KI_SCALE); }
  void set_speed_kd(float v) { speed_kd_ = static_cast<uint32_t>(v * ROLLER_SPEED_KD_SCALE); }
  void set_position_kp(float v) { position_kp_ = static_cast<uint32_t>(v * ROLLER_SPEED_KP_SCALE); }
  void set_position_ki(float v) { position_ki_ = static_cast<uint32_t>(v * ROLLER_SPEED_KI_SCALE); }
  void set_position_kd(float v) { position_kd_ = static_cast<uint32_t>(v * ROLLER_SPEED_KD_SCALE); }
  void set_rgb_brightness(uint8_t v) { rgb_brightness_ = v; }
  void set_rgb_color(uint8_t r, uint8_t g, uint8_t b, uint8_t mode) {
    rgb_r_ = r; rgb_g_ = g; rgb_b_ = b; rgb_mode_ = mode;
  }

  // Convenience send helpers that use cached values
  void send_speed() { this->cmd_speed(speed_x100_, speed_max_current_x100_); }
  void send_position() { this->cmd_position(position_x100_, position_max_current_x100_); }
  void send_current() { this->cmd_current(current_x100_); }
  void send_speed_pid() { this->cmd_speed_pid(speed_kp_, speed_ki_, speed_kd_); }
  void send_position_pid() { this->cmd_position_pid(position_kp_, position_ki_, position_kd_); }
  void send_rgb() { this->cmd_rgb(rgb_r_, rgb_g_, rgb_b_, rgb_mode_, rgb_brightness_); }

  // ---- Sensor entity setters ----
#ifdef USE_SENSOR
  void set_speed_sensor(sensor::Sensor *s) { speed_sensor_ = s; }
  void set_position_sensor(sensor::Sensor *s) { position_sensor_ = s; }
  void set_current_sensor(sensor::Sensor *s) { current_sensor_ = s; }
  void set_vin_sensor(sensor::Sensor *s) { vin_sensor_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }
  void set_encoder_sensor(sensor::Sensor *s) { encoder_sensor_ = s; }
#endif

#ifdef USE_TEXT_SENSOR
  void set_status_text_sensor(text_sensor::TextSensor *s) { status_text_sensor_ = s; }
  void set_mode_text_sensor(text_sensor::TextSensor *s) { mode_text_sensor_ = s; }
  void set_error_text_sensor(text_sensor::TextSensor *s) { error_text_sensor_ = s; }
#endif

 protected:
  uint8_t motor_id_{1};

  // Cached setpoints
  int32_t speed_x100_{0};
  int32_t speed_max_current_x100_{50000};
  int32_t position_x100_{0};
  int32_t position_max_current_x100_{50000};
  int32_t current_x100_{0};
  uint32_t speed_kp_{100000};
  uint32_t speed_ki_{1000000};
  uint32_t speed_kd_{0};
  uint32_t position_kp_{100000};
  uint32_t position_ki_{1000000};
  uint32_t position_kd_{0};
  uint8_t rgb_r_{0}, rgb_g_{0}, rgb_b_{0};
  uint8_t rgb_mode_{0};
  uint8_t rgb_brightness_{50};

#ifdef USE_SENSOR
  sensor::Sensor *speed_sensor_{nullptr};
  sensor::Sensor *position_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *vin_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *encoder_sensor_{nullptr};
#endif

#ifdef USE_TEXT_SENSOR
  text_sensor::TextSensor *status_text_sensor_{nullptr};
  text_sensor::TextSensor *mode_text_sensor_{nullptr};
  text_sensor::TextSensor *error_text_sensor_{nullptr};
#endif

  // ---- Publish helpers (called by transport subclasses on readback) ----
  void publish_speed(float rpm) {
#ifdef USE_SENSOR
    if (speed_sensor_) speed_sensor_->publish_state(rpm);
#endif
  }
  void publish_position(float deg) {
#ifdef USE_SENSOR
    if (position_sensor_) position_sensor_->publish_state(deg);
#endif
  }
  void publish_current(float ma) {
#ifdef USE_SENSOR
    if (current_sensor_) current_sensor_->publish_state(ma);
#endif
  }
  void publish_vin(float v) {
#ifdef USE_SENSOR
    if (vin_sensor_) vin_sensor_->publish_state(v);
#endif
  }
  void publish_temperature(float c) {
#ifdef USE_SENSOR
    if (temperature_sensor_) temperature_sensor_->publish_state(c);
#endif
  }
  void publish_encoder(int32_t enc) {
#ifdef USE_SENSOR
    if (encoder_sensor_) encoder_sensor_->publish_state(static_cast<float>(enc));
#endif
  }
  void publish_status_mode_error(uint8_t status, uint8_t mode, uint8_t error) {
#ifdef USE_TEXT_SENSOR
    if (status_text_sensor_) {
      const char *s = status == 0 ? "Standby" : status == 1 ? "Running" : "Error";
      status_text_sensor_->publish_state(s);
    }
    if (mode_text_sensor_) {
      const char *m = mode == 1 ? "Speed" : mode == 2 ? "Position" : mode == 3 ? "Current" : mode == 4 ? "Encoder" : "Unknown";
      mode_text_sensor_->publish_state(m);
    }
    if (error_text_sensor_) {
      if (error == 0) {
        error_text_sensor_->publish_state("None");
      } else {
        std::string err;
        if (error & ROLLER_ERROR_OVERVOLTAGE) err += "Overvoltage";
        if (error & ROLLER_ERROR_STALLED) { if (!err.empty()) err += ", "; err += "Stalled"; }
        if (error & ROLLER_ERROR_OVERRANGE) { if (!err.empty()) err += ", "; err += "Over Range"; }
        error_text_sensor_->publish_state(err);
      }
    }
#endif
  }
};

// Shared I2C register map base for m5stack_roller_485_i2c and m5stack_roller_can_i2c
// Included by both transport implementations to avoid code duplication
class RollerI2CBase : public RollerBase {
 public:
  // Register addresses
  static constexpr uint8_t REG_MOTOR_CONFIG = 0x00;
  static constexpr uint8_t REG_BUS_CONFIG = 0x10;  // offset 0=ID, 1=BPS, 2=brightness
  static constexpr uint8_t REG_SPEED_SETTING = 0x40;
  static constexpr uint8_t REG_SPEED_MAX_CURRENT = 0x50;
  static constexpr uint8_t REG_SPEED_READBACK = 0x60;
  static constexpr uint8_t REG_SPEED_PID = 0x70;
  static constexpr uint8_t REG_POSITION_SETTING = 0x80;
  static constexpr uint8_t REG_POSITION_MAX_CURRENT = 0x20;
  static constexpr uint8_t REG_POSITION_READBACK = 0x90;
  static constexpr uint8_t REG_POSITION_PID = 0xA0;
  static constexpr uint8_t REG_CURRENT_SETTING = 0xB0;
  static constexpr uint8_t REG_CURRENT_READBACK = 0xC0;
  static constexpr uint8_t REG_SYSTEM = 0x30;  // RGB(B,G,R,mode) | VIN(4B) | Temp(4B) | Encoder(4B)
  static constexpr uint8_t REG_FLASH_FW_ADDR = 0xF0;

  // Motor config register byte offsets
  static constexpr uint8_t CFG_OFF_OUTPUT = 0;
  static constexpr uint8_t CFG_OFF_MODE = 1;
  static constexpr uint8_t CFG_OFF_OVER_RANGE = 9;  // byte A
  static constexpr uint8_t CFG_OFF_BUTTON_SWITCH = 13;  // byte D
  static constexpr uint8_t CFG_OFF_STALL_PROTECT = 14;  // byte E
  static constexpr uint8_t CFG_OFF_REMOVE_PROTECT = 10;  // byte A (write 1)
  static constexpr uint8_t CFG_OFF_MOTOR_STATUS = 11;   // byte B (read)
  static constexpr uint8_t CFG_OFF_MOTOR_ERROR = 12;    // byte C (read)
};

}  // namespace m5stack_roller
}  // namespace esphome
