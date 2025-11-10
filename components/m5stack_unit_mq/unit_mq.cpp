#include "unit_mq.h"
#include "esphome/core/log.h"


namespace esphome {
namespace unit_mq {

static const char *TAG = "unit_mq.sensor";

static const uint8_t MQ_STATUS_REG = 0x00;
static const uint8_t LED_STATUS_REG = 0x01;

static const uint8_t HIGH_LEVEL_TIME_REG = 0x10;
static const uint8_t LOW_LEVEL_TIME_REG = 0x11;

static const uint8_t MQ_ADC_8_REG = 0x20; // 8 bits mq adc

static const uint8_t MQ_ADC_12_REG = 0x30; // 12 bits mq adc (low)

static const uint8_t VALID_TAGS_REG = 0x40;

static const uint8_t NTC_ADC_8_REG = 0x50; // 8 bits ntc adc

static const uint8_t NTC_ADC_12_REG = 0x60; // 12 bits ntc adc (low)

static const uint8_t NTC_RESISTANCE_REG = 0x70; // NTC resistance (low)

static const uint8_t REF_VOLT_REG = 0x80; // reference voltage low byte
static const uint8_t MQ_VOLT_REG  = 0x82; // MQ voltage low byte
static const uint8_t NTC_VOLT_REG = 0x84; // NTC voltage low byte

static const uint8_t FIRM_VER_REG = 0xFE; // firmware version register

// NTC temperature
static const float RESISTANCE_REFERENCE  = 10000.0f;
static const float THERMISTOR_B_CONSTANT = 3950.0f;
static const float ABSOLUTE_ZERO_TEMP    = 273.15f;
static const float ABSOLUTE_TEMP_AT_25C  = 298.15f; // ABSOLUTE_ZERO_TEMP + 25.0f


const LogString *heat_mode_to_string(HeatMode mode) {
  switch (mode) {
    case HeatMode::OFF:
      return LOG_STR("OFF");
    case HeatMode::CONTINUOUS:
      return LOG_STR("CONTINUOUS");
    case HeatMode::SWITCH:
      return LOG_STR("SWITCH");
    default:
      return LOG_STR("OFF");;
  }
}

void UnitMQSensor::setup() {
    ESP_LOGD(TAG, "Setting up Unit MQ...");
    
    // Check firmware version
    if( !this->read_byte(FIRM_VER_REG, &this->firm_ver_) ) {
        ESP_LOGE(TAG, "Unable to retrieve firmware version, check your connections and I2C address settings.");
        this->mark_failed();
        return ;
    }


    // Set heat mode
    switch (this->mode_){
      case HeatMode::OFF:
        this->write_byte(MQ_STATUS_REG, 0x00);
        break;
      case HeatMode::CONTINUOUS:
        this->write_byte(MQ_STATUS_REG, 0x01);
      case HeatMode::SWITCH:
      default:
        this->write_byte(MQ_STATUS_REG, 0x10);
        break;
    }

    // Set high level time
    if ( this->high_level_time_ != 0 ) {
      this->write_byte(HIGH_LEVEL_TIME_REG, this->high_level_time_);
    }

    // Set low level time
    if ( this->low_level_time_ != 0 ) {
      this->write_byte(LOW_LEVEL_TIME_REG, this->low_level_time_);
    }
    
}

// Updata sensor values
void UnitMQSensor::update()
{
  // check valid tags
  // retrieve information.
  uint8_t valid_tag_val;
  this->read_byte(VALID_TAGS_REG, &valid_tag_val);
  this->valid_tags_ = static_cast<bool>(valid_tag_val);

  if (this->valid_tags_) {
    this->update_mq_adc_sensor();
    this->update_mq_voltage_sensor();
  } else {
    ESP_LOGW(TAG, "Sensor report invalid data. Skipping and waiting for the next polling.");
  }
  
  this->update_ntc_adc_sensor();
  this->update_ntc_resistance_sensor();
  this->update_temperature_sensor();
  this->update_reference_voltage_sensor();
  this->update_ntc_voltage_sensor();
  this->update_led_binary_sensor();
}

void UnitMQSensor::update_temperature_sensor()
{
  if (!this->temperature_sensor_)
    return;

  if (this->read_register(
          NTC_RESISTANCE_REG,
          reinterpret_cast<uint8_t *>(&this->ntc_resistance_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read NTC resistance for temperature");
    return;
  }

  float ntc_resistance_ohm = static_cast<float>(this->ntc_resistance_);

  float ratio = ntc_resistance_ohm / RESISTANCE_REFERENCE;
  float inv_T = (1.0f / ABSOLUTE_TEMP_AT_25C) +
                (logf(ratio) / THERMISTOR_B_CONSTANT);
  float temp_K = 1.0f / inv_T;
  float temperature = temp_K - ABSOLUTE_ZERO_TEMP;

  this->temperature_sensor_->publish_state(temperature);
}

void UnitMQSensor::update_mq_adc_sensor()
{
  if (!this->mq_adc_sensor_)
    return;

  if (this->read_register(
          MQ_ADC_12_REG,
          reinterpret_cast<uint8_t *>(&this->mq_adc_val_12_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read MQ ADC");
    return;
  }

  this->mq_adc_sensor_->publish_state(static_cast<float>(this->mq_adc_val_12_));
}

void UnitMQSensor::update_ntc_adc_sensor()
{
  if (!this->ntc_adc_sensor_)
    return;

  if (this->read_register(
          NTC_ADC_12_REG,
          reinterpret_cast<uint8_t *>(&this->ntc_adc_val_12_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read NTC ADC");
    return;
  }

  this->ntc_adc_sensor_->publish_state(static_cast<float>(this->ntc_adc_val_12_));
}

void UnitMQSensor::update_reference_voltage_sensor()
{
  if (!this->reference_voltage_sensor_)
    return;

  if (this->read_register(
          REF_VOLT_REG,
          reinterpret_cast<uint8_t *>(&this->ref_voltage_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read reference voltage");
    return;
  }  

  this->reference_voltage_sensor_->publish_state(static_cast<float>(this->ref_voltage_));
}

void UnitMQSensor::update_mq_voltage_sensor()
{
  if (!this->mq_voltage_sensor_)
    return;

  if (this->read_register(
          MQ_VOLT_REG,
          reinterpret_cast<uint8_t *>(&this->mq_voltage_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read MQ voltage");
    return;
  }  

  this->mq_voltage_sensor_->publish_state(static_cast<float>(this->mq_voltage_));
}

void UnitMQSensor::update_ntc_voltage_sensor()
{
  if (!this->ntc_voltage_sensor_)
    return;


  if (this->read_register(
          NTC_VOLT_REG,
          reinterpret_cast<uint8_t *>(&this->ntc_voltage_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read NTC voltage");
    return;
  }  

  this->ntc_voltage_sensor_->publish_state(static_cast<float>(this->ntc_voltage_));
}

void UnitMQSensor::update_led_binary_sensor()
{
  if (!this->led_binary_sensor_)
    return;

    if ( this->valid_tags_ ) {
      this->write_byte(LED_STATUS_REG, 0x01); // Turn on the LED
    } else {
      this->write_byte(LED_STATUS_REG, 0x00); // Turn off the LED
    }
  
  this->led_binary_sensor_->publish_state(this->valid_tags_);
}

void UnitMQSensor::update_ntc_resistance_sensor() {
  if (!this->ntc_resistance_sensor_)
    return;

  if (this->read_register(
          NTC_RESISTANCE_REG,
          reinterpret_cast<uint8_t *>(&this->ntc_resistance_),
          2) != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "Failed to read NTC resistance");
    return;
  }  

  this->ntc_resistance_sensor_->publish_state(static_cast<float>(this->ntc_resistance_));
}

// Dump config
void UnitMQSensor::dump_config() {
    LOG_I2C_DEVICE(this);

    if (this->is_failed()) {
        ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
    }

    ESP_LOGCONFIG(
        TAG,
        "Unit MQ:\n"
        "  Firmware version: %d\n"
        "  Heat mode: %s\n"
        "  High level time: %d\n"
        "  Low level time: %d",
        this->firm_ver_,
        heat_mode_to_string(this->mode_),
        this->high_level_time_,
        this->low_level_time_
    );
}


} // namespace unit_mq

} // namespace esphome