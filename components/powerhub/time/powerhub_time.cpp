#include "powerhub_time.h"

namespace esphome {
namespace powerhub {

static const char *TAG = "powerhub.time";


// Strange values ...
const LogString *reg_val_to_weekday(uint8_t val) {
  switch (val) {
    case 1:
    case 80: return LOG_STR("Sunday");
    case 2: return LOG_STR("Monday");
    case 4: return LOG_STR("Tuesday");
    case 8: return LOG_STR("Wednesday");
    case 10: return LOG_STR("Thursday");
    case 20: return LOG_STR("Friday");
    case 40: return LOG_STR("Saturday");
    default: return LOG_STR("Unkown");
  }
}

static const uint8_t to_std_weekday(uint8_t val) {
    switch (val) {
        case 1:
        case 80: return 1;  // Sunday
        case 2:  return 2;  // Monday
        case 4:  return 3;  // Tuesday
        case 8:  return 4;  // Wednesday
        case 10: return 5;  // Thursday
        case 20: return 6;  // Friday
        case 40: return 7;  // Saturday
        default: return 0;  // Unknown
    }
}

static const inline uint8_t to_custom_weekday(uint8_t val) {
    switch (val) {
        case 1: return 1;   // Sunday
        case 2: return 2;   // Monday
        case 3: return 4;   // Tuesday
        case 4: return 8;   // Wednesday
        case 5: return 10;  // Thursday
        case 6: return 20;  // Friday
        case 7: return 40;  // Saturday
        default: return 0;  // Unknown
    }
}

void PowerHubTime::read_time() {
    RTC_t time = this->parent_->get_rtc_time();

    ESP_LOGD(TAG, "RTC read time: %0u:%0u:%0u %0u-%0u-%0u Weekday: %s",
                  time.rtc_hour,
                  time.rtc_min,
                  time.rtc_sec,
                  (2000 + time.rtc_year),
                  time.rtc_month,
                  time.rtc_day,
                  reg_val_to_weekday(time.rtc_weekday));

    ESPTime rtc_time{
        .second        = time.rtc_sec,
        .minute        = time.rtc_min,
        .hour          = time.rtc_hour,
        .day_of_week   = to_std_weekday(time.rtc_weekday),
        .day_of_month  = time.rtc_day,
        .day_of_year   = 1,                                 // ignored by recalc_timestamp_utc(false)
        .month         = time.rtc_month,
        .year          = time.rtc_year,
        .is_dst        = false,                             // not used
        .timestamp     = 0                                  // overwritten by recalc_timestamp_utc(false)
    };

    rtc_time.recalc_timestamp_utc(false);

    if (!rtc_time.is_valid()) {
        ESP_LOGE(TAG, "Invalid RTC time, not syncing to system clock.");
        return;
    }

    time::RealTimeClock::synchronize_epoch_(rtc_time.timestamp);
}


void PowerHubTime::write_time() {
    auto now = time::RealTimeClock::utcnow();

    if (!now.is_valid()) {
        ESP_LOGE(TAG, "Invalid system time, not syncing to RTC.");
        return;
    }

    RTC_t time {
        .rtc_sec     = now.second,
        .rtc_min     = now.minute,
        .rtc_hour    = now.hour,
        .rtc_day     = now.day_of_month,
        .rtc_month   = now.month,
        .rtc_year    = static_cast<uint8_t>(now.year - 2000),
        .rtc_weekday = to_custom_weekday(now.day_of_week)
    };


    ESP_LOGD(TAG, "RTC write time: %0u:%0u:%0u %0u-%0u-%0u Weekday: %s",
                  time.rtc_hour,
                  time.rtc_min,
                  time.rtc_sec,
                  time.rtc_year + 2000,
                  time.rtc_month,
                  time.rtc_day,
                  reg_val_to_weekday(time.rtc_weekday));

    this->parent_->set_rtc_time(time);
}


} // namespace powerhub
} // namespace esphome