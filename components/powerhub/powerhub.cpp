#include "powerhub.h"
#include "esphome/core/log.h"

namespace esphome {
namespace powerhub {

static const char *TAG = "powerhub";

// Register map for M5 PowerHub
enum Register : uint8_t {
    // power enable regs
    REG_LED_PWR_EN        = 0x00,
    REG_USB_PWR_EN        = 0x01,
    REG_GROVE_RED_PWR_EN  = 0x02,
    REG_GROVE_BLUE_PWR_EN = 0x03,
    REG_RS485_CAN_PWR_EN  = 0x04,
    REG_VAMETER_PWR_EN    = 0x05,
    REG_CHARGE_PWR_EN     = 0x06,
    // usb mode
    REG_USB_MODE               = 0x10,

    // RS485 / CAN power config
    REG_VOLT_LSB               = 0x20,
    REG_VOLT_MSB               = 0x21,
    REG_CURRENT_LIMIT          = 0x22,
    REG_PWR_OUTPUT_CTRL        = 0x23,
    REG_PWR_DIR                = 0x24,

    // VAMeter monitor
    REG_V_BAT_LSB              = 0x30,
    REG_V_BAT_MSB              = 0x31,
    REG_I_BAT_LSB              = 0x32,
    REG_I_BAT_MSB              = 0x33,
    REG_V_PWR_CAN_LSB          = 0x34,
    REG_V_PWR_CAN_MSB          = 0x35,
    REG_I_PWR_CAN_LSB          = 0x36,
    REG_I_PWR_CAN_MSB          = 0x37,
    REG_V_PWR_485_LSB          = 0x38,
    REG_V_PWR_485_MSB          = 0x39,
    REG_I_PWR_485_LSB          = 0x3A,
    REG_I_PWR_485_MSB          = 0x3B,
    REG_V_USB_LSB              = 0x3C,
    REG_V_USB_MSB              = 0x3D,
    REG_I_USB_LSB              = 0x3E,
    REG_I_USB_MSB              = 0x3F,
    REG_V_GROVE_RED_LSB        = 0x40,
    REG_V_GROVE_RED_MSB        = 0x41,
    REG_I_GROVE_RED_LSB        = 0x42,
    REG_I_GROVE_RED_MSB        = 0x43,
    REG_V_GROVE_BLUE_LSB       = 0x44,
    REG_V_GROVE_BLUE_MSB       = 0x45,
    REG_I_GROVE_BLUE_LSB       = 0x46,
    REG_I_GROVE_BLUE_MSB       = 0x47,

    // detect status
    REG_CHARGE_STATUS          = 0x50,
    REG_VIN_STATUS             = 0x51,

    // LED Color
    REG_USB_C_B                = 0x60,
    REG_USB_C_G                = 0x61,
    REG_USB_C_R                = 0x62,
    REG_USB_A_B                = 0x64,
    REG_USB_A_G                = 0x65,
    REG_USB_A_R                = 0x66,
    REG_GROVE_BLUE_B           = 0x68,
    REG_GROVE_BLUE_G           = 0x69,
    REG_GROVE_BLUE_R           = 0x6A,
    REG_485_CAN_B              = 0x6C,
    REG_485_CAN_G              = 0x6D,
    REG_485_CAN_R              = 0x6E,
    REG_GROVE_RED_B            = 0x70,
    REG_GROVE_RED_G            = 0x71,
    REG_GROVE_RED_R            = 0x72,
    REG_BAT_CHARGE_B           = 0x74,
    REG_BAT_CHARGE_G           = 0x75,
    REG_BAT_CHARGE_R           = 0x76,
    REG_PWR_L_B                = 0x78,
    REG_PWR_L_G                = 0x79,
    REG_PWR_L_R                = 0x7A,
    REG_PWR_R_B                = 0x7C,
    REG_PWR_R_G                = 0x7D,
    REG_PWR_R_R                = 0x7E,

    // LED brightness ( 0 - 100 )
    REG_LED_USB_C_BRIGHTNESS      = 0x80,
    REG_LED_USB_A_BRIGHTNESS      = 0x81,
    REG_LED_GROVE_BLUE_BRIGHTNESS = 0x82,
    REG_LED_485_CAN_BRIGHTNESS    = 0x83,
    REG_LED_GROVE_RED_BRIGHTNESS  = 0x84,
    REG_LED_BAT_CHARGE_BRIGHTNESS = 0x85,
    REG_LED_PWR_L_BRIGHTNESS      = 0x86,
    REG_LED_PWR_R_BRIGHTNESS      = 0x87,

    // SC8721 config (read only)
    REG_GET_SC8721_CFG = 0x90,
    REG_SC8721_CSO = 0x91,
    REG_SC8721_VOUT_SET_MSB  = 0x93,
    REG_SC8721_VOUT_SET_LSB = 0x94,
    REG_SC8721_GLOBAL_CTRL = 0x95,
    REG_SC8721_SYS_SET = 0x96,
    // 0x97 RESERVED
    REG_SC8721_FREQ_SET = 0x98,
    REG_SC8721_STATUS_1 = 0x99,
    REG_SC8721_STATUS_2 = 0x9A,

    // Button
    REG_BTN                    = 0xA0,

    // Wakeup source
    REG_NSTBY_WKUP             = 0xB0,
    REG_VIN_DET                = 0xB1,
    REG_VUSB_DET               = 0xB2,

    // RTC
    REG_RTC_SEC                = 0xC0,
    REG_RTC_MIN                = 0xC1,
    REG_RTC_HOUR               = 0xC2,
    REG_RTC_DD                 = 0xC3,
    REG_RTC_MM                 = 0xC4,
    REG_RTC_YY                 = 0xC5,
    REG_RTC_WEEK               = 0xC6,

    // RTC Alarm
    REG_ALARM_MIN              = 0xD0,
    REG_ALARM_HOUR             = 0xD1,
    REG_ALARM_DAY              = 0xD2,
    REG_ALARM_EN               = 0xD3,


    // System command
    REG_SYS_PWR_OFF = 0xE0,
    REG_SYS_RESET = 0xE1,
    REG_SYS_DOWNLOAD_MODE = 0xE2,

    // versions
    REG_BOOT_VER               = 0xFC,
    REG_FIRM_VER               = 0xFE,

    // I2C address
    REG_I2C_ADDR               = 0xFF,

};


const char* vin_status_to_string(uint8_t status) {
    switch (status) {
        case 0: return "No Input";
        case 1: return "Input present";
        default: return "Unknown";
    }
}


const char* charge_status_to_string(uint8_t status) {
    switch (status) {
        case 0: return "Discharging";
        case 1: return "Charging";
        default: return "Unknown";
    }
}



void PowerHub::setup() {
    ESP_LOGD(TAG, "Setting up PowerHub...");


    // read versions information
    RETURN_IF_ERROR(this->read_register(Register::REG_BOOT_VER, 
                                        &this->boot_version_, 
                                        1));
    RETURN_IF_ERROR(this->read_register(Register::REG_FIRM_VER, 
                                        &this->firmware_version_, 
                                        1));


    // software reset, default behavior of powerhub
    
    // enable Charge, LED, VAMeter channels, disable other channel
    uint8_t pwr_en[7] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01 }; 
    RETURN_IF_ERROR(this->write_register(Register::REG_LED_PWR_EN, 
                                         pwr_en, 
                                         sizeof(pwr_en)));

#ifdef USE_SWITCH
    this->charge_power_enabled_ = true;
    this->led_power_enabled_ = true;
    this->vameter_power_enabled_ = true;
#endif

    // USB Slave mode
    uint8_t mode = 0x00;
    RETURN_IF_ERROR(this->write_register(Register::REG_USB_MODE, &mode, 1)); 

#ifdef USE_SELECT
    // this->usb_mode_select_->publish_state("Default");
#endif
    /* 
     * RS485 & CAN Voltage, current limit and power direction
     * Default 3.0 V ( minimal voltage ) / 12.9 mA (13 mA, minimal current)
     * Power output disable
     * Power direction input
     * The voltage step is 20 mV
     * The current limit:
     * Iout limit(mA) = ( Current Limit Value + 1 ) * 8 / 0.62
     * Note: The total output power must not exceed 20W. 
     * If the user's output voltage is high, 
     * the output current register will be limited to a lower value.
    */
    uint8_t rs485_can_config[5] = { 0x0B, 0xB8, 0x00, 0x00, 0x00 };
    RETURN_IF_ERROR(this->write_register(Register::REG_VOLT_LSB, 
                                         rs485_can_config, 
                                         sizeof(rs485_can_config)));
#ifdef USE_NUMBER
    // this->rs485_can_output_voltage_number_->publish_state(3000.0f);
    // this->rs485_can_current_limit_number_->publish_state(13.0f);
#endif
    // enable all wakeup source
    // Note: At least one of the three wake sources must be enabled. 
    // If all three wake sources are disabled, hibernation will not be permitted.
    uint8_t source_cfg[3] = { 0x01, 0x01, 0x01 };
    RETURN_IF_ERROR(this->write_register(Register::REG_NSTBY_WKUP, 
                                         source_cfg, 
                                         sizeof(source_cfg)));

    /*
     *   read and store the RTC time (required by powerhub.read_time), RTC Alarm
     */
    WARN_IF(this->read_register(Register::REG_RTC_SEC, 
                                reinterpret_cast<uint8_t*>(&this->clock_), 
                                sizeof(this->clock_)));
    
    WARN_IF(this->read_register(Register::REG_ALARM_MIN,
                                reinterpret_cast<uint8_t*>(&this->rtc_alarm_), 
                                sizeof(this->rtc_alarm_)));
    // Disable the rtc alarm
    uint8_t alarm_en = 0;
    WARN_IF(this->write_register(Register::REG_ALARM_EN, &alarm_en, 1));



#ifdef USE_BINARY_SENSOR
    // Inteval used to read PMU button status
    // polling every 20ms
    this->button_binary_sensor_->publish_initial_state(false);
    this->set_interval("pmu_button", 20, [this]() {
        this->update_pmu_button_sensor();
    });
#endif

#ifdef USE_TEXT_SENSOR
    // Interval used to read charge/vin status
    // polling every 500 ms (0.5s)
    this->set_interval("charge_vin_status", 500, [this]() {
        this->update_charge_vin_sensor();
    });
#endif

    ESP_LOGD(TAG, "Setup powerhub finished...");
}


void PowerHub::update() {

#ifdef USE_SENSOR
   this->update_vameter_sensor();
#endif

#ifdef USE_TEXT_SENSOR
    if (this->firmware_ver_text_sensor_) {
        this->firmware_ver_text_sensor_->publish_state(std::to_string(this->get_firmware_ver()));
    }

    if(this->bootloader_ver_text_sensor_) {
        this->bootloader_ver_text_sensor_->publish_state(std::to_string(this->get_bootloader_ver()));
    }
#endif

}


void PowerHub::dump_config() {
    LOG_I2C_DEVICE(this);
    ESP_LOGCONFIG(TAG, 
                  "  Bootloader Version: %d \n"
                  "  Firmware Version: %d\n",
                  this->boot_version_, this->firmware_version_);
#ifdef USE_BINARY_SENSOR
    ESP_LOGCONFIG(TAG, "Binary Sensor:");
    LOG_BINARY_SENSOR("  ", "Button", this->button_binary_sensor_);
#endif

#ifdef USE_SENSOR
    ESP_LOGCONFIG(TAG, "Sensor:");
    LOG_SENSOR("  ", "Battery Level", this->battery_level_sensor_);
    LOG_SENSOR("  ", "Battery Voltage", this->battery_voltage_sensor_);
    LOG_SENSOR("  ", "Battery Current", this->battery_current_sensor_);
#endif

#ifdef USE_TEXT_SENSOR
    ESP_LOGCONFIG(TAG, "Text Sensor:");
    LOG_TEXT_SENSOR("  ", "Bootloader", this->bootloader_ver_text_sensor_);
    LOG_TEXT_SENSOR("  ", "Charge Status", this->charge_status_text_sensor_);
    LOG_TEXT_SENSOR("  ", "Firmware", this->firmware_ver_text_sensor_);
    LOG_TEXT_SENSOR("  ", "External Input Power", this->vin_status_text_sensor_);
#endif

#ifdef USE_SWITCH
    ESP_LOGCONFIG(TAG, "Switch:");
    LOG_SWITCH("  ", "LED Power", this->led_pwr_switch_);
    LOG_SWITCH("  ", "USB Power", this->usb_pwr_switch_);
    LOG_SWITCH("  ", "Port.A Power (Grove Red)", this->grove_red_pwr_switch_);
    LOG_SWITCH("  ", "Port.C Power (Grove Blue)", this->grove_blue_pwr_switch_);
    LOG_SWITCH("  ", "RS485 & CAN Power", this->rs485_can_pwr_switch_);
    LOG_SWITCH("  ", "RS485 & CAN Power Direction", this->rs485_can_direction_switch_);
    LOG_SWITCH("  ", "V/A Meter Power", this->vameter_pwr_switch_);
    LOG_SWITCH("  ", "Charge Power", this->charge_pwr_switch_);
#endif

#ifdef USE_SELECT
    ESP_LOGCONFIG(TAG, "Select:");
    LOG_SELECT("  ", "USB Mode", this->usb_mode_select_);
#endif

#ifdef USE_NUMBER
    ESP_LOGCONFIG(TAG, "Number:");
    LOG_NUMBER("  ", "RS485 & CAN Output Voltage", this->rs485_can_output_voltage_number_);
    LOG_NUMBER("  ", "RS485 & CAN Output Current Limit", this->rs485_can_current_limit_number_);
#endif

}

void PowerHub::set_led_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_LED_PWR_EN, &val, 1));
    this->led_power_enabled_ = en;
}

void PowerHub::set_usb_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_USB_PWR_EN, &val, 1));
    this->usb_power_enabled_ = en;
}

void PowerHub::set_grove_red_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_GROVE_RED_PWR_EN, &val, 1));
    this->grove_red_power_enabled_ = en;
}

void PowerHub::set_grove_blue_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_GROVE_BLUE_PWR_EN, &val, 1));
    this->grove_blue_power_enabled_ = en;
}

void PowerHub::set_rs485_can_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_RS485_CAN_PWR_EN, &val, 1));
    this->rs485_can_power_enabled_ = en;
}

void PowerHub::set_vameter_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_VAMETER_PWR_EN, &val, 1));
    this->vameter_power_enabled_ = en;
}

void PowerHub::set_charge_pwr_en(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_CHARGE_PWR_EN, &val, 1));
    this->charge_power_enabled_ = en;
}


void PowerHub::set_usb_mode(uint8_t mode) {
    WARN_IF(this->write_register(Register::REG_USB_MODE, &mode, 1));
    this->usb_mode_ = mode;
}

void PowerHub::set_rs485_can_voltage(uint16_t mv) {
    uint8_t step = 20;
    uint16_t min = 3000;
    uint16_t max = 20000;

    // align voltage to 20 mV steps relative to min (3000 mV), rounding to nearest step
    if (mv >= min) {
        uint16_t mod = (mv - min) % step;
        if (mod != 0) {
            if (mod >= step / 2) {
                mv += (step - mod);
            } else {
                mv -= mod;
            }
            ESP_LOGW(TAG, "Invalid voltage, rounding to nearest value %d", mv);
        }
    }
    if ( mv < min || mv > max ) {
        mv = std::clamp(mv, min, max);
        ESP_LOGW(TAG, "Invalid voltage, support range is 3000 - 20000, clamping to %d", mv);
    }
    
    WARN_IF(this->write_register(Register::REG_VOLT_LSB, reinterpret_cast<uint8_t *>(&mv), 2));
    this->rs485_can_voltage_mv_ = mv;
}

void PowerHub::set_rs485_can_current(uint16_t limit) {

    uint8_t val = static_cast<uint8_t>(limit * 0.62 / 8 - 1);

    // 0 - 232, corresponding limits 12 mA - 3000 mA
    if ( val > 232 ) {
        ESP_LOGW(TAG, "RS485 & CAN output current limit exceed the maximum value!");
        val = 232;
    }
    
    WARN_IF(this->write_register(Register::REG_CURRENT_LIMIT, &val, 1));
    this->rs485_can_current_limit_ = limit;
}

// Configure the SC8721 power direction.
// 0x01 output direction, 
// 0x00 input  direction
// if output direction was set, it will enable power output automatically.
void PowerHub::set_rs485_can_pwr_direction(bool direction) {
    uint8_t val = direction ? 0x01 : 0x00;
    WARN_IF(this->write_register(Register::REG_PWR_DIR, &val, 1));
    this->rs485_can_power_direction_ = direction;
}

// Configure the SC8721 power output.
// 0x01 enable output
// 0x00 disable output
void PowerHub::set_rs485_can_pwr_output(bool output) {
    uint8_t val = output ? 0x01 : 0x00;
    WARN_IF(this->write_register(Register::REG_PWR_DIR, &val, 1));
    this->rs485_can_power_output_ctrl_ = output;
}

float PowerHub::calc_battery_level() {

    const float offset = 100.0f;

    float voltage = this->battery_.voltage;
    int16_t current = this->battery_.current;

    // Maximum voltage is typically 8.4 V
    // normal case the battery usually stop charging
    // at around 8.3 V
    const float m_volt = 8400.0f - offset;
    // Nominal (rated) voltage is 7.4 V 
    const float n_volt = 7400.0f;
    // Battery termination voltage 6V
    // when near this value, battery is almost depleted
    const float t_volt = 6000.0f + offset;

    // usually if current is 0
    // meaning no battery is connected
    if ( current == 0 ) {
        return NAN;
    }
    
    // read voltage is 0, battery may not connected
    if (voltage < 1e-6f ) {
        return NAN;
    }

    // read voltage lower than 't_volt'
    if ( voltage < t_volt + 1e-6f && current != 0) {
        return 0;
    }

    // Apply min-max normalization
    float percent = (voltage - t_volt) / (m_volt - t_volt) * 100;

    return std::clamp(percent, 0.0f, 100.0f);
}

// Read and store once depend on the switch values
void PowerHub::read_power_channel() {
    Raw_Power_t power;

    // Read once
    WARN_IF(this->read_register(Register::REG_V_BAT_LSB, reinterpret_cast<uint8_t *>(&power), sizeof(Raw_Power_t)));

    if (!this->vameter_power_enabled_) {
        ESP_LOGW(TAG, "VAMeter power switch is not enabled, measurements are invalid!");
        return;
    }

    if (this->charge_power_enabled_) {
        this->battery_.voltage = power.bat_volt;
        this->battery_.current = power.bat_curr;
    }

    if (this->usb_power_enabled_) {
        this->usb_.voltage = power.usb_volt;
        this->usb_.current = power.usb_curr;
    }

    if (this->grove_red_power_enabled_) {
        this->grove_red_.voltage = power.grove_red_volt;
        this->grove_red_.current = power.grove_red_curr;
    }

    if (this->grove_blue_power_enabled_) {
        this->grove_blue_.voltage = power.grove_blue_volt;
        this->grove_blue_.current = power.grove_blue_curr;
    }

    if (this->rs485_can_power_enabled_) {
        this->can_.voltage   = power.can_volt;
        this->can_.current   = power.can_curr;
        this->rs485_.voltage = power.rs485_volt;
        this->rs485_.current = power.rs485_curr;
    }
}

void PowerHub::update_vameter_sensor() {

    this->read_power_channel();

    if (this->battery_level_sensor_) {
        this->battery_level_sensor_->publish_state(this->calc_battery_level());
    }

    if(this->battery_voltage_sensor_) {
        uint16_t mv = this->battery_.voltage;
        this->battery_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if(this->battery_current_sensor_) {
        int16_t ma = this->battery_.current;
        this->battery_current_sensor_->publish_state(static_cast<float>(ma));
    }

    if (this->can_voltage_sensor_) {
        uint16_t mv = this->can_.voltage;
        this->can_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if (this->can_current_sensor_) {
        int16_t ma = this->can_.current;
        this->can_current_sensor_->publish_state(static_cast<float>(ma));
    }

    if (this->rs485_voltage_sensor_) {
        uint16_t mv = this->rs485_.voltage;
        this->rs485_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if (this->rs485_current_sensor_) {
        int16_t ma = this->rs485_.current;
        this->rs485_current_sensor_->publish_state(static_cast<float>(ma));
    }

    if (this->usb_voltage_sensor_) {
        uint16_t mv = this->usb_.voltage;
        this->usb_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if (this->usb_current_sensor_) {
        int16_t ma = this->usb_.current;
        this->usb_current_sensor_->publish_state(static_cast<float>(ma));
    }

    if (this->grove_red_voltage_sensor_) {
        uint16_t mv = this->grove_red_.voltage;
        this->grove_red_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if (this->grove_red_current_sensor_) {
        int16_t ma = this->grove_red_.current;
        this->grove_red_current_sensor_->publish_state(static_cast<float>(ma));
    }

    if (this->grove_blue_voltage_sensor_) {
        uint16_t mv = this->grove_blue_.voltage;
        this->grove_blue_voltage_sensor_->publish_state(static_cast<float>(mv));
    }

    if (this->grove_blue_current_sensor_) {
        int16_t ma = this->grove_blue_.current;
        this->grove_blue_current_sensor_->publish_state(static_cast<float>(ma));
    }
}

uint8_t PowerHub::read_charge_status() {
    // not accurate
    // uint8_t val = 0;
    // WARN_IF(this->read_register(Register::REG_CHARGE_STATUS, &val, 1));
    // return val;

    // temporary solution is to read the current direction to determine the charging
    int16_t val = this->battery_.current;
    // if measured current is 0
    // indicating the battery may not connected
    // or battery is entering the protect state
    // thus no power
    if ( val == 0 ) {
        return 2;
    }
    // positive current suggest the battery is powering the device (discharging)
    if ( val > -10 ) return 0;
    // negative current suggest external input is charging the battery
    else if ( val < -10 ) return 1;
    else return 2;
}

uint8_t PowerHub::read_vin_status() {
    uint8_t val = 0;
    WARN_IF(this->read_register(Register::REG_VIN_STATUS, &val, 1));
    return val;
}

void PowerHub::update_charge_vin_sensor() {
#ifdef USE_TEXT_SENSOR
    if (this->charge_status_text_sensor_) {
        std::string status = charge_status_to_string(this->read_charge_status());
        this->charge_status_text_sensor_->publish_state(status);
    }

    if (this->vin_status_text_sensor_) {
        std::string status = vin_status_to_string(this->read_vin_status());
        this->vin_status_text_sensor_->publish_state(status);
    }
#endif 
}

void PowerHub::set_led_usb_a_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);         
    WARN_IF(this->write_register(Register::REG_USB_A_B, color, 3));
    this->led_usb_a_color_ = c;
}

void PowerHub::set_led_usb_c_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_USB_C_B, color, 3));
    this->led_usb_c_color_ = c;
}

void PowerHub::set_led_grove_blue_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_GROVE_BLUE_B, color, 3));
    this->led_grove_blue_color_ = c;
}

void PowerHub::set_led_grove_red_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_GROVE_RED_B, color, 3));
    this->led_grove_red_color_ = c;
}

void PowerHub::set_led_rs485_can_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_485_CAN_B, color, 3));
    this->led_rs485_can_color_ = c;
}

void PowerHub::set_led_bat_charge_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_BAT_CHARGE_B, color, 3));
    this->led_bat_charge_color_ = c;
}

void PowerHub::set_led_pwr_l_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_PWR_L_B, color, 3));
    this->led_pwr_l_color_ = c;
}

void PowerHub::set_led_pwr_r_color(const BGR_t &c) {
    const uint8_t *color = reinterpret_cast<const uint8_t*>(&c);
    WARN_IF(this->write_register(Register::REG_PWR_R_B, color, 3));
    this->led_pwr_r_color_ = c;
}


void PowerHub::set_brightness_usb_c(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_USB_C_BRIGHTNESS, &b, 1));
    this->brightness_usb_c_ = b;
}

void PowerHub::set_brightness_usb_a(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_USB_A_BRIGHTNESS, &b, 1));
    this->brightness_usb_a_ = b;
}

void PowerHub::set_brightness_grove_blue(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_GROVE_BLUE_BRIGHTNESS, &b, 1));
    this->brightness_grove_blue_ = b;
}

void PowerHub::set_brightness_rs485_can(uint8_t b)
{
    WARN_IF(this->write_register(Register::REG_LED_485_CAN_BRIGHTNESS, &b, 1));
    this->brightness_rs485_can_ = b;
}

void PowerHub::set_brightness_grove_red(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_GROVE_RED_BRIGHTNESS, &b, 1));
    this->brightness_grove_red_ = b;
}

void PowerHub::set_brightness_bat_charge(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_BAT_CHARGE_BRIGHTNESS, &b, 1));
    this->brightness_bat_charge_ = b;
}

void PowerHub::set_brightness_pwr_l(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_PWR_L_BRIGHTNESS, &b, 1));
    this->brightness_pwr_l_ = b;
}

void PowerHub::set_brightness_pwr_r(uint8_t b) {
    WARN_IF(this->write_register(Register::REG_LED_PWR_R_BRIGHTNESS, &b, 1));
    this->brightness_pwr_r_ = b;
}



// Consider adding invert options in YAML
// 0: press
// 1: no press
bool PowerHub::read_pmu_btn_status() {
    uint8_t val = 0;
    WARN_IF(this->read_register(Register::REG_BTN, &val, 1));
    return !static_cast<bool>(val);
}

void PowerHub::update_pmu_button_sensor() {
#ifdef USE_BINARY_SENSOR
    // publish only when value changes?
    if (this->button_binary_sensor_) {
        bool pressed = this->read_pmu_btn_status();
        if (pressed != this->last_button_state_) {
            this->last_button_state_ = pressed;
            this->button_binary_sensor_->publish_state(pressed);
        }
    }
#endif
}

void PowerHub::set_wake_nstby(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_NSTBY_WKUP, &val ,1));
    this->wake_nstby_ = static_cast<bool>(val);
}

void PowerHub::set_wake_vin_detect(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_VIN_DET, &val ,1));
    this->wake_vin_detect_ = static_cast<bool>(val);
}

void PowerHub::set_wake_vusb_detect(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_VUSB_DET, &val ,1));
    this->wake_vusb_detect_ = static_cast<bool>(val);
}

void PowerHub::set_rtc_time(const RTC_t &t) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(&t);
    size_t len = sizeof(RTC_t);
    WARN_IF(this->write_register(Register::REG_RTC_SEC, data , len));
    this->clock_ = t;
}

void PowerHub::set_rtc_alarm(const RTC_Alarm_t &t) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(&t);
    size_t len = sizeof(RTC_Alarm_t);
    WARN_IF(this->write_register(Register::REG_ALARM_MIN, data , len));
    this->rtc_alarm_ = t;
}

void PowerHub::set_alarm_enabled(bool en) {
    uint8_t val = en ? 1 : 0;
    WARN_IF(this->write_register(Register::REG_ALARM_EN, &val, 1));
    this->alarm_enabled_ = en;
}

void PowerHub::sys_poweroff() {
    uint8_t val = 1;
    WARN_IF(this->write_register(Register::REG_SYS_PWR_OFF, &val, 1));
}

void PowerHub::sys_reset() {
    uint8_t val = 1;
    WARN_IF(this->write_register(Register::REG_SYS_RESET, &val, 1));
}

void PowerHub::sys_download_mode() {
    uint8_t val = 1;
    WARN_IF(this->write_register(Register::REG_SYS_DOWNLOAD_MODE, &val, 1));
}



}    
} // namespace esphome
