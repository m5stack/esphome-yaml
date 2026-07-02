#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "../m5stack_roller/m5stack_roller.h"
#include "esphome/components/uart_multi/uart_multi.h"

namespace esphome {
namespace m5stack_roller_485 {

using m5stack_roller::RollerBase;

// RS-485 frame format
//   TX: [CmdID][MotorID][D1:4B][D2:4B][D3:4B][CRC8]  = 15 bytes
//   RX standard: 0xAA 0x55 + same 15 bytes            = 17 bytes total
//   RX readback (0x50-0x53): 0xAA 0x55 + 18 bytes     = 20 bytes total
//     extra 3 bytes: Data4, Data5, Data6 before CRC

static constexpr uint8_t RS485_CMD_LEN      = 15;
static constexpr uint8_t RS485_STD_RESP_LEN = 15;
static constexpr uint8_t RS485_RB_RESP_LEN  = 18;  // readback extended
static constexpr uint8_t RS485_PREAMBLE_0   = 0xAA;
static constexpr uint8_t RS485_PREAMBLE_1   = 0x55;
static constexpr uint8_t RS485_RX_BUF_MAX   = 20;

static constexpr uint8_t CMD_MOTOR_SWITCH         = 0x00;
static constexpr uint8_t CMD_MODE                 = 0x01;
static constexpr uint8_t CMD_REMOVE_PROTECTION    = 0x06;
static constexpr uint8_t CMD_SAVE_FLASH           = 0x07;
static constexpr uint8_t CMD_ENCODER              = 0x08;
static constexpr uint8_t CMD_BUTTON_SWITCH_MODE   = 0x09;
static constexpr uint8_t CMD_RGB                  = 0x0A;
static constexpr uint8_t CMD_BPS                  = 0x0B;
static constexpr uint8_t CMD_485_ID               = 0x0C;
static constexpr uint8_t CMD_JAM_PROTECTION       = 0x0D;
static constexpr uint8_t CMD_OVER_RANGE_PROTECTION = 0x0E;
static constexpr uint8_t CMD_SPEED                = 0x20;
static constexpr uint8_t CMD_SPEED_PID            = 0x21;
static constexpr uint8_t CMD_POSITION             = 0x22;
static constexpr uint8_t CMD_POSITION_PID         = 0x23;
static constexpr uint8_t CMD_CURRENT              = 0x24;
static constexpr uint8_t CMD_READBACK_0           = 0x40;
static constexpr uint8_t CMD_READBACK_1           = 0x41;
static constexpr uint8_t CMD_READBACK_2           = 0x42;
static constexpr uint8_t CMD_READBACK_3           = 0x43;

// Response command IDs (cmd + 0x10 for settings, 0x50-0x53 for readback)
static constexpr uint8_t RESP_READBACK_0 = 0x50;
static constexpr uint8_t RESP_READBACK_1 = 0x51;
static constexpr uint8_t RESP_READBACK_2 = 0x52;
static constexpr uint8_t RESP_READBACK_3 = 0x53;

static inline bool is_readback_response(uint8_t cmd) {
  return cmd >= RESP_READBACK_0 && cmd <= RESP_READBACK_3;
}

class RollerMotor485 : public RollerBase, public Component, public uart_multi::UARTMultiDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_host_id(uint8_t id) { host_id_ = id; }
  void set_update_interval(uint32_t ms) { update_interval_ms_ = ms; }

  // UARTMultiDevice interface
  void send_update() override;
  void on_uart_multi_byte(uint8_t byte) override;

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
  uint8_t host_id_{0};
  uint32_t update_interval_ms_{500};
  uint8_t readback_cycle_{0};
  uint32_t last_update_{0};

  // RX state machine
  uint8_t rx_buf_[RS485_RX_BUF_MAX]{};
  uint8_t rx_pos_{0};
  bool rx_in_frame_{false};
  uint8_t rx_expected_len_{0};

  static uint8_t crc8(const uint8_t *data, uint8_t len);
  void send_frame(uint8_t cmd, const uint8_t *d1 = nullptr, const uint8_t *d2 = nullptr, const uint8_t *d3 = nullptr);
  void process_response(const uint8_t *p, uint8_t len);
};

}  // namespace m5stack_roller_485
}  // namespace esphome
