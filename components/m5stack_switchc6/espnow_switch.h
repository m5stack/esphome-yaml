#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/espnow/espnow_component.h"
#include <string>

namespace esphome {
namespace espnow_switch {

class ESPNowSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void loop() override;
  
  // 设置 ESPNow 组件引用
  void set_espnow_component(espnow::ESPNowComponent *espnow) { this->espnow_ = espnow; }
  
  // 设置 MAC 地址
  void set_mac_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f) {
    this->mac_address_[0] = a;
    this->mac_address_[1] = b;
    this->mac_address_[2] = c;
    this->mac_address_[3] = d;
    this->mac_address_[4] = e;
    this->mac_address_[5] = f;
  }
  
  // 设置响应匹配令牌
  void set_response_token(const std::string &token) { this->response_token_ = token; }
  
  // 设置重试参数
  void set_retry_count(uint8_t count) { this->retry_count_ = count; }
  void set_retry_interval(uint32_t interval) { this->retry_interval_ = interval; }
  
  // 接收响应的回调
  void on_espnow_broadcast(const uint8_t *data, size_t len);
  // 供 YAML on_broadcast 调用以停止重试
  void handle_broadcast(const uint8_t *data, size_t len) { this->on_espnow_broadcast(data, len); }

 protected:
  void write_state(bool state) override;
  void send_command_(const std::string &cmd);
  
  espnow::ESPNowComponent *espnow_{nullptr};
  uint8_t mac_address_[6];
  std::string response_token_;
  uint8_t retry_count_{12};
  uint32_t retry_interval_{300};
  
  bool response_received_{false};
  std::string current_command_;
  bool pending_send_{false};
  uint8_t attempts_sent_{0};
  uint32_t last_send_ms_{0};

  // Throttle: only one send in-flight at a time (callback clears)
  bool send_in_flight_{false};
};

}  // namespace espnow_switch
}  // namespace esphome
