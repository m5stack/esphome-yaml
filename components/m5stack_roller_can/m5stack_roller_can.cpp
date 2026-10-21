#include "m5stack_roller_can.h"
#include "esphome/core/log.h"
#include "esphome/core/automation.h"

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
namespace m5stack_roller_can {

static const char *const TAG = "m5stack_roller_can";

// ---- RollerCANReceiver ----

class RollerCANAction : public Action<std::vector<uint8_t>, uint32_t, bool> {
 public:
  explicit RollerCANAction(RollerMotorCAN *motor) : motor_(motor) {}
  void play(std::vector<uint8_t> data, uint32_t can_id, bool rtr) override {
    if (!rtr) motor_->on_can_frame(can_id, data);
  }

 protected:
  RollerMotorCAN *motor_;
};

void RollerCANReceiver::setup() {
  canbus::CanbusTrigger::setup();
  auto *automation = new Automation<std::vector<uint8_t>, uint32_t, bool>(this);
  automation->add_actions({new RollerCANAction(motor_)});
}

// ---- RollerMotorCAN ----

void RollerMotorCAN::setup() {
  ESP_LOGCONFIG(TAG, "Setting up RollerCAN motor_id=%u", motor_id_);
}

void RollerMotorCAN::dump_config() {
  ESP_LOGCONFIG(TAG, "RollerCAN:");
  ESP_LOGCONFIG(TAG, "  Motor ID: %u", motor_id_);
  ESP_LOGCONFIG(TAG, "  Host ID: %u", host_id_);
}

void RollerMotorCAN::send_can(uint8_t comm_type, const uint8_t *data8) {
  canbus::CanFrame frame{};
  frame.can_id = build_can_id(comm_type, host_id_, motor_id_);
  frame.use_extended_id = true;
  frame.remote_transmission_request = false;
  frame.can_data_length_code = 8;
  memcpy(frame.data, data8, 8);
  canbus_->send_message(&frame);
}

void RollerMotorCAN::write_param(uint16_t index, const uint8_t *data4) {
  // comm_type 0x12 write: Byte0~1=index, Byte2~3=00, Byte4~7=param data
  uint8_t d[8] = {};
  d[0] = index & 0xFF;
  d[1] = (index >> 8) & 0xFF;
  if (data4) memcpy(d + 4, data4, 4);
  send_can(CAN_CMD_WRITE_PARAM, d);
}

void RollerMotorCAN::read_param(uint16_t index) {
  uint8_t d[8] = {};
  d[0] = index & 0xFF;
  d[1] = (index >> 8) & 0xFF;
  send_can(CAN_CMD_READ_PARAM, d);
}

void RollerMotorCAN::update() {
  static const uint16_t readback_params[] = {
    PARAM_SPEED_RB, PARAM_POSITION_RB, PARAM_CURRENT_RB,
    PARAM_VIN, PARAM_TEMP, PARAM_ENCODER
  };
  constexpr uint8_t num_params = sizeof(readback_params) / sizeof(readback_params[0]);
  read_param(readback_params[readback_cycle_]);
  readback_cycle_ = (readback_cycle_ + 1) % num_params;
}

void RollerMotorCAN::on_can_frame(uint32_t can_id, const std::vector<uint8_t> &data) {
  uint8_t dest = can_id & 0xFF;
  if (dest != motor_id_ && dest != 0xFF) return;

  uint8_t comm_type = (can_id >> 24) & 0x1F;
  if (data.size() < 8) return;
  const uint8_t *d = data.data();

  switch (comm_type) {
    case CAN_CMD_FEEDBACK:
      process_feedback(d);
      break;
    case CAN_CMD_READ_PARAM: {
      uint16_t index = d[0] | (static_cast<uint16_t>(d[1]) << 8);
      process_param_response(index, d + 4);
      break;
    }
    default:
      break;
  }
}

void RollerMotorCAN::process_feedback(const uint8_t *d) {
  // Motor feedback (comm_type 0x02) from doc:
  // Byte0~1: speed (rpm), Byte2~3: position (°), Byte4~5: current (mA), Byte6~7: VIN (V)
  // All int16_t, values are direct units (not ×100)
  int16_t spd = static_cast<int16_t>(d[0] | (static_cast<uint16_t>(d[1]) << 8));
  int16_t pos = static_cast<int16_t>(d[2] | (static_cast<uint16_t>(d[3]) << 8));
  int16_t cur = static_cast<int16_t>(d[4] | (static_cast<uint16_t>(d[5]) << 8));
  int16_t vin = static_cast<int16_t>(d[6] | (static_cast<uint16_t>(d[7]) << 8));
  publish_speed(static_cast<float>(spd));
  publish_position(static_cast<float>(pos));
  publish_current(static_cast<float>(cur));
  publish_vin(static_cast<float>(vin));
}

void RollerMotorCAN::process_param_response(uint16_t index, const uint8_t *data4) {
  using namespace m5stack_roller;
  switch (index) {
    case PARAM_SPEED_RB:     publish_speed(to_int32_le(data4) / 100.0f); break;
    case PARAM_POSITION_RB:  publish_position(to_int32_le(data4) / 100.0f); break;
    case PARAM_CURRENT_RB:   publish_current(to_int32_le(data4) / 100.0f); break;
    case PARAM_VIN:          publish_vin(to_int32_le(data4) / 100.0f); break;
    case PARAM_TEMP:         publish_temperature(static_cast<float>(to_int32_le(data4))); break;
    case PARAM_ENCODER:      publish_encoder(to_int32_le(data4)); break;
    default: break;
  }
}

// ---- Command implementations ----

void RollerMotorCAN::cmd_motor_output(bool enable) {
  const uint8_t zero8[8] = {};
  send_can(enable ? CAN_CMD_ENABLE : CAN_CMD_STOP, zero8);
}

void RollerMotorCAN::cmd_set_mode(uint8_t mode) {
  const uint8_t d[4] = {mode, 0, 0, 0};
  write_param(PARAM_RUN_MODE, d);
}

void RollerMotorCAN::cmd_speed(int32_t speed_x100, int32_t max_current_x100) {
  uint8_t d[4];
  m5stack_roller::from_int32_le(speed_x100, d);      write_param(PARAM_SPEED, d);
  m5stack_roller::from_int32_le(max_current_x100, d); write_param(PARAM_SPD_MAX_CURRENT, d);
}

void RollerMotorCAN::cmd_position(int32_t pos_x100, int32_t max_current_x100) {
  uint8_t d[4];
  m5stack_roller::from_int32_le(pos_x100, d);          write_param(PARAM_POSITION, d);
  m5stack_roller::from_int32_le(max_current_x100, d);  write_param(PARAM_POS_MAX_CURRENT, d);
}

void RollerMotorCAN::cmd_current(int32_t current_x100) {
  uint8_t d[4];
  m5stack_roller::from_int32_le(current_x100, d);
  write_param(PARAM_CURRENT, d);
}

void RollerMotorCAN::cmd_speed_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  uint8_t d[4];
  m5stack_roller::from_uint32_le(kp, d);  write_param(PARAM_SPEED_KP, d);
  m5stack_roller::from_uint32_le(ki, d);  write_param(PARAM_SPEED_KI, d);
  m5stack_roller::from_uint32_le(kd, d);  write_param(PARAM_SPEED_KD, d);
}

void RollerMotorCAN::cmd_position_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  uint8_t d[4];
  m5stack_roller::from_uint32_le(kp, d);  write_param(PARAM_POSITION_KP, d);
  m5stack_roller::from_uint32_le(ki, d);  write_param(PARAM_POSITION_KI, d);
  m5stack_roller::from_uint32_le(kd, d);  write_param(PARAM_POSITION_KD, d);
}

void RollerMotorCAN::cmd_save_to_flash() {
  const uint8_t d[4] = {2, 0, 0, 0};  // value > 1 saves to flash
  write_param(PARAM_SAVE_FLASH, d);
}

void RollerMotorCAN::cmd_release_stall_protection() {
  const uint8_t zero8[8] = {};
  send_can(CAN_CMD_RELEASE_PROT, zero8);
}

void RollerMotorCAN::cmd_stall_protection(bool enable) {
  const uint8_t zero8[8] = {};
  send_can(enable ? CAN_CMD_STALL_ON : CAN_CMD_STALL_OFF, zero8);
}

void RollerMotorCAN::cmd_over_range_protection(bool /*enable*/) {
  ESP_LOGW(TAG, "over_range_protection not available via direct CAN; use m5stack_roller_can_i2c");
}

void RollerMotorCAN::cmd_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t mode, uint8_t brightness) {
  uint32_t color = b | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(r) << 16);
  uint8_t d[4];
  m5stack_roller::from_uint32_le(color, d);         write_param(PARAM_RGB_COLOR, d);
  d[0] = mode; d[1] = d[2] = d[3] = 0;          write_param(PARAM_RGB_MODE, d);
  d[0] = brightness; d[1] = d[2] = d[3] = 0;    write_param(PARAM_RGB_BRIGHTNESS, d);
}

void RollerMotorCAN::cmd_encoder(int32_t value) {
  uint8_t d[4];
  m5stack_roller::from_int32_le(value, d);
  write_param(PARAM_ENCODER, d);
}

void RollerMotorCAN::cmd_button_switching_mode(bool /*enable*/) {
  ESP_LOGW(TAG, "button_switching_mode not available via direct CAN protocol");
}

}  // namespace m5stack_roller_can
}  // namespace esphome
