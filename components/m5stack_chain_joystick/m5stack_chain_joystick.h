#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace m5stack_chain_joystick {

enum ChainStatus : uint8_t {
  CHAIN_OK = 0x00,
  CHAIN_PARAMETER_ERROR = 0x01,
  CHAIN_RETURN_PACKET_ERROR = 0x02,
  CHAIN_BUSY = 0x04,
  CHAIN_TIMEOUT = 0x05,
};

enum JoystickCommand : uint8_t {
  CHAIN_JOYSTICK_GET_ADC_XY_MAPPED_INT16_VALUE = 0x34,
};

static const uint8_t PACK_HEAD_HIGH = 0xAA;
static const uint8_t PACK_HEAD_LOW = 0x55;
static const uint8_t PACK_END_HIGH = 0x55;
static const uint8_t PACK_END_LOW = 0xAA;

static const uint8_t PACK_SIZE_MIN = 0x09;

static const uint16_t RECEIVE_BUFFER_SIZE = 1024;
static const uint16_t SEND_BUFFER_SIZE = 256;
static const uint16_t CMD_BUFFER_SIZE = 256;

static const uint32_t TIMEOUT_MS = 1;

class ChainJoystickSensor : public sensor::Sensor,
                            public PollingComponent,
                            public uart::UARTDevice {
 public:
  void set_device_id(uint8_t id) { this->device_id_ = id; }
  void set_axis(uint8_t axis) { this->axis_ = axis; }

  void setup() override;
  void update() override;

 protected:
  ChainStatus get_mapped_int16_value_(uint8_t id, int16_t *x_value, int16_t *y_value, uint32_t timeout_ms = 100);

  bool acquire_mutex_();
  void release_mutex_();

  void send_packet_(uint16_t id, uint8_t cmd, const uint8_t *buffer, uint16_t size);
  bool wait_for_data_(uint16_t id, uint8_t cmd, uint32_t timeout_ms);

  void read_buffer_();
  bool process_buffer_data_(uint16_t id, uint8_t cmd);

  bool check_packet_(const uint8_t *buffer, uint16_t size) const;
  uint8_t calculate_crc_(const uint8_t *buffer, uint16_t size) const;

  uint8_t device_id_{1};
  uint8_t axis_{0};

  bool mutex_locked_{false};

  uint8_t cmd_buffer_[CMD_BUFFER_SIZE] = {0};
  uint16_t cmd_buffer_size_{0};

  uint8_t return_packet_[CMD_BUFFER_SIZE] = {0};
  uint16_t return_packet_size_{0};

  uint8_t receive_buffer_[RECEIVE_BUFFER_SIZE] = {0};
  uint16_t receive_buffer_size_{0};

  uint8_t send_buffer_[SEND_BUFFER_SIZE] = {0};
  uint16_t send_buffer_size_{0};
};

}  // namespace m5stack_chain_joystick
}  // namespace esphome
