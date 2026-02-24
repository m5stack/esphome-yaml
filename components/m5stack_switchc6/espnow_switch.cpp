#include "espnow_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/application.h"

namespace esphome {
namespace espnow_switch {

static const char *const TAG = "espnow_switch";

void ESPNowSwitch::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ESPNow Switch...");
  // 无需在 C++ 层注册广播回调；依赖乐观状态与重试发送
}

void ESPNowSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "ESPNow Switch:");
  LOG_SWITCH("  ", "Switch", this);
  ESP_LOGCONFIG(TAG, "  MAC Address: %02X:%02X:%02X:%02X:%02X:%02X", 
                this->mac_address_[0], this->mac_address_[1], this->mac_address_[2],
                this->mac_address_[3], this->mac_address_[4], this->mac_address_[5]);
  ESP_LOGCONFIG(TAG, "  Response Token: %s", this->response_token_.c_str());
  ESP_LOGCONFIG(TAG, "  Retry Count: %d", this->retry_count_);
  ESP_LOGCONFIG(TAG, "  Retry Interval: %dms", this->retry_interval_);
}

void ESPNowSwitch::write_state(bool state) {
  ESP_LOGD(TAG, "Setting switch to %s", state ? "ON" : "OFF");
  
  // 设置命令：1 = ON, 0 = OFF
  std::string cmd = state ? "1" : "0";
  this->current_command_ = cmd;
  // 初始化重试状态（非阻塞）
  this->response_received_ = false;
  this->attempts_sent_ = 0;
  this->pending_send_ = true;
  this->send_in_flight_ = false;
  this->last_send_ms_ = 0;
  // 立即尝试发送一次（后续重试由 loop() 节流）
  this->send_command_(cmd);

  // 发布状态（乐观模式）
  this->publish_state(state);
}

void ESPNowSwitch::send_command_(const std::string &cmd) {
  // 单次发送，不阻塞；使用回调节流（只允许一个 in-flight）
  if (this->send_in_flight_) {
    return;
  }

  // 获取当前 WiFi 信道
  int channel = this->espnow_->get_wifi_channel();

  // 构建消息格式: MAC-ADDRESS=CMD;ch=CHANNEL;
  char data[64];
  snprintf(data, sizeof(data), "%02X%02X-%02X%02X-%02X%02X=%s;ch=%d;",
           this->mac_address_[0], this->mac_address_[1],
           this->mac_address_[2], this->mac_address_[3],
           this->mac_address_[4], this->mac_address_[5],
           cmd.c_str(), channel);

  // 发送 ESPNow 消息（避免每次重试都分配 vector，减少 heap 压力）
  this->send_in_flight_ = true;
  this->last_send_ms_ = millis();
  this->attempts_sent_++;
  const size_t payload_len = strlen(data);

  auto cb = [this, data_str = std::string(data)](esp_err_t status) {
    this->send_in_flight_ = false;
    if (status == ESP_OK) {
      ESP_LOGV(TAG, "ESPNow message sent (attempt %d/%d): %s", this->attempts_sent_, this->retry_count_, data_str.c_str());
    } else {
      ESP_LOGW(TAG, "Failed to send ESPNow message (attempt %d/%d): %s", this->attempts_sent_, this->retry_count_, esp_err_to_name(status));
    }
  };

  esp_err_t result = this->espnow_->send(this->mac_address_, (const uint8_t *) data, payload_len, cb);
  if (result != ESP_OK) {
    // send() 没有入队成功，回调不会触发，手动释放 in-flight
    this->send_in_flight_ = false;
    ESP_LOGW(TAG, "ESPNow send() failed immediately (attempt %d/%d): %s", this->attempts_sent_, this->retry_count_, esp_err_to_name(result));
  }
}

void ESPNowSwitch::on_espnow_broadcast(const uint8_t *data, size_t len) {
  // 将数据转换为字符串
  std::string response((char*)data, len);
  
  // 检查响应中是否包含我们的匹配令牌
  if (response.find(this->response_token_) != std::string::npos) {
    ESP_LOGI(TAG, "Response received (matched token): %s", this->response_token_.c_str());
    this->response_received_ = true;
    this->pending_send_ = false;
  }
}

void ESPNowSwitch::loop() {
  if (!this->pending_send_)
    return;
  if (this->response_received_) {
    this->pending_send_ = false;
    return;
  }
  if (this->attempts_sent_ >= this->retry_count_) {
    ESP_LOGW(TAG, "No response received after %d attempts", this->retry_count_);
    this->pending_send_ = false;
    return;
  }
  // 若上一次发送仍在进行，等待回调释放
  if (this->send_in_flight_) {
    return;
  }
  // 间隔到达后再重试
  uint32_t now = millis();
  if (this->last_send_ms_ == 0 || now - this->last_send_ms_ >= this->retry_interval_) {
    this->send_command_(this->current_command_);
  }
}

}  // namespace espnow_switch
}  // namespace esphome
