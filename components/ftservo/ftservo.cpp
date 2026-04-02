#include "ftservo.h"
#include "esphome/core/progmem.h"
#include <functional>

namespace esphome {
namespace ftservo {

static const char *TAG = "ftservo";

#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_DEBUG

PROGMEM_STRING_TABLE(FTServoErrString,
  "FT_OK",
  "FT_ERR_NO_REPLY",
  "FT_ERR_CRC_CMP",
  "FT_ERR_SLAVE_ID",
	"FT_ERR_BUFF_LEN"
);

static const LogString *get_err_string(FTServoErr err) {
    return FTServoErrString::get_log_str(static_cast<uint8_t>(err), 0);
}

#endif

void FTServo::add_move_callback(std::function<void()> &&move_callback) {
  this->move_callbacks_.add(std::move(move_callback));
}


void FTServo::add_error_callback(std::function<void(const LogString *)> &&error_callback) {
  this->error_callbacks_.add(std::move(error_callback));
}


}
}