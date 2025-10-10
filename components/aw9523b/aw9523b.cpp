#include "aw9523b.h"

namespace esphome {
namespace aw9523b{

static const char *TAG = "aw9523b";
    
#define AW9523B_ERROR_FAILED(err) \
  if (!(err)) { \
    this->mark_failed(); \
    return; \
  }

    
void AW9523B::setup()
{
    ESP_LOGD(TAG, "Setting up AW9523B IO expander.");
    AW9523B_ERROR_FAILED(this->write_byte(0x7f, 0x00)); // Reset AW9523
    AW9523B_ERROR_FAILED(this->write_byte(0x02, 0x24)); // set P0 AW RST 
    AW9523B_ERROR_FAILED(this->write_byte(0x03, 0x80)); // set P1 BOOST EN 
    AW9523B_ERROR_FAILED(this->write_byte(0x04, 0x18)); // CONFIG P0
    AW9523B_ERROR_FAILED(this->write_byte(0x05, 0x0c)); // CONFIG P1
    AW9523B_ERROR_FAILED(this->write_byte(0x11, 0x10)); // GCR P0 port is Push-Pull mode.
    AW9523B_ERROR_FAILED(this->write_byte(0x12, 0xFF)); // LEDMODE_P0
    AW9523B_ERROR_FAILED(this->write_byte(0x13, 0xFF)); // LEDMODE_P1  
    
    ESP_LOGD(TAG, "Setting up AW9523B Success.");
}

void AW9523B::loop() {}

void AW9523B::dump_config() {}

} // namespace aw8523b
    
    
}