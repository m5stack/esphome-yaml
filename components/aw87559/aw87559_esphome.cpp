#include "aw87559_esphome.h"

namespace esphome {
namespace aw87559 {


#define AW87559_ERROR_FAILED(func) \
  if(!(func)) {                                 \
 this->mark_failed(); \
    return; \
  } 

// Return false; use outside of setup
#define AW87559_ERROR_CHECK(func) \
  if (!(func)) { \
    return false; \
  }


static const char *const TAG = "aw87559";

void AW87559Component::setup() {

  delay(1000);
  ESP_LOGD(TAG, "Setting up AW87559...");

  AW87559_ERROR_FAILED(this->write_byte(AW87559_REG_CHIPID, 0xff));
  AW87559_ERROR_FAILED(this->write_byte(AW87559_REG_SYSCTRL, 0x78));

  ESP_LOGI(TAG, "AW87559 Audio Amplifier initialized @0x%02X", this->address_);

  this->setup_complete_ = true;
}

void AW87559Component::dump_config() {
  ESP_LOGCONFIG(TAG, "AW87559 Audio Amplifier:");
  
  // 打印 I2C 地址信息
  LOG_I2C_DEVICE(this);

  // 打印初始化状态
  ESP_LOGCONFIG(TAG, "  Setup Status: %s", this->setup_complete_ ? "OK" : "FAILED");
}


}  // namespace aw87559
}  // namespace esphome
