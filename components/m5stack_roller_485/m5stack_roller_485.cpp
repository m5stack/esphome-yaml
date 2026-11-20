#include "m5stack_roller_485.h"
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
namespace m5stack_roller_485 {

static const char *const TAG = "m5stack_roller_485";

uint8_t RollerMotor485::crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0x00;
  while (len--) {
    crc ^= *data++;
    for (uint8_t i = 0; i < 8; i++)
      crc = (crc & 0x01) ? (crc >> 1) ^ 0x8C : crc >> 1;
  }
  return crc;
}

void RollerMotor485::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Roller485 motor_id=%u", motor_id_);
}

void RollerMotor485::dump_config() {
  ESP_LOGCONFIG(TAG, "Roller485:");
  ESP_LOGCONFIG(TAG, "  Motor ID: %u", motor_id_);
  ESP_LOGCONFIG(TAG, "  Host ID: %u", host_id_);
  ESP_LOGCONFIG(TAG, "  Update interval: %u ms", update_interval_ms_);
}

void RollerMotor485::send_frame(uint8_t cmd, const uint8_t *d1, const uint8_t *d2, const uint8_t *d3) {
  uint8_t frame[RS485_CMD_LEN] = {};
  frame[0] = cmd;
  frame[1] = motor_id_;
  if (d1) memcpy(&frame[2], d1, 4);
  if (d2) memcpy(&frame[6], d2, 4);
  if (d3) memcpy(&frame[10], d3, 4);
  frame[14] = crc8(frame, 14);
  this->send(std::vector<uint8_t>(frame, frame + RS485_CMD_LEN));
}

void RollerMotor485::send_update() {
  uint32_t now = millis();
  if (now - last_update_ < update_interval_ms_) return;
  last_update_ = now;

  const uint8_t zero4[4] = {};
  send_frame(CMD_READBACK_0 + readback_cycle_, zero4);
  readback_cycle_ = (readback_cycle_ + 1) & 0x03;
}

void RollerMotor485::on_uart_multi_byte(uint8_t byte) {
  if (!rx_in_frame_) {
    // Look for 0xAA 0x55 preamble using a 2-byte sliding window
    rx_buf_[0] = rx_buf_[1];
    rx_buf_[1] = byte;
    if (rx_buf_[0] == RS485_PREAMBLE_0 && rx_buf_[1] == RS485_PREAMBLE_1) {
      rx_in_frame_ = true;
      rx_pos_ = 0;
      rx_expected_len_ = 0;  // determined after first byte (cmd)
    }
    return;
  }

  rx_buf_[rx_pos_++] = byte;

  // After first byte we know if this is a readback (extended) response
  if (rx_pos_ == 1) {
    rx_expected_len_ = is_readback_response(byte) ? RS485_RB_RESP_LEN : RS485_STD_RESP_LEN;
  }

  if (rx_pos_ < rx_expected_len_) return;

  // Complete frame received
  rx_in_frame_ = false;
  rx_pos_ = 0;

  uint8_t len = rx_expected_len_;
  uint8_t expected_crc = crc8(rx_buf_, len - 1);
  if (expected_crc != rx_buf_[len - 1]) {
    ESP_LOGW(TAG, "CRC mismatch (got 0x%02X expected 0x%02X)", rx_buf_[len - 1], expected_crc);
    return;
  }
  if (rx_buf_[1] != motor_id_) return;

  process_response(rx_buf_, len);
}

void RollerMotor485::process_response(const uint8_t *p, uint8_t len) {
  using namespace m5stack_roller;
  uint8_t cmd = p[0];
  const uint8_t *d1 = p + 2;
  const uint8_t *d2 = p + 6;
  const uint8_t *d3 = p + 10;

  switch (cmd) {
    case RESP_READBACK_0: {
      // d1=Speed X100, d2=Position X100, d3=Current X100, p[14]=Mode, p[15]=Status, p[16]=Error
      publish_speed(to_int32_le(d1) / 100.0f);
      publish_position(to_int32_le(d2) / 100.0f);
      publish_current(to_int32_le(d3) / 100.0f);
      publish_status_mode_error(p[15], p[14], p[16]);
      break;
    }
    case RESP_READBACK_1: {
      // d1=VIN X100, d2=Temp, d3=Encoder, p[14]=RGB Mode, p[15]=RGB Brightness
      publish_vin(to_int32_le(d1) / 100.0f);
      publish_temperature(static_cast<float>(to_int32_le(d2)));
      publish_encoder(to_int32_le(d3));
      break;
    }
    case RESP_READBACK_2:
    case RESP_READBACK_3:
      // PID readback - not published as sensors currently
      break;
    default:
      break;
  }
}

// ---- Command implementations ----

void RollerMotor485::cmd_motor_output(bool enable) {
  const uint8_t d1[4] = {static_cast<uint8_t>(enable ? 1 : 0), 0, 0, 0};
  send_frame(CMD_MOTOR_SWITCH, d1);
}

void RollerMotor485::cmd_set_mode(uint8_t mode) {
  const uint8_t d1[4] = {mode, 0, 0, 0};
  send_frame(CMD_MODE, d1);
}

void RollerMotor485::cmd_speed(int32_t speed_x100, int32_t max_current_x100) {
  uint8_t d1[4], d2[4];
  m5stack_roller::from_int32_le(speed_x100, d1);
  m5stack_roller::from_int32_le(max_current_x100, d2);
  send_frame(CMD_SPEED, d1, d2);
}

void RollerMotor485::cmd_position(int32_t pos_x100, int32_t max_current_x100) {
  uint8_t d1[4], d2[4];
  m5stack_roller::from_int32_le(pos_x100, d1);
  m5stack_roller::from_int32_le(max_current_x100, d2);
  send_frame(CMD_POSITION, d1, d2);
}

void RollerMotor485::cmd_current(int32_t current_x100) {
  uint8_t d1[4];
  m5stack_roller::from_int32_le(current_x100, d1);
  send_frame(CMD_CURRENT, d1);
}

void RollerMotor485::cmd_speed_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  uint8_t d1[4], d2[4], d3[4];
  m5stack_roller::from_uint32_le(kp, d1);
  m5stack_roller::from_uint32_le(ki, d2);
  m5stack_roller::from_uint32_le(kd, d3);
  send_frame(CMD_SPEED_PID, d1, d2, d3);
}

void RollerMotor485::cmd_position_pid(uint32_t kp, uint32_t ki, uint32_t kd) {
  uint8_t d1[4], d2[4], d3[4];
  m5stack_roller::from_uint32_le(kp, d1);
  m5stack_roller::from_uint32_le(ki, d2);
  m5stack_roller::from_uint32_le(kd, d3);
  send_frame(CMD_POSITION_PID, d1, d2, d3);
}

void RollerMotor485::cmd_save_to_flash() {
  const uint8_t d1[4] = {1, 0, 0, 0};
  send_frame(CMD_SAVE_FLASH, d1);
}

void RollerMotor485::cmd_release_stall_protection() {
  // Doc: Data2[0]=Release Jam Protection, send 1 to unprotect
  const uint8_t d1[4] = {};
  const uint8_t d2[4] = {1, 0, 0, 0};
  send_frame(CMD_REMOVE_PROTECTION, d1, d2);
}

void RollerMotor485::cmd_stall_protection(bool enable) {
  const uint8_t d1[4] = {static_cast<uint8_t>(enable ? 1 : 0), 0, 0, 0};
  send_frame(CMD_JAM_PROTECTION, d1);
}

void RollerMotor485::cmd_over_range_protection(bool enable) {
  const uint8_t d1[4] = {static_cast<uint8_t>(enable ? 1 : 0), 0, 0, 0};
  send_frame(CMD_OVER_RANGE_PROTECTION, d1);
}

void RollerMotor485::cmd_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t mode, uint8_t brightness) {
  const uint8_t d1[4] = {b, g, r, mode};
  const uint8_t d2[4] = {brightness, 0, 0, 0};
  send_frame(CMD_RGB, d1, d2);
}

void RollerMotor485::cmd_encoder(int32_t value) {
  uint8_t d1[4];
  m5stack_roller::from_int32_le(value, d1);
  send_frame(CMD_ENCODER, d1);
}

void RollerMotor485::cmd_button_switching_mode(bool enable) {
  const uint8_t d1[4] = {static_cast<uint8_t>(enable ? 1 : 0), 0, 0, 0};
  send_frame(CMD_BUTTON_SWITCH_MODE, d1);
}

}  // namespace m5stack_roller_485
}  // namespace esphome
