#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "../m5stack_roller/m5stack_roller.h"

namespace esphome {
namespace m5stack_roller_485_i2c {

using m5stack_roller::RollerI2CBase;

// I2C register map (shared with m5stack_roller_can_i2c, except bus config bytes)
// REG_BUS_CONFIG (0x10): byte0=MotorID, byte1=485 BPS, byte2=RGB Brightness

class RollerMotor485I2C : public RollerI2CBase, public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // RollerBase transport commands
  void cmd_motor_output(bool enable) override;
  void cmd_set_mode(uint8_t mode) override;
  void cmd_speed(int32_t speed_x100, int32_t max_current_x100) override;
  void cmd_position(int32_t pos_x100, int32_t max_current_x100) override;
  void cmd_current(int32_t current_x100) override;
  void cmd_speed_pid(uint32_t kp, uint32_t ki, uint32_t kd) override;
  void cmd_position_pid(uint32_t kp, uint32_t ki, uint32_t kd) override;
  void cmd_save_to_flash() override;
  void cmd_release_stall_protection() override;
  void cmd_stall_protection(bool enable) override;
  void cmd_over_range_protection(bool enable) override;
  void cmd_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t mode, uint8_t brightness) override;
  void cmd_encoder(int32_t value) override;
  void cmd_button_switching_mode(bool enable) override;

 protected:
  uint8_t readback_cycle_{0};
  bool write_reg(uint8_t reg, const uint8_t *data, size_t len);
  bool read_reg(uint8_t reg, uint8_t *data, size_t len);
  bool write_reg_int32(uint8_t reg, int32_t value);
  bool write_reg_uint32(uint8_t reg, uint32_t value);
  int32_t read_reg_int32(uint8_t reg);

  // Motor config register cache: 16 bytes at REG_MOTOR_CONFIG (0x00)
  void write_motor_config_byte(uint8_t offset, uint8_t value);
};

}  // namespace m5stack_roller_485_i2c
}  // namespace esphome
