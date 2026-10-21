#include "m5stack_roller_485_i2c.h"
#include "esphome/core/log.h"

#ifdef USE_SWITCH
#include "../m5stack_roller/switch/m5stack_roller_switch.h"
#endif
#ifdef USE_NUMBER
#include "../m5stack_roller/number/m5stack_roller_number.h"
#endif
#ifdef USE_SELECT
#include "../m5stack_roller/select/m5stack_roller_select.h"
#endif
#ifdef USE_LIGHT
#include "../m5stack_roller/light/m5stack_roller_light.h"
#endif
#ifdef USE_BUTTON
#include "../m5stack_roller/button/m5stack_roller_button.h"
#endif

namespace esphome {
namespace m5stack_roller_485_i2c {

static const char *const TAG = "m5stack_roller_485_i2c";

bool RollerMotor485I2C::write_reg(uint8_t reg, const uint8_t *data, size_t len) {
  auto err = this->write_register(reg, data, len);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "I2C write reg 0x%02X failed", reg);
    return false;
  }
  return true;
}

bool RollerMotor485I2C::read_reg(uint8_t reg, uint8_t *data, size_t len) {
  auto err = this->read_register(reg, data, len);
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "I2C read reg 0x%02X failed", reg);
    return false;
  }
  return true;
}

bool RollerMotor485I2C::write_reg_int32(uint8_t reg, int32_t value) {
  uint8_t buf[4];
  m5stack_roller::from_int32_le(value, buf);
  return write_reg(reg, buf, 4);
}

bool RollerMotor485I2C::write_reg_uint32(uint8_t reg, uint32_t value) {
  uint8_t buf[4];
  m5stack_roller::from_uint32_le(value, buf);
  return write_reg(reg, buf, 4);
}

int32_t RollerMotor485I2C::read_reg_int32(uint8_t reg) {
  uint8_t buf[4] = {};
  read_reg(reg, buf, 4);
  return m5stack_roller::to_int32_le(buf);
}

void RollerMotor485I2C::write_motor_config_byte(uint8_t offset, uint8_t value) {
  // Single-byte write at REG_MOTOR_CONFIG + offset
  // I2C register is REG_MOTOR_CONFIG; the I2C driver on the device uses
  // the register address as the base and offset within the register block.
  write_reg(REG_MOTOR_CONFIG + offset, &value, 1);
}

void RollerMotor485I2C::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Roller485I2C motor_id=%u", motor_id_);
}

void RollerMotor485I2C::update() {
  switch (readback_cycle_) {
    case 0: {
      // Speed readback
      float speed = read_reg_int32(REG_SPEED_READBACK) / 100.0f;
      publish_speed(speed);
      break;
    }
    case 1: {
      // Position readback
      float pos = read_reg_int32(REG_POSITION_READBACK) / 100.0f;
      publish_position(pos);
      break;
    }
    case 2: {
      // Current readback
      float cur = read_reg_int32(REG_CURRENT_READBACK) / 100.0f;
      publish_current(cur);
      break;
    }
    case 3: {
      // VIN + Temp + Encoder (system register 0x30: B G R Mode | VIN(4) | Temp(4) | Encoder(4))
      uint8_t sys[16] = {};
      if (read_reg(REG_SYSTEM, sys, 16)) {
        // Layout at 0x30: [B][G][R][Mode] = bytes 0-3 (RGB)
        //                 [VIN x4] = bytes 4-7
        //                 [Temp x4] = bytes 8-11
        //                 [Encoder x4] = bytes 12-15
        float vin  = m5stack_roller::to_int32_le(sys + 4) / 100.0f;
        float temp = static_cast<float>(m5stack_roller::to_int32_le(sys + 8));
        int32_t enc = m5stack_roller::to_int32_le(sys + 12);
        publish_vin(vin);
        publish_temperature(temp);
        publish_encoder(enc);
      }
      break;
    }
    case 4: {
      // Motor status
      uint8_t status_buf[2] = {};
      if (read_reg(REG_MOTOR_CONFIG + CFG_OFF_MOTOR_STATUS, status_buf, 2)) {
        publish_status_mode_error(status_buf[0], 0, status_buf[1]);
      }
      break;
    }
  }
  readback_cycle_ = (readback_cycle_ + 1) % 5;
}

void RollerMotor485I2C::dump_config() {
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "Roller485I2C:");
  ESP_LOGCONFIG(TAG, "  Motor ID: %u", motor_id_);
}

// ---- Command implementations ----

void RollerMotor485I2C::cmd_motor_output(bool enable) {
  uint8_t v = enable ? 1 : 0;
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_OUTPUT, &v, 1);
}

void RollerMotor485I2C::cmd_set_mode(uint8_t mode) {
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_MODE, &mode, 1);
}

void RollerMotor485I2C::cmd_speed(int32_t speed_x100, int32_t max_current_x100) {
  write_reg_int32(REG_SPEED_SETTING, speed_x100);
  write_reg_int32(REG_SPEED_MAX_CURRENT, max_current_x100);
}

void RollerMotor485I2C::cmd_position(int32_t pos_x100, int32_t max_current_x100) {
  write_reg_int32(REG_POSITION_SETTING, pos_x100);
  write_reg_int32(REG_POSITION_MAX_CURRENT, max_current_x100);
}

void RollerMotor485I2C::cmd_current(int32_t current_x100) {
  write_reg_int32(REG_CURRENT_SETTING, current_x100);
}

void RollerMotor485I2C::cmd_speed_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  // PID stored as 12 consecutive bytes at REG_SPEED_PID
  uint8_t buf[12];
  m5stack_roller::from_uint32_le(kp, buf);
  m5stack_roller::from_uint32_le(ki, buf + 4);
  m5stack_roller::from_uint32_le(kd, buf + 8);
  write_reg(REG_SPEED_PID, buf, 12);
}

void RollerMotor485I2C::cmd_position_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  uint8_t buf[12];
  m5stack_roller::from_uint32_le(kp, buf);
  m5stack_roller::from_uint32_le(ki, buf + 4);
  m5stack_roller::from_uint32_le(kd, buf + 8);
  write_reg(REG_POSITION_PID, buf, 12);
}

void RollerMotor485I2C::cmd_save_to_flash() {
  uint8_t v = 1;
  write_reg(REG_FLASH_FW_ADDR, &v, 1);
}

void RollerMotor485I2C::cmd_release_stall_protection() {
  uint8_t v = 1;
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_REMOVE_PROTECT, &v, 1);
}

void RollerMotor485I2C::cmd_stall_protection(bool enable) {
  uint8_t v = enable ? 1 : 0;
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_STALL_PROTECT, &v, 1);
}

void RollerMotor485I2C::cmd_over_range_protection(bool enable) {
  uint8_t v = enable ? 1 : 0;
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_OVER_RANGE, &v, 1);
}

void RollerMotor485I2C::cmd_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t mode, uint8_t brightness) {
  // REG_SYSTEM (0x30): [B][G][R][Mode]
  uint8_t buf[4] = {b, g, r, mode};
  write_reg(REG_SYSTEM, buf, 4);
  // RGB brightness at REG_BUS_CONFIG + 2
  write_reg(REG_BUS_CONFIG + 2, &brightness, 1);
}

void RollerMotor485I2C::cmd_encoder(int32_t value) {
  // Encoder Counter at REG_SYSTEM + 12 (bytes 12-15)
  uint8_t buf[4];
  m5stack_roller::from_int32_le(value, buf);
  write_reg(REG_SYSTEM + 12, buf, 4);
}

void RollerMotor485I2C::cmd_button_switching_mode(bool enable) {
  uint8_t v = enable ? 1 : 0;
  write_reg(REG_MOTOR_CONFIG + CFG_OFF_BUTTON_SWITCH, &v, 1);
}

}  // namespace m5stack_roller_485_i2c
}  // namespace esphome
