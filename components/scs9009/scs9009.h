#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include <driver/uart.h>

namespace esphome {
namespace scs9009 {

// Instructions
#define INST_PING       0x01
#define INST_READ       0x02
#define INST_WRITE      0x03
#define INST_REG_WRITE  0x04
#define INST_REG_ACTION 0x05
#define INST_SYNC_READ  0x82
#define INST_SYNC_WRITE 0x83

// Memory table definition
//-------EPROM (read-only)--------
#define SCSCL_VERSION_L 3
#define SCSCL_VERSION_H 4

//-------EPROM (read and write)--------
#define SCSCL_ID                5
#define SCSCL_BAUD_RATE         6
#define SCSCL_MIN_ANGLE_LIMIT_L 9
#define SCSCL_MIN_ANGLE_LIMIT_H 10
#define SCSCL_MAX_ANGLE_LIMIT_L 11
#define SCSCL_MAX_ANGLE_LIMIT_H 12
#define SCSCL_CW_DEAD           26
#define SCSCL_CCW_DEAD          27

//-------SRAM (read and write)--------
#define SCSCL_TORQUE_ENABLE   40
#define SCSCL_GOAL_POSITION_L 42
#define SCSCL_GOAL_POSITION_H 43
#define SCSCL_GOAL_TIME_L     44
#define SCSCL_GOAL_TIME_H     45
#define SCSCL_GOAL_SPEED_L    46
#define SCSCL_GOAL_SPEED_H    47
#define SCSCL_LOCK            48

//-------SRAM (read only)--------
#define SCSCL_PRESENT_POSITION_L  56
#define SCSCL_PRESENT_POSITION_H  57
#define SCSCL_PRESENT_SPEED_L     58
#define SCSCL_PRESENT_SPEED_H     59
#define SCSCL_PRESENT_LOAD_L      60
#define SCSCL_PRESENT_LOAD_H      61
#define SCSCL_PRESENT_VOLTAGE     62
#define SCSCL_PRESENT_TEMPERATURE 63
#define SCSCL_MOVING              66
#define SCSCL_PRESENT_CURRENT_L   69
#define SCSCL_PRESENT_CURRENT_H   70


class SCS9009Component : public uart::UARTDevice,
                         public Component
{

 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  float get_setup_priority() const override { return setup_priority::DATA; }

  /// high level control API

  /// mid level control API
  int general_write(uint8_t id, uint8_t addr, uint8_t *data, size_t len);

  int reg_write(uint8_t id, uint8_t addr, uint8_t *data, size_t len);
  
  int reg_write_action(uint8_t id = 0xFE);

  void sync_write(uint8_t *id, uint8_t idx,
                  uint8_t addr, uint8_t *data,
                  size_t len);

  int write_byte(uint8_t id, uint8_t addr, uint8_t data);

  int write_word(uint8_t id, uint8_t addr, uint16_t data);

  // Read instruction
  int read(uint8_t id, uint8_t addr, uint8_t *data, size_t len);       

  int read_byte(uint8_t id, uint8_t addr);  // Read 1 byte

  int read_word(uint8_t id, uint8_t addr);  // Read 2 bytes

  int ping(uint8_t id);

  int sync_read_packet_tx(uint8_t *id, uint8_t idx, 
                          uint8_t addr, size_t len);

  int sync_read_packet_rx(uint8_t id, uint8_t *data);

  int sync_read_rx_packet_to_byte();

  int sync_read_rx_packet_to_word(uint8_t neg_bit = 0);

  int write_pos(uint8_t id, uint16_t position, uint16_t time, uint16_t speed);  // Normal write of single servo position command

  int write_pos_ex(uint8_t id, int16_t position, uint16_t speed, uint8_t acc);  // Single servo position command

  int reg_write_pos(uint8_t id, uint16_t position, uint16_t time, uint16_t speed = 0);  // Asynchronously write a single servo position command (RegWriteAction takes effect)

  // Synchronously write multiple servo position commands
  void sync_write_pos(uint8_t *id, uint8_t idx, 
                      uint16_t *position, uint16_t *time,
                      uint16_t *speed);        

  int pwm_mode(uint8_t id);                    // PWM output mode
  int write_pwm(uint8_t id, int16_t pwm_out);       // PWM output mode command
  int switch_mode(uint8_t id, uint8_t mode);  // Switch between position mode and PWM mode
  int enable_torque(uint8_t id, uint8_t enable);    // Torque control command
  int unlock_eprom(uint8_t id);                // eprom unlock
  int lock_eprom(uint8_t id);                  // eprom lock
  int feedback(uint8_t id);                  // Feedback servo information
  int read_pos(uint8_t id);                   // Read position
  int read_speed(uint8_t id);                 // Read speed
  int read_load(uint8_t id);                  // Read the output voltage percentage to the motor (uint8_t id0~1000)
  int read_voltage(uint8_t id);               // Read voltage
  int read_current(uint8_t id);               // read current
  int read_temperature(uint8_t id);                // Read temperature
  int read_move(uint8_t id);                  // Read movement status
  int read_mode(uint8_t id);
  int read_torque_enable(uint8_t id);
  int calibration_ofs(uint8_t id);
  int read_info_value(uint8_t id, int add_input);

 protected:

  /// low level communication API
  /**
   * @brief Internal helper function to write data
   * @param buf output buffer
   * @param len bytes to write
   */

  int write_scs_(uint8_t *buf, size_t len);


  /**
   * @brief Internal helper function to write 1 byte
   */
  int write_scs_(uint8_t data);

  /**
   * @brief Internal helper function to read data
   * @param buf input buffer
   * @param len bytes to read
   */
  int read_scs_(uint8_t *buf, size_t len);


  /**
   *  @brief flush the reading buffer
   */
  void r_flush_scs_();

  /**
   *  @brief Internal helper function to flush writing buffer
   */
  void w_flush_scs_();


  void write_buffer_(uint8_t id, uint8_t addr, uint8_t *data, 
                     size_t len, uint8_t func);

  void host_2_scs_(uint8_t *data_l, uint8_t *data_h, uint16_t data);

  uint16_t scs_2_host_(uint8_t data_l, uint8_t data_h);
  
  int check_head_();

  int ack_(uint8_t id);
  
  uint32_t io_timeout_{100};

  uint8_t error_;
  uint8_t end_;
  uint8_t level_;
  uint8_t sync_read_rx_packet_idx_;
  uint8_t sync_read_rx_packet_len_;
  uint8_t *sync_read_rx_packet_;

  uint8_t mem_[SCSCL_PRESENT_CURRENT_H - SCSCL_PRESENT_POSITION_L + 1];
  int min_angle_[2];
  int max_angle_[2];
};

} // namespace scs9009
} // namespace esphome