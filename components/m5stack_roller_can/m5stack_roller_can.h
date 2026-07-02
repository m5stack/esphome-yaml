#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/canbus/canbus.h"
#include "../m5stack_roller/m5stack_roller.h"

namespace esphome {
namespace m5stack_roller_can {

using m5stack_roller::RollerBase;

// 29-bit extended CAN ID layout for M5Stack BLDC:
//   bit28~24: communication type (cmd)
//   bit23~8:  data area 2 (bit15~8=host_id for most cmds)
//   bit7~0:   target motor CAN ID

static inline uint32_t build_can_id(uint8_t comm_type, uint8_t host_id, uint8_t motor_id) {
  return (static_cast<uint32_t>(comm_type) << 24) | (static_cast<uint32_t>(host_id) << 8) | motor_id;
}

// Communication types
static constexpr uint8_t CAN_CMD_GET_ID       = 0x00;
static constexpr uint8_t CAN_CMD_FEEDBACK     = 0x02;
static constexpr uint8_t CAN_CMD_ENABLE       = 0x03;
static constexpr uint8_t CAN_CMD_STOP         = 0x04;
static constexpr uint8_t CAN_CMD_SET_CAN_ID   = 0x07;
static constexpr uint8_t CAN_CMD_RELEASE_PROT = 0x09;
static constexpr uint8_t CAN_CMD_SAVE_FLASH   = 0x0A;
static constexpr uint8_t CAN_CMD_SET_BPS      = 0x0B;
static constexpr uint8_t CAN_CMD_STALL_ON     = 0x0C;
static constexpr uint8_t CAN_CMD_STALL_OFF    = 0x0D;
static constexpr uint8_t CAN_CMD_READ_PARAM   = 0x11;
static constexpr uint8_t CAN_CMD_WRITE_PARAM  = 0x12;

// Parameter indices (Table 1)
static constexpr uint16_t PARAM_SAVE_FLASH      = 0x7002;
static constexpr uint16_t PARAM_RELEASE_PROT    = 0x7003;
static constexpr uint16_t PARAM_ON_OFF          = 0x7004;
static constexpr uint16_t PARAM_RUN_MODE        = 0x7005;
static constexpr uint16_t PARAM_CURRENT         = 0x7006;
static constexpr uint16_t PARAM_SPEED           = 0x700A;
static constexpr uint16_t PARAM_POSITION        = 0x7016;
static constexpr uint16_t PARAM_POS_MAX_CURRENT = 0x7017;
static constexpr uint16_t PARAM_SPD_MAX_CURRENT = 0x7018;
static constexpr uint16_t PARAM_SPEED_KP        = 0x7020;
static constexpr uint16_t PARAM_SPEED_KI        = 0x7021;
static constexpr uint16_t PARAM_SPEED_KD        = 0x7022;
static constexpr uint16_t PARAM_POSITION_KP     = 0x7023;
static constexpr uint16_t PARAM_POSITION_KI     = 0x7024;
static constexpr uint16_t PARAM_POSITION_KD     = 0x7025;
static constexpr uint16_t PARAM_SPEED_RB        = 0x7030;
static constexpr uint16_t PARAM_POSITION_RB     = 0x7031;
static constexpr uint16_t PARAM_CURRENT_RB      = 0x7032;
static constexpr uint16_t PARAM_ENCODER         = 0x7033;
static constexpr uint16_t PARAM_VIN             = 0x7034;
static constexpr uint16_t PARAM_TEMP            = 0x7035;
static constexpr uint16_t PARAM_RGB_MODE        = 0x7050;
static constexpr uint16_t PARAM_RGB_COLOR       = 0x7051;
static constexpr uint16_t PARAM_RGB_BRIGHTNESS  = 0x7052;

class RollerMotorCAN;

// Routes all matching extended CAN frames to a RollerMotorCAN instance
class RollerCANReceiver : public canbus::CanbusTrigger {
 public:
  // Match all extended frames (id=0, mask=0)
  RollerCANReceiver(canbus::Canbus *parent, RollerMotorCAN *motor)
      : CanbusTrigger(parent, 0x00000000, 0x00000000, true), motor_(motor) {}

  void setup() override;

 protected:
  RollerMotorCAN *motor_;
};

class RollerMotorCAN : public RollerBase, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_canbus(canbus::Canbus *bus) { canbus_ = bus; }
  void set_host_id(uint8_t id) { host_id_ = id; }

  void on_can_frame(uint32_t can_id, const std::vector<uint8_t> &data);

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
  canbus::Canbus *canbus_{nullptr};
  uint8_t host_id_{0};
  uint8_t readback_cycle_{0};

  void send_can(uint8_t comm_type, const uint8_t *data8);
  void write_param(uint16_t index, const uint8_t *data4);
  void read_param(uint16_t index);
  void process_feedback(const uint8_t *data8);
  void process_param_response(uint16_t index, const uint8_t *data4);
};

}  // namespace m5stack_roller_can
}  // namespace esphome
