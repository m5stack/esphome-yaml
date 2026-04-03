#include "scs9009.h"
#include "esphome/components/uart/uart_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace scs9009 {

static const char *TAG = "scs9009";

void SCS9009Component::setup() {
  ESP_LOGD(TAG, "Setting up SCS9009");
  // variables initialization
  this->level_ = 1;
  this->error_ = 0; // initial no error 
  this->end_ = 1;   // the scs9009 used big endian address
}

void SCS9009Component::loop() {

}

void SCS9009Component::dump_config() {
  ESP_LOGCONFIG(TAG, "SCS9009 Config: ");
  if ( this->is_failed() ) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
  this->check_uart_settings(1000000, 1, uart::UART_CONFIG_PARITY_NONE, 8);
}

int SCS9009Component::read_scs_(uint8_t *buf, size_t nlen) {

  size_t count = 0;
  uint32_t t_begin = millis();

  uint8_t byte;

  while(1) {
    size_t available = this->available();

    if ( available > 0 ) {
      bool status = this->read_array(&byte, 1);
      if ( status ) {
        if ( buf ) buf[count] = byte;
        count++;
      }
      t_begin = millis();
    }

    if (count >= nlen) {
      break;
    }

    uint32_t t_user = millis() - t_begin;

    if (t_user > 500) {
      this->status_set_warning(LOG_STR("Timeout when reading byte(s)"));
      break;
    }
    yield();
  } 
  return count;
}

int SCS9009Component::write_scs_(uint8_t *buf, size_t len) {

  if ( buf == nullptr ) {
    this->status_set_warning(LOG_STR("Reading NULL in write buffer, skipping."));
    return 0;
  }

  // assume the write is successful
  this->write_array(buf, len);

  return len;
} 

int SCS9009Component::write_scs_(uint8_t data) {
  this->write_array(&data, 1);
  return 1;
}

void SCS9009Component::r_flush_scs_() {
  while (this->available()) {
    uint8_t dummy;
    this->read_array(&dummy, 1);
  }
}

void SCS9009Component::w_flush_scs_() {
  this->flush();
}


void SCS9009Component::host_2_scs_(uint8_t *data_l, uint8_t *data_h, uint16_t data) 
{
  if ( this->end_ ) {
    *data_l = (data >> 8);
    *data_h = (data & 0xFF);
  } else {
    *data_h = (data >> 8);
    *data_l = (data & 0xFF);
  }
}

uint16_t SCS9009Component::scs_2_host_(uint8_t data_l, uint8_t data_h) {

  uint16_t data;

  if ( this->end_ ) {
    data = ((data_l << 8) | data_h);
  } else {
    data = ((data_h) << 8 | data_l);
  }

  return data;

}

void SCS9009Component::write_buffer_(uint8_t id, uint8_t addr, uint8_t *data, 
                                     size_t len, uint8_t func) 
{

  uint8_t msg_len = 2;

  uint8_t buf[6];

  uint8_t checksum = 0;

  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = id;
  buf[4] = func;

  if ( data ) {
    msg_len += len + 1;
    buf[3] = msg_len;
    buf[5] = addr;

    this->write_scs_(buf, 6);
  } else {
    buf[3] = msg_len;
    this->write_scs_(buf, 5);
  }

  checksum = id + msg_len + func + addr;

  if ( data ) {
    for (size_t i = 0; i < len; ++i) {
      checksum += data[i];
    }

    this->write_scs_(data, len);
  }

  this->write_scs_(~checksum);
}

int SCS9009Component::general_write(uint8_t id, uint8_t addr, uint8_t *data, size_t len)
{
  this->r_flush_scs_();
  this->write_buffer_(id, addr, data, len, INST_WRITE);
  this->w_flush_scs_();
  return this->ack_(id);
}

int SCS9009Component::reg_write(uint8_t id, uint8_t addr, uint8_t *data, size_t len)
{
  this->r_flush_scs_();
  this->write_buffer_(id, addr, data, len, INST_REG_WRITE);
  this->w_flush_scs_();
  return this->ack_(id);
}

int SCS9009Component::reg_write_action(uint8_t id) {
  this->r_flush_scs_();
  this->write_buffer_(id, 0, NULL, 0, INST_REG_ACTION);
  this->w_flush_scs_();
  // this is a fixed ID 0xFE for broadcast
  return this->ack_(id);
}

void SCS9009Component::sync_write(uint8_t *id, uint8_t idx,
                                  uint8_t addr, uint8_t *data,
                                  size_t len)
{
  this->r_flush_scs_();

  uint8_t msg_len = (len + 1) * idx + 4;
  uint8_t sum = 0;
  uint8_t buf[7];

  buf[0] = 0xFF;
  buf[1] = 0xFF;
  buf[2] = 0xFE;
  buf[3] = msg_len;
  buf[4] = INST_SYNC_WRITE;
  buf[5] = addr;
  buf[6] = len;

  this->write_scs_(buf, 7);

  sum = 0xFE + msg_len + INST_SYNC_WRITE + addr + len;

  for ( size_t i = 0; i < idx; ++i ) {
    this->write_scs_(id[i]);
    this->write_scs_(data + i * len, len);
    sum += id[i];

    for ( size_t j = 0; j < len; ++j ) {
      sum += data[ i * len + j ];
    }

  }

  this->write_scs_(~sum);

  this->w_flush_scs_();

}


int SCS9009Component::write_byte(uint8_t id, uint8_t addr, uint8_t data) {
  this->r_flush_scs_();
  this->write_buffer_(id, addr, &data, 1, INST_WRITE);
  this->w_flush_scs_();
  return this->ack_(id);
}

int SCS9009Component::write_word(uint8_t id, uint8_t addr, uint16_t data) {
  uint8_t buf[2];
  this->host_2_scs_(buf + 0, buf + 1, data);
  this->r_flush_scs_();
  this->write_buffer_(id, addr, buf, 2, INST_WRITE);
  this->w_flush_scs_();
  return this->ack_(id);
}

int SCS9009Component::read(uint8_t id, uint8_t addr, uint8_t *data, size_t len)
{
  this->r_flush_scs_();

  uint8_t read_len = static_cast<uint8_t>(len);
  this->write_buffer_(id, addr, &read_len, 1, INST_READ);

  this->w_flush_scs_();

  if ( !this->check_head_() ) {
    return 0;
  }

  uint8_t buf[4];

  this->error_ = 0;

  if ( this->read_scs_(buf, 3) != 3 ) {
    return 0;
  }

  int size = this->read_scs_(data, len);

  if ( size != len ) {
    return 0;
  }

  if ( this->read_scs_(buf + 3, 1)  != 1 ) {
    return 0;
  } 

  uint8_t sum = buf[0] + buf[1] + buf[2];

  for( size_t i = 0; i < size; ++i ) {
    sum += data[i];
  }

  sum = ~sum;

  if ( sum != buf[3] ) {
    return 0;
  }

  this->error_ = buf[2];

  return size;
  
}                            

int SCS9009Component::read_byte(uint8_t id, uint8_t addr) {
  uint8_t data;
  int size = this->read(id, addr, &data, 1);
  if ( size != 1 ) {
    return -1;
  } else {
    return data;
  }
}

int SCS9009Component::read_word(uint8_t id, uint8_t addr) {
  uint8_t data[2];

  uint16_t word_data;

  int size = this->read(id, addr, data, 2);

  if ( size != 2 ) {
    return -1;
  }

  word_data = this->scs_2_host_(data[0], data[1]);

  return word_data;
}


int SCS9009Component::check_head_() {
  uint8_t data;

  uint8_t buf[2] = {0, 0};

  uint8_t count = 0;


  while (1) {
    if ( !this->read_scs_(&data, 1) ) {
      return 0;
    }
    buf[1] = buf[0];
    buf[0] = data;

    if ( buf[0] == 0xFF && buf[1] == 0xFF) {
      break;
    }

    count++;

    if ( count > 10 ) {
      this->status_set_warning(LOG_STR("Max timeout retries reached, breaking.."));
      return 0;
    }
  }

  this->status_clear_warning();

  return 1;
}

int SCS9009Component::ping(uint8_t id) {
  this->r_flush_scs_();

  this->write_buffer_(id, 0, NULL, 0, INST_PING);

  this->w_flush_scs_();

  this->error_ = 0;

  if ( !this->check_head_() ) {
    this->status_set_warning(LOG_STR("Failed to check the head..."));
    return -1;
  }

  uint8_t buf[4];

  if ( this->read_scs_(buf, 4) != 4 ) {
    this->status_set_warning(LOG_STR("Failed to read from SCS"));
    return -1;
  }

  if ( buf[1] != 2 ) {
    this->status_set_warning(LOG_STR("Read incorrect length, data length mismatch"));
    return -1;
  }

  uint8_t checksum = ~(buf[0] + buf[1] + buf[2]);

  if ( checksum != buf[3] ) {
    this->status_set_warning(LOG_STR("Invalid checksum"));
    return -1;
  }

  this->error_ = buf[2];

  this->status_clear_warning();
  return buf[0];
}

int SCS9009Component::ack_(uint8_t id) {
  this->error_ = 0;
  if ( id != 0xFE && this->level_ ) {
    if ( this->check_head_() ) {
      return 0;
    }

    uint8_t buf[4];

    if( this->read_scs_(buf, 4) != 4 ) {
      return 0;
    }

    if ( buf[0] != id ) {
      return 0;
    }

    if ( buf[1] != 2 ) {
      return 0;
    }

    uint8_t checksum = ~(buf[0] + buf[1] + buf[2]);

    if ( checksum != buf[3] ) {
      return 0;
    }

    this->error_ = buf[2];

  }

  return 1;
}


int SCS9009Component::sync_read_packet_tx(uint8_t *id, uint8_t idx, 
                                           uint8_t addr, size_t len)

{
  this->sync_read_rx_packet_len_ = len;

  uint8_t checksum = (4 + 0xFE) + idx + addr + len + INST_SYNC_READ;

  this->write_scs_(0xFF);
  this->write_scs_(0xFF);
  this->write_scs_(0xFE);
  this->write_scs_(idx + 4);
  this->write_scs_(INST_SYNC_READ);
  this->write_scs_(addr);
  this->write_scs_(len);

  for ( size_t i = 0; i < idx; ++i ) {
    this->write_scs_(id[i]);
    checksum += id[i];
  }

  checksum = ~checksum;
  this->write_scs_(checksum);
  return len;
}


int SCS9009Component::sync_read_packet_rx(uint8_t id, uint8_t *data) {
  this->sync_read_rx_packet_ = data;

  this->sync_read_rx_packet_idx_ = 0;

  uint8_t buf[4];

  if ( !this->check_head_() ) {
    return 0;
  }

  if ( this->read_scs_(buf, 3) != 3 ) {
    return 0;
  }

  if ( buf[0] != id ) {
    return 0;
  }

  if ( buf[1] != this->sync_read_rx_packet_len_ + 2 ) {
    return 0;
  }

  this->error_ = buf[2];

  if ( this->read_scs_(data, this->sync_read_rx_packet_len_)
       != this->sync_read_rx_packet_len_  )
  {
    return 0;
  }

  return this->sync_read_rx_packet_len_;
}


int SCS9009Component::sync_read_rx_packet_to_byte() {
    if (this->sync_read_rx_packet_idx_ >= this->sync_read_rx_packet_len_) {
        return -1;
    }
    return this->sync_read_rx_packet_[this->sync_read_rx_packet_idx_++];
}

int SCS9009Component::sync_read_rx_packet_to_word(uint8_t neg_bit) {
  if ( (this->sync_read_rx_packet_idx_ + 1) 
       >= this->sync_read_rx_packet_len_ )
  {
    return -1;
  }

  uint16_t word_data = this->scs_2_host_(this->sync_read_rx_packet_[this->sync_read_rx_packet_idx_],
                                         this->sync_read_rx_packet_[this->sync_read_rx_packet_idx_ + 1]);

  this->sync_read_rx_packet_idx_ += 2;
  
  if ( neg_bit ) {
    if ( word_data & (1 << neg_bit) ) {
      word_data = ~(word_data & ~(1 << neg_bit));
    }
  }

  return word_data;
}

int SCS9009Component::write_pos(uint8_t id, uint16_t position, uint16_t time, uint16_t speed) 
{
  uint8_t buf[6];
  this->host_2_scs_(buf + 0, buf + 1, position);
  this->host_2_scs_(buf + 2, buf + 3, time);
  this->host_2_scs_(buf + 4, buf + 5, speed);

  return this->general_write(id, SCSCL_GOAL_POSITION_L, buf, 6);
} 

int SCS9009Component::write_pos_ex(uint8_t id, int16_t position, uint16_t speed, uint8_t acc) 
{

  // (void)ACC;  // ACC parameter is not used in this implementation
  uint16_t time = 0;
  uint8_t buf[6];
  this->host_2_scs_(buf + 0, buf + 1, position);
  this->host_2_scs_(buf + 2, buf + 3, time);
  this->host_2_scs_(buf + 4, buf + 5, speed);

  return this->general_write(id, SCSCL_GOAL_POSITION_L, buf, 6);

} 

int SCS9009Component::reg_write_pos(uint8_t id, uint16_t position, uint16_t time, uint16_t speed) {

  uint8_t buf[6];
  this->host_2_scs_(buf + 0, buf + 1, position);
  this->host_2_scs_(buf + 2, buf + 3, time);
  this->host_2_scs_(buf + 4, buf + 5, speed);

  return this->reg_write(id, SCSCL_GOAL_POSITION_L, buf, 6);
} 

void SCS9009Component::sync_write_pos(uint8_t *id, uint8_t idx, 
                                      uint16_t *position, uint16_t *time, 
                                      uint16_t *speed) 
{
  uint8_t offbuf[6 * idx];
  for (uint8_t i = 0; i < idx; i++) {
      uint16_t T, V;
      if ( time ) {
          T = time[i];
      } else {
          T = 0;
      }
      if ( speed ) {
          V = speed[i];
      } else {
          V = 0;
      }
      this->host_2_scs_(offbuf + i * 6 + 0, offbuf + i * 6 + 1, position[i]);
      this->host_2_scs_(offbuf + i * 6 + 2, offbuf + i * 6 + 3, T);
      this->host_2_scs_(offbuf + i * 6 + 4, offbuf + i * 6 + 5, V);
  }
  this->sync_write(id, idx, SCSCL_GOAL_POSITION_L, offbuf, 6);
} 

int SCS9009Component::pwm_mode(uint8_t id) {
  uint8_t buf[4];
  buf[0] = 0;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  return this->general_write(id, SCSCL_MIN_ANGLE_LIMIT_L, buf, 4);
} 

int SCS9009Component::write_pwm(uint8_t id, int16_t pwm_out) {
  if ( pwm_out < 0 ) {
    pwm_out = -pwm_out;
    pwm_out |= (1 << 10);
  }
  uint8_t buf[2];
  this->host_2_scs_(buf + 0, buf + 1, pwm_out);
  return this->general_write(id, SCSCL_GOAL_TIME_L, buf, 2);
} 

int SCS9009Component::switch_mode(uint8_t id, uint8_t mode) {

  if ( id < 0 || id > 1 ) {
    return -1;  // Invalid id
  }
  if ( mode > 1 ) {
    return -2;  // Invalid mode
  }

  if ( mode == 1 ) {  // PWM mode
      // Store current angle limits
      this->min_angle_[id] = this->read_word(id, SCSCL_MIN_ANGLE_LIMIT_L);
      this->max_angle_[id] = this->read_word(id, SCSCL_MAX_ANGLE_LIMIT_L);
      if ( this->min_angle_[id] == -1 || this->max_angle_[id] == -1 ) {
          return -3;  // Failed to read angle limits
      }
      this->pwm_mode(id);  // Switch to PWM mode
      return 0;
  } else {  // Position mode (mode == 0)
      if ( this->write_word(id, SCSCL_MIN_ANGLE_LIMIT_L, 
                     static_cast<uint16_t>(this->min_angle_[id])) != 1 ) {
          return -4;  // Failed to write min angle limit
      }
      if ( this->write_word(id ,SCSCL_MAX_ANGLE_LIMIT_L, 
                     static_cast<uint16_t>(this->max_angle_[id])) != 1 ) {
          return -5;  // Failed to write max angle limit
      }
      return 0;
  }

} 

int SCS9009Component::enable_torque(uint8_t id, uint8_t enable) {
  return this->write_byte(id, SCSCL_TORQUE_ENABLE, enable);
} 

int SCS9009Component::unlock_eprom(uint8_t id) {
  return this->write_byte(id, SCSCL_LOCK, 0);
} 

int SCS9009Component::lock_eprom(uint8_t id) {
  return this->write_byte(id, SCSCL_LOCK, 1);
} 

int SCS9009Component::feedback(uint8_t id) {
    int len = this->read(id, SCSCL_PRESENT_POSITION_L, this->mem_, sizeof(this->mem_));
    if ( len != sizeof(this->mem_) ) {
        this->error_ = 1;
        return -1;
    }
    this->error_ = 0;
    return len;
} 

int SCS9009Component::read_pos(uint8_t id) {
  int pos = -1;
  if ( id == -1 ) {
      pos = this->mem_[SCSCL_PRESENT_POSITION_L - SCSCL_PRESENT_POSITION_L];
      pos <<= 8;
      pos |= this->mem_[SCSCL_PRESENT_POSITION_H - SCSCL_PRESENT_POSITION_L];
  } else {
      this->error_ = 0;
      pos = this->read_word(id, SCSCL_PRESENT_POSITION_L);
      if ( pos == -1 ) {
          this->error_ = 1;
      }
  }
  return pos;
} 

int SCS9009Component::read_speed(uint8_t id) {
  int speed = -1;
  if ( id == -1 ) {
      speed = this->mem_[SCSCL_PRESENT_SPEED_L - SCSCL_PRESENT_POSITION_L];
      speed <<= 8;
      speed |= this->mem_[SCSCL_PRESENT_SPEED_H - SCSCL_PRESENT_POSITION_L];
  } else {
      this->error_   = 0;
      speed = this->read_word(id, SCSCL_PRESENT_SPEED_L);
      if (speed == -1) {
          this->error_ = 1;
          return -1;
      }
  }
  if ( !this->error_ && (speed & (1 << 15)) ) {
      speed = -(speed & ~(1 << 15));
  }
  return speed;

} 

int SCS9009Component::read_load(uint8_t id) {
  int load = -1;
  if ( id == -1 ) {
      load = this->mem_[SCSCL_PRESENT_LOAD_L - SCSCL_PRESENT_POSITION_L];
      load <<= 8;
      load |= this->mem_[SCSCL_PRESENT_LOAD_H - SCSCL_PRESENT_POSITION_L];
  } else {
      this->error_  = 0;
      load = this->read_word(id, SCSCL_PRESENT_LOAD_L);
      if ( load == -1 ) {
          this->error_ = 1;
      }
  }
  if ( !this->error_ && (load & (1 << 10)) ) {
      load = -(load & ~(1 << 10));
  }
  return load;

} 

int SCS9009Component::read_voltage(uint8_t id) {
  int voltage = -1;
  if ( id == -1 ) {
      voltage = this->mem_[SCSCL_PRESENT_VOLTAGE - SCSCL_PRESENT_POSITION_L];
  } else {
      this->error_     = 0;
      voltage = this->read_byte(id, SCSCL_PRESENT_VOLTAGE);
      if ( voltage == -1 ) {
          this->error_ = 1;
      }
  }
  return voltage;
}

int SCS9009Component::read_current(uint8_t id) {
    int current = -1;
    if ( id == -1 ) {
        current = this->mem_[SCSCL_PRESENT_CURRENT_L - SCSCL_PRESENT_POSITION_L];
        current <<= 8;
        current |= this->mem_[SCSCL_PRESENT_CURRENT_H - SCSCL_PRESENT_POSITION_L];
    } else {
        this->error_     = 0;
        current = this->read_word(id, SCSCL_PRESENT_CURRENT_L);
        if ( current == -1 ) {
            this->error_ = 1;
            return -1;
        }
    }
    if ( !this->error_ && (current & (1 << 15)) ) {
        current = -(current & ~(1 << 15));
    }
    return current;
} 

int SCS9009Component::read_temperature(uint8_t id) {
  int temp = -1;
  if ( id == -1 ) {
      temp = this->mem_[SCSCL_PRESENT_TEMPERATURE - SCSCL_PRESENT_POSITION_L];
  } else {
      this->error_ = 0;
      temp = this->read_byte(id, SCSCL_PRESENT_TEMPERATURE);
      if ( temp == -1 ) {
          this->error_ = 1;
      }
  }
  return temp;
} 

int SCS9009Component::read_move(uint8_t id) {
    int move = -1;
    if ( id == -1 ) {
        move = this->mem_[SCSCL_MOVING - SCSCL_PRESENT_POSITION_L];
    } else {
        this->error_ = 0;
        move = this->read_byte(id, SCSCL_MOVING);
        if ( move == -1 ) {
            this->error_ = 1;
        }
    }
    return move;
}

int SCS9009Component::read_mode(uint8_t id) {
  int mode = -1;
  mode = this->read_word(id, SCSCL_MIN_ANGLE_LIMIT_L);
  if ( mode == 0 ) {
      return 1;
  } else if ( mode > 0 ) {
      return 0;
  }
  return mode;
}

int SCS9009Component::read_torque_enable(uint8_t id) {
  int enabled = -1;
  enabled = this->read_word(id, SCSCL_TORQUE_ENABLE);
  return enabled;
}

int SCS9009Component::calibration_ofs(uint8_t id) {
  // literally does nothing...
  return -1;
}

int SCS9009Component::read_info_value(uint8_t id, int add_input) {
  int info = -1;
  info = this->read_word(id, add_input);
  return info;
}

} // namespace scs9009
} // namespace esphome