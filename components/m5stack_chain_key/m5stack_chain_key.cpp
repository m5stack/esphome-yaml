#include "m5stack_chain_key.h"

#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_chain_key {

static const char *const TAG = "m5stack_chain_key";

void ChainKeyBinarySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up M5Stack Chain Key (device_id=%u)...", this->device_id_);
}

void ChainKeyBinarySensor::update() {
  uint8_t status = 0;
  ChainStatus res = this->get_key_status_(this->device_id_, &status, 100);

  if (res == CHAIN_OK) {
    bool pressed = (status != 0);
    this->publish_state(pressed);
  } else {
    ESP_LOGW(TAG, "Failed to read key status (status=0x%02X)", static_cast<uint8_t>(res));
  }
}

ChainStatus ChainKeyBinarySensor::get_key_status_(uint8_t id, uint8_t *status, uint32_t timeout_ms) {
  if (status == nullptr) {
    return CHAIN_PARAMETER_ERROR;
  }

  ChainStatus res = CHAIN_OK;

  if (this->acquire_mutex_()) {
    this->cmd_buffer_size_ = 0;

    this->send_packet_(id, CHAIN_BUTTON_GET_STATUS, this->cmd_buffer_, this->cmd_buffer_size_);

    if (this->wait_for_data_(id, CHAIN_BUTTON_GET_STATUS, timeout_ms)) {
      if (this->check_packet_(this->return_packet_, this->return_packet_size_)) {
        *status = this->return_packet_[6];
      } else {
        res = CHAIN_RETURN_PACKET_ERROR;
      }
    } else {
      res = CHAIN_TIMEOUT;
    }

    this->release_mutex_();
  } else {
    res = CHAIN_BUSY;
  }

  return res;
}

bool ChainKeyBinarySensor::acquire_mutex_() {
  uint32_t start = millis();
  while (true) {
    if (!this->mutex_locked_) {
      this->mutex_locked_ = true;
      return true;
    }
    if (millis() - start >= 1000) {
      return false;
    }
    delay(1);
  }
}

void ChainKeyBinarySensor::release_mutex_() { this->mutex_locked_ = false; }

void ChainKeyBinarySensor::send_packet_(uint16_t id, uint8_t cmd, const uint8_t *buffer, uint16_t size) {
  uint16_t cmd_size = 3 + size;
  this->send_buffer_size_ = size + 9;

  this->send_buffer_[0] = PACK_HEAD_HIGH;
  this->send_buffer_[1] = PACK_HEAD_LOW;
  this->send_buffer_[2] = cmd_size & 0xFF;
  this->send_buffer_[3] = (cmd_size >> 8) & 0xFF;
  this->send_buffer_[4] = id;
  this->send_buffer_[5] = cmd;

  if (size > 0 && buffer != nullptr) {
    memcpy(this->send_buffer_ + 6, buffer, size);
  }

  uint8_t crc = this->calculate_crc_(this->send_buffer_, this->send_buffer_size_);
  this->send_buffer_[this->send_buffer_size_ - 3] = crc;
  this->send_buffer_[this->send_buffer_size_ - 2] = PACK_END_HIGH;
  this->send_buffer_[this->send_buffer_size_ - 1] = PACK_END_LOW;

  this->write_array(this->send_buffer_, this->send_buffer_size_);
}

bool ChainKeyBinarySensor::wait_for_data_(uint16_t id, uint8_t cmd, uint32_t timeout_ms) {
  uint32_t start = millis();
  while (millis() - start < timeout_ms) {
    if (this->available() > 0) {
      this->read_buffer_();
      bool ok = this->process_buffer_data_(id, cmd);
      if (ok) {
        return true;
      }
    }
    delay(1);
  }
  return false;
}

void ChainKeyBinarySensor::read_buffer_() {
  uint32_t start_time = millis();
  while (millis() - start_time < TIMEOUT_MS) {
    if (this->available() > 0 && this->receive_buffer_size_ < RECEIVE_BUFFER_SIZE) {
      uint8_t byte;
      if (this->read_byte(&byte)) {
        this->receive_buffer_[this->receive_buffer_size_++] = byte;
        start_time = millis();
      } else {
        break;
      }
    } else {
      break;
    }
  }
}

bool ChainKeyBinarySensor::process_buffer_data_(uint16_t id, uint8_t cmd) {
  uint16_t start_index = 0;
  bool packet_found = false;

  while ((this->receive_buffer_size_ - start_index) >= PACK_SIZE_MIN) {
    if (this->receive_buffer_[start_index] == PACK_HEAD_HIGH &&
        this->receive_buffer_[start_index + 1] == PACK_HEAD_LOW) {
      uint16_t length = static_cast<uint16_t>(this->receive_buffer_[start_index + 2]) |
                        (static_cast<uint16_t>(this->receive_buffer_[start_index + 3]) << 8);

      uint16_t packet_size = 4 + length + 2;

      if ((this->receive_buffer_size_ - start_index) < packet_size) {
        break;
      }

      if (this->receive_buffer_[start_index + packet_size - 2] != PACK_END_HIGH ||
          this->receive_buffer_[start_index + packet_size - 1] != PACK_END_LOW) {
        start_index++;
        continue;
      }

      uint8_t *packet_data = this->receive_buffer_ + start_index;
      uint8_t packet_id = packet_data[4];
      uint8_t packet_cmd = packet_data[5];

      if (!this->check_packet_(packet_data, packet_size)) {
        start_index++;
        continue;
      }

      if (packet_id == id && packet_cmd == cmd) {
        memcpy(this->return_packet_, packet_data, packet_size);
        this->return_packet_size_ = packet_size;
        packet_found = true;
      }

      uint16_t remaining_size = this->receive_buffer_size_ - (start_index + packet_size);
      if (remaining_size > 0) {
        memmove(this->receive_buffer_, this->receive_buffer_ + start_index + packet_size, remaining_size);
      }
      this->receive_buffer_size_ = remaining_size;
      start_index = 0;
    } else {
      start_index++;
    }
  }

  return packet_found;
}

bool ChainKeyBinarySensor::check_packet_(const uint8_t *buffer, uint16_t size) const {
  if (size < PACK_SIZE_MIN) {
    return false;
  }

  if (buffer[0] != PACK_HEAD_HIGH || buffer[1] != PACK_HEAD_LOW || buffer[size - 1] != PACK_END_LOW ||
      buffer[size - 2] != PACK_END_HIGH) {
    return false;
  }

  uint16_t length = (static_cast<uint16_t>(buffer[3]) << 8) | buffer[2];
  length += 6;
  if (length != size) {
    return false;
  }

  uint8_t crc = this->calculate_crc_(buffer, size);
  if (crc != buffer[size - 3]) {
    return false;
  }

  return true;
}

uint8_t ChainKeyBinarySensor::calculate_crc_(const uint8_t *buffer, uint16_t size) const {
  uint8_t crc8 = 0;
  for (uint16_t i = 4; i < (size - 3); i++) {
    crc8 += buffer[i];
  }
  return crc8;
}

}  // namespace m5stack_chain_key
}  // namespace esphome
