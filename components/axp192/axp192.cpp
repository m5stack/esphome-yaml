#include "axp192.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace axp192 {

static const char *TAG = "axp192";

void AXP192::AXP192::setup()
{
    ESP_LOGCONFIG(TAG, "Setting Up AXP192 PMU...");

    // Init, check the chip ID
    // If no valid ID (0x03) was returned, call it failed. //was 0x4A
    if (!this->initImpl()) {
        ESP_LOGE(TAG, "Failed to read Chip ID, Exiting...");
        this->mark_failed();
        return;
    }


    ESP_LOGCONFIG(TAG, "Chip ID:0x%x", this->getChipID());
    delay(10);
    //DC1 is MCU supply
    // Disable unused channels
    this->disableDC2(); //Unused?
    this->enableDC3(); //LCD Backlight so need on

//    this->enableDLDO1(); //LDO1 always on anyway, remove
    this->enableDLDO2(); //Periferal VDD & display power
    this->disableDLDO3(); //Vibration Motor VDD

    this->clearIrqStatus();

    this->configureAdcMeasure(); //Enable all monitoring except external TS pin and GPIO ADC

    // It is necessary to disable the detection function of the TS pin on the board
    // without the battery temperature detection function, otherwise it will cause abnormal charging
    this->disableTSPinMeasure();

    // Disable all interrupts
    this->disableIRQ(XPOWERS_AXP192_ALL_IRQ);
    // Clear all interrupt flags
    this->clearIrqStatus();
    // Enable the required interrupt function

    // All these need updating as interrrupts have all changed
    // this->enableIRQ(
    //     XPOWERS_AXP192_BAT_INSERT_IRQ    | XPOWERS_AXP192_BAT_REMOVE_IRQ      |   //BATTERY
    //     XPOWERS_AXP192_VBUS_INSERT_IRQ   | XPOWERS_AXP192_VBUS_REMOVE_IRQ     |   //VBUS
    //     XPOWERS_AXP192_PKEY_SHORT_IRQ    | XPOWERS_AXP192_PKEY_LONG_IRQ       |   //POWER KEY
    //     XPOWERS_AXP192_BAT_CHG_DONE_IRQ  | XPOWERS_AXP192_BAT_CHG_START_IRQ       //CHARGE
    //     // XPOWERS_AXP192_PKEY_NEGATIVE_IRQ | XPOWERS_AXP192_PKEY_POSITIVE_IRQ   |   //POWER KEY
    // );


    /*
      The default setting is CHGLED (AXP192 GPIO1) is automatically controlled by the this->
    - XPOWERS_CHG_LED_OFF,
    - XPOWERS_CHG_LED_BLINK_1HZ,
    - XPOWERS_CHG_LED_BLINK_4HZ,
    - XPOWERS_CHG_LED_ON,
    - XPOWERS_CHG_LED_CTRL_CHG,
    * */

    // this->setChargingLedMode(XPOWERS_CHG_LED_OFF); //Not connected in Core2 so remove

    // Set the precharge charging current
    // this->setPrechargeCurr(XPOWERS_AXP192_PRECHARGE_50MA); //NA in AXP192 so remove
    // Set constant current charge current limit
    this->setChargerConstantCurr(XPOWERS_AXP192_CHG_CUR_200MA);
    // Set stop charging termination current
    this->setChargerTerminationCurr(XPOWERS_AXP192_CHG_ITERM_25MA);

    // Set charge cut-off voltage
    this->setChargeTargetVoltage(XPOWERS_AXP192_CHG_VOL_4V1);

    // Set the watchdog trigger event type
    // this->setWatchdogConfig(XPOWERS_AXP192_WDT_IRQ_TO_PIN);
    // Set watchdog timeout
    // this->setWatchdogTimeout(XPOWERS_AXP192_WDT_TIMEOUT_4S); //NA in AXP192 so remove
    // Enable watchdog to trigger interrupt event
    // this->enableWatchdog(); //NA in AXP192 so remove

    // Set the time of pressing the button to turn off
    this->setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);
    uint8_t opt = this->getPowerKeyPressOffTime();
    switch (opt) {
    case XPOWERS_POWEROFF_4S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOffTime: 4 Second");
        break;
    case XPOWERS_POWEROFF_6S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOffTime: 6 Second");
        break;
    case XPOWERS_POWEROFF_8S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOffTime: 8 Second");
        break;
    case XPOWERS_POWEROFF_10S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOffTime: 10 Second");
        break;
    default:
        break;
    }
    // Set the button power-on press time
    this->setPowerKeyPressOnTime(XPOWERS_POWERON_1S);
    opt = this->getPowerKeyPressOnTime();
    switch (opt) {
    case XPOWERS_POWERON_128MS:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOnTime: 128 Ms");
        break;
    case XPOWERS_POWERON_512MS:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOnTime: 512 Ms");
        break;
    case XPOWERS_POWERON_1S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOnTime: 1 Second");
        break;
    case XPOWERS_POWERON_2S:
        ESP_LOGCONFIG(TAG, "PowerKeyPressOnTime: 2 Second");
        break;
    default:
        break;
    }

    // Enable internal ADC detection
    this->enableBattDetection();

    ESP_LOGI(TAG, "AXP192 PMU Init Success!");
}



void AXP192::AXP192::loop(){ }


void AXP192::AXP192::dump_config()
{
    ESP_LOGCONFIG(TAG, "AXP192:");
    ESP_LOGCONFIG(TAG, "DC1  : %s   Voltage:%u mV",  this->isEnableDC1()  ? "+" : "-", this->getDC1Voltage());
    ESP_LOGCONFIG(TAG, "DC2  : %s   Voltage:%u mV",  this->isEnableDC2()  ? "+" : "-", this->getDC2Voltage());
    ESP_LOGCONFIG(TAG, "DC3  : %s   Voltage:%u mV",  this->isEnableDC3()  ? "+" : "-", this->getDC3Voltage());
    ESP_LOGCONFIG(TAG, "ALDO1: %s   Voltage:%u mV",  this->isEnableALDO1()  ? "+" : "-", this->getALDO1Voltage());
    ESP_LOGCONFIG(TAG, "ALDO2: %s   Voltage:%u mV",  this->isEnableALDO2()  ? "+" : "-", this->getALDO2Voltage());
    ESP_LOGCONFIG(TAG, "ALDO3: %s   Voltage:%u mV",  this->isEnableALDO3()  ? "+" : "-", this->getALDO3Voltage());
    ESP_LOGCONFIG(TAG, "ALDO4: %s   Voltage:%u mV",  this->isEnableALDO4()  ? "+" : "-", this->getALDO4Voltage());
    ESP_LOGCONFIG(TAG, "DLDO3: %s   Voltage:%u mV",  this->isEnableDLDO3()  ? "+" : "-", this->getDLDO3Voltage());
    ESP_LOGCONFIG(TAG, "DLDO2: %s   Voltage:%u mV",  this->isEnableDLDO2()  ? "+" : "-", this->getDLDO2Voltage());

}


/*
*  Convenient functions to operate with registers, implement with internal ESPHome method.
*/

int AXP192::readRegister(uint8_t reg)
{
    uint8_t val;
    bool ret = this->read_byte(reg, &val);

    if (!ret) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", reg);
        return -1;
    }
    return static_cast<int>(val);
}

int AXP192::writeRegister(uint8_t reg, uint8_t val)
{
    bool ret = this->write_byte(reg, val);
    if ( !ret ) {
         ESP_LOGE(TAG, "I2C write error at reg 0x%02X", reg);
        return -1;
    }
    return ret ? 0 : -1;
}

inline bool AXP192::clrRegisterBit(uint8_t registers, uint8_t bit)
{
    uint8_t val;
    bool ret = this->read_byte(registers, &val);

    if (!ret) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", registers);
        ESP_LOGE(TAG, "Error when clear the register bit %d", bit);
        return false;
    }
    // Set bit to 0
    val = val & (~_BV(bit));
    return this -> write_byte(registers, val);
}

inline bool AXP192::setRegisterBit(uint8_t registers, uint8_t bit)
{
    uint8_t val;
    bool ret = this->read_byte(registers, &val);

    if (!ret) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", registers);
        ESP_LOGE(TAG, "Error when set the register bit %d", bit);
        return false;
    }
    // Set bit to 0
    val = val | (_BV(bit));
    return this -> write_byte(registers, val);
}

inline bool AXP192::getRegisterBit(uint8_t registers, uint8_t bit)
{
    uint8_t val;
    bool ret = this->read_byte(registers, &val);
    if(!ret) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", registers);
        return false;
    }
    return val & _BV(bit);
}

//new function for ADC reads
inline uint32_t AXP192::readRegisterH8M8L8(uint8_t highReg, uint8_t medReg, uint8_t lowReg)
{
    uint8_t h8, m8, l8;
    bool ret1 = this->read_byte(highReg, &h8);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", highReg);
        return 0;
    }
    bool ret2 = this->read_byte(medReg, &m8);
    if (!ret2) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", medReg);
        return 0;
    }
    bool ret3 = this->read_byte(lowReg, &l8);
    if (!ret3) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", lowReg);
        return 0;
    }
    return (h8 << 16) | (m8 << 8) | (l8);
}

inline uint16_t AXP192::readRegisterH8L4(uint8_t highReg, uint8_t lowReg)
{
    uint8_t h8, l4;
    bool ret1 = this->read_byte(highReg, &h8);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", highReg);
        return 0;
    }
    bool ret2 = this->read_byte(lowReg, &l4);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", lowReg);
        return 0;
    }
    return (h8 << 4) | (l4 & 0x0F);
}

inline uint16_t AXP192::readRegisterH6L8(uint8_t highReg, uint8_t lowReg)
{
    uint8_t h6, l8;
    bool ret1 = this->read_byte(highReg, &h6);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", highReg);
        return 0;
    }
    bool ret2 = this->read_byte(lowReg, &l8);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", lowReg);
        return 0;
    }
    return ((h6 & 0x3F) << 8) | l8;
}

inline uint16_t AXP192::readRegisterH5L8(uint8_t highReg, uint8_t lowReg)
{
    uint8_t h5, l8;
    bool ret1 = this->read_byte(highReg, &h5);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", highReg);
        return 0;
    }
    bool ret2 = this->read_byte(lowReg, &l8);
    if (!ret1) {
        ESP_LOGE(TAG, "I2C write error at reg 0x%02X", lowReg);
        return 0;
    }
    return ((h5 & 0x1F) << 8) | l8;
}

// GPIO functions
void AXP192::setGpioPin(uint8_t gpio, uint8_t pin_state){
    //Assumes GPIO with external pullups, active low
    uint8_t val=0;

    switch (gpio) {
    case 0:
        val = readRegister(XPOWERS_AXP192_GPIO0_MODE);
        if (val == -1) return;
        val &= 0xF8;
        writeRegister(XPOWERS_AXP192_GPIO0_MODE, val | ((pin_state == 1) ? 0x06 : 0x05)); //Floating Output or Low output
        break;
    case 1:
        val = readRegister(XPOWERS_AXP192_GPIO1_MODE);
        if (val == -1) return;
        val &= 0xF8;
        writeRegister(XPOWERS_AXP192_GPIO1_MODE, val | ((pin_state == 1) ? 0x05 : 0x06)); //Low output (on) or Floating output (off)
        break;
    case 2:
        val = readRegister(XPOWERS_AXP192_GPIO2_MODE);
        if (val == -1) return;
        val &= 0xF8;
        writeRegister(XPOWERS_AXP192_GPIO2_MODE, val | ((pin_state == 1) ? 0x06 : 0x05)); //Floating Output or Low output
        break;
    case 3:
        val = readRegister(XPOWERS_AXP192_GPIO3_MODE);
        if (val == -1) return;
        val &= 0xF8;
        writeRegister(XPOWERS_AXP192_GPIO3_MODE, val | (0x06)); //Floating Output
        break;
    case 4:
        val = readRegister(XPOWERS_AXP192_GPIO4_MODE);
        if (val == -1) return;
        val &= 0xF8;
        writeRegister(XPOWERS_AXP192_GPIO4_MODE, val | (0x06)); //Floating Output
        break;
    default:
        break;
    }

}


/*
 * PMU status functions
 */
uint16_t AXP192::status()
{
    uint16_t status1 = readRegister(XPOWERS_AXP192_STATUS1) & 0x1F;
    uint16_t status2 = readRegister(XPOWERS_AXP192_STATUS2) & 0x1F;;
    return (status1 << 8) | (status2);
}


// getBatPresentState
bool AXP192::isACINPresent(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 7) == 1; } //new
bool AXP192::isACINValid(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 6) == 1; } //new
bool AXP192::isVbusIn(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 5) == 1; } //updated
bool AXP192::isVbusGood(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 4) == 1; } //was 5
//bit 3 Vbus above Vhold before insert?
bool AXP192::isDischarge(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 2) == 0x00; } //updated
//bit 1 ACIN and VBus pin shorted on PCB
//bit 0 Trigger boot from ACIN/VBus or not

bool AXP192::getThermalRegulationStatus(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 7); } //was 1
bool AXP192::isCharging(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 6) == 1; } //updated 1=charging, 0=not charging or finished
bool AXP192::isBatteryConnect(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 5) == 1; } //was 3
//bit 4 reserved
bool AXP192::isBatInActiveModeState(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 3) ==1; } // was 2
bool AXP192::isChargingCurrentLessThanExpected(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 2) == 1; } //new
//bit 1 reserved
//bit 0 reserved


// //Remove these
// bool AXP192::getCurrentLimitStatus(void){ return getRegisterBit(XPOWERS_AXP192_STATUS1, 0); } //NA, remove?

// bool AXP192::isStandby(void){ return (readRegister(XPOWERS_AXP192_STATUS2) >> 5) == 0x00; } //NA?

// bool AXP192::isPowerOn(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 4); }  //NA?

// bool AXP192::isPowerOff(void){ return getRegisterBit(XPOWERS_AXP192_STATUS2, 4); } //NA?


// xpowers_chg_status_t AXP192::getChargerStatus(void){ //NA remove
//     int val = readRegister(XPOWERS_AXP192_STATUS2);
//     if (val == -1)return XPOWERS_AXP192_CHG_STOP_STATE;
//     val &= 0x07;
//     return (xpowers_chg_status_t)val;
// }

/*
 * PMU common configuration
 */

/**
 * @brief   Internal off-discharge enable for DCDC & LDO & SWITCH
 */

void AXP192::enableInternalDischarge(void){ setRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 5); }

void AXP192::disableInternalDischarge(void){ clrRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 5); }


/**
 * @brief   PWROK PIN pull low to Restart
 */
void AXP192::enablePwrOkPinPullLow(void){ setRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 3); }

void AXP192::disablePwrOkPinPullLow(void){ clrRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 3); }

void AXP192::enablePwronShutPMIC(void){ setRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 2); }

void AXP192::disablePwronShutPMIC(void){ clrRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 2); }


/**
 * @brief  Restart the SoC System, POWOFF/POWON and reset the related registers
 * @retval None
 */
void AXP192::reset(void){ setRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 1); }

/**
 * @brief  Set shutdown, calling shutdown will turn off all power channels,
 *         only VRTC belongs to normal power supply
 * @retval None
 */
void AXP192::shutdown(void){ setRegisterBit(XPOWERS_AXP192_COMMON_CONFIG, 0); }

// /**
//  * @param  opt: 0:115 , 1:125 , 2:135
//  */
// void AXP192::setDieOverTempLevel1(uint8_t opt)
// {
//     int val = readRegister(XPOWERS_AXP192_DIE_TEMP_CTRL);
//     if (val == -1)return;
//     val &= 0xF9;
//     writeRegister(XPOWERS_AXP192_DIE_TEMP_CTRL, val | (opt << 1));
// }

// uint8_t AXP192::getDieOverTempLevel1(void){ return (readRegister(XPOWERS_AXP192_DIE_TEMP_CTRL) & 0x06); }

// void AXP192::enableDieOverTempDetect(void){ setRegisterBit(XPOWERS_AXP192_DIE_TEMP_CTRL, 0); }

// void AXP192::disableDieOverTempDetect(void){ clrRegisterBit(XPOWERS_AXP192_DIE_TEMP_CTRL, 0); }

// // Linear Charger Vsys voltage dpm
// void AXP192::setLinearChargerVsysDpm(xpower_chg_dpm_t opt)
// {
//     int val = readRegister(XPOWERS_AXP192_MIN_SYS_VOL_CTRL);
//     if (val == -1)return;
//     val &= 0x8F;
//     writeRegister(XPOWERS_AXP192_MIN_SYS_VOL_CTRL, val | (opt << 4));
// }

// uint8_t AXP192::getLinearChargerVsysDpm(void)
// {
//     int val = readRegister(XPOWERS_AXP192_MIN_SYS_VOL_CTRL);
//     if (val == -1)return 0;
//     val &= 0x70;
//     return (val & 0x70) >> 4;
// }


/**
 * @brief  Set VBUS Voltage Input Limit.
 * @param  opt: View the related chip type xpowers_AXP192_vbus_vol_limit_t enumeration
 *              parameters in "axp192_cost.h"
 */
void AXP192::setVbusVoltageLimit(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_INPUT_VOL_LIMIT_CTRL);
    if (val == -1)return;
    val &= 0xF0;
    writeRegister(XPOWERS_AXP192_INPUT_VOL_LIMIT_CTRL, val | (opt & 0x0F));
}

/**
* @brief  Get VBUS Voltage Input Limit.
* @retval View the related chip type xpowers_axp192_vbus_vol_limit_t enumeration
*              parameters in "axp192_cost.h"
*/
uint8_t AXP192::getVbusVoltageLimit(void){ return ( readRegister(XPOWERS_AXP192_INPUT_VOL_LIMIT_CTRL) & 0x0F); }

/**
* @brief  Set VBUS Current Input Limit.
* @param  opt: View the related chip type xpowers_axp192_vbus_cur_limit_t enumeration
*              parameters in "axp192_cost.h"
* @retval true valid false invalid
*/
bool AXP192::setVbusCurrentLimit(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_INPUT_CUR_LIMIT_CTRL);
    if (val == -1)return false;
    val &= 0xF8;
    return 0 == writeRegister(XPOWERS_AXP192_INPUT_CUR_LIMIT_CTRL, val | (opt & 0x07));
}

/**
* @brief  Get VBUS Current Input Limit.
* @retval View the related chip type xpowers_axp192_vbus_cur_limit_t enumeration
*              parameters in "axp192_cost.h"
*/
uint8_t AXP192::getVbusCurrentLimit(void){ return (readRegister(XPOWERS_AXP192_INPUT_CUR_LIMIT_CTRL) & 0x07); }

/**
 * @brief  Reset the fuel gauge
 */
void AXP192::resetGauge(void){ setRegisterBit(XPOWERS_AXP192_RESET_FUEL_GAUGE, 3); }

/**
 * @brief   reset the gauge besides reset
 */
void AXP192::resetGaugeBesides(void){ setRegisterBit(XPOWERS_AXP192_RESET_FUEL_GAUGE, 2); }

/**
 * @brief Gauge Module
 */
void AXP192::enableGauge(void){ setRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 3); }

void AXP192::disableGauge(void){ clrRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 3); }

/**
 * @brief  Button Battery charge
 */
bool AXP192::enableButtonBatteryCharge(void){ return setRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 2); } //NA remove

bool AXP192::disableButtonBatteryCharge(void){ return clrRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 2); } //NA remove

bool AXP192::isEnableButtonBatteryCharge(){ return getRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 2); } //NA remove


//Button battery charge termination voltage setting
bool AXP192::setButtonBatteryChargeVoltage(uint16_t millivolt) //NA remove
{
    if (millivolt % XPOWERS_AXP192_BTN_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! Button battery charging step voltage is %u mV", XPOWERS_AXP192_BTN_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_BTN_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! The minimum charge termination voltage of the coin cell battery is %u mV", XPOWERS_AXP192_BTN_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_BTN_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! The minimum charge termination voltage of the coin cell battery is %u mV", XPOWERS_AXP192_BTN_VOL_MAX);
        return false;
    }
    int val =  readRegister(XPOWERS_AXP192_BTN_BAT_CHG_VOL_SET);
    if (val == -1)return 0;
    val  &= 0xF8;
    val |= (millivolt - XPOWERS_AXP192_BTN_VOL_MIN) / XPOWERS_AXP192_BTN_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP192_BTN_BAT_CHG_VOL_SET, val);
}

uint16_t AXP192::getButtonBatteryVoltage(void){
    int val =  readRegister(XPOWERS_AXP192_BTN_BAT_CHG_VOL_SET);
    if (val == -1)return 0;
    return (val & 0x07) * XPOWERS_AXP192_BTN_VOL_STEPS + XPOWERS_AXP192_BTN_VOL_MIN;
}


// /**
//  * @brief Cell Battery charge
//  */
// void AXP192::enableCellbatteryCharge(void){ setRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 1); }

// void AXP192::disableCellbatteryCharge(void){ clrRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 1); }

// /**
//  * @brief  Watchdog Module
//  */
// void AXP192::enableWatchdog(void){
//     setRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 0);
//     enableIRQ(XPOWERS_AXP192_WDT_EXPIRE_IRQ);
// }

// void AXP192::disableWatchdog(void)
// {
//     disableIRQ(XPOWERS_AXP192_WDT_EXPIRE_IRQ);
//     clrRegisterBit(XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL, 0);
// }

// /**
//  * @brief Watchdog Config
//  * @note
//  * @param  opt: 0: IRQ Only 1: IRQ and System reset  2: IRQ, System Reset and Pull down PWROK 1s  3: IRQ, System Reset, DCDC/LDO PWROFF & PWRON
//  * @retval None
//  */
// void AXP192::setWatchdogConfig(xpowers_wdt_config_t opt)
// {
//     int val = readRegister(XPOWERS_AXP192_WDT_CTRL);
//     if (val == -1)return;
//     val &= 0xCF;
//     writeRegister(XPOWERS_AXP192_WDT_CTRL, val | (opt << 4));
// }

// uint8_t AXP192::getWatchConfig(void)
// {
//     return (readRegister(XPOWERS_AXP192_WDT_CTRL) & 0x30) >> 4;
// }

// void AXP192::clrWatchdog(void)
// {
//     setRegisterBit(XPOWERS_AXP192_WDT_CTRL, 3);
// }


// void AXP192::setWatchdogTimeout(xpowers_wdt_timeout_t opt)
// {
//     int val = readRegister(XPOWERS_AXP192_WDT_CTRL);
//     if (val == -1)return;
//     val &= 0xF8;
//     writeRegister(XPOWERS_AXP192_WDT_CTRL, val | opt);
// }

// uint8_t AXP192::getWatchdogTimerout(void){ return readRegister(XPOWERS_AXP192_WDT_CTRL) & 0x07; }

// /**
//  * @brief  Low battery warning threshold 5-20%, 1% per step
//  * @param  percentage:   5 ~ 20
//  * @retval None
//  */
// void AXP192::setLowBatWarnThreshold(uint8_t percentage)
// {
//     if (percentage < 5 || percentage > 20)return;
//     int val = readRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET);
//     if (val == -1)return;
//     val &= 0x0F;
//     writeRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET, val | ((percentage - 5) << 4));
// }

// uint8_t AXP192::getLowBatWarnThreshold(void)
// {
//     int val = readRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET);
//     if (val == -1)return 0;
//     val &= 0xF0;
//     val >>= 4;
//     return val;
// }

// /**
//  * @brief  Low battery shutdown threshold 0-15%, 1% per step
//  * @param  opt:   0 ~ 15
//  * @retval None
//  */
// void AXP192::setLowBatShutdownThreshold(uint8_t opt)
// {
//     if (opt > 15) {
//         opt = 15;
//     }
//     int val = readRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET);
//     if (val == -1)return;
//     val &= 0xF0;
//     writeRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET, val | opt);
// }

// uint8_t AXP192::getLowBatShutdownThreshold(void)
// {
//     return (readRegister(XPOWERS_AXP192_LOW_BAT_WARN_SET) & 0x0F);
// }

//!  PWRON statu  20
// POWERON always high when EN Mode as POWERON Source
bool AXP192::isPoweronAlwaysHighSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 5); }

// Battery Insert and Good as POWERON Source
bool AXP192::isBattInsertOnSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 4); }

// Battery Voltage > 3.3V when Charged as Source
bool AXP192::isBattNormalOnSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 3); }

// Vbus Insert and Good as POWERON Source
bool AXP192::isVbusInsertOnSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 2); }

// IRQ PIN Pull-down as POWERON Source
bool AXP192::isIrqLowOnSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 1); }

// POWERON low for on level when POWERON Mode as POWERON Source
bool AXP192::isPwronLowOnSource(){ return getRegisterBit(XPOWERS_AXP192_PWRON_STATUS, 0); }

xpower_power_on_source_t AXP192::getPowerOnSource()
{
    int val = readRegister(XPOWERS_AXP192_PWRON_STATUS);
    if (val == -1) return XPOWER_POWERON_SRC_UNKONW;
    return (xpower_power_on_source_t)val;
}

//!  PWROFF status  21
// Die Over Temperature as POWEROFF Source
bool AXP192::isOverTemperatureOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 7); }

// DCDC Over Voltage as POWEROFF Source
bool AXP192::isDcOverVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 6); }

// DCDC Under Voltage as POWEROFF Source
bool AXP192::isDcUnderVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 5); }

// VBUS Over Voltage as POWEROFF Source
bool AXP192::isVbusOverVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 4); }

// Vsys Under Voltage as POWEROFF Source
bool AXP192::isVsysUnderVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 3); }

// POWERON always low when EN Mode as POWEROFF Source
bool AXP192::isPwronAlwaysLowOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 2); }

// Software configuration as POWEROFF Source
bool AXP192::isSwConfigOffSource(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 1); }

// POWERON Pull down for off level when POWERON Mode as POWEROFF Source
bool AXP192::isPwrSourcePullDown(){ return getRegisterBit(XPOWERS_AXP192_PWROFF_STATUS, 0); }

xpower_power_off_source_t AXP192::getPowerOffSource(){ 
    int val = readRegister(XPOWERS_AXP192_PWROFF_STATUS);
    if (val == -1) return XPOWER_POWEROFF_SRC_UNKONW;
    return (xpower_power_off_source_t)val;
}

//!REG 22H
void AXP192::enableOverTemperatureLevel2PowerOff(){ setRegisterBit(XPOWERS_AXP192_PWROFF_EN, 2); }

void AXP192::disableOverTemperaturePowerOff(){ clrRegisterBit(XPOWERS_AXP192_PWROFF_EN, 2); }

// CHANGE:  void AXP192::enablePwrOnOverVolOffLevelPowerOff()
void AXP192::enableLongPressShutdown(){ setRegisterBit(XPOWERS_AXP192_PWROFF_EN, 1); }

// CHANGE:  void AXP192::disablePwrOnOverVolOffLevelPowerOff()
void AXP192::disableLongPressShutdown(){ clrRegisterBit(XPOWERS_AXP192_PWROFF_EN, 1); }

//CHANGE: void AXP192::enablePwrOffSelectFunction()
void AXP192::setLongPressRestart(){ setRegisterBit(XPOWERS_AXP192_PWROFF_EN, 0); }

//CHANGE: void AXP192::disablePwrOffSelectFunction()
void AXP192::setLongPressPowerOFF(){ clrRegisterBit(XPOWERS_AXP192_PWROFF_EN, 0); }

//!REG 23H
// DCDC 120%(130%) high voltage turn off PMIC function
void AXP192::enableDCHighVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 5); }

void AXP192::disableDCHighVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 5); }

// DCDC3 85% low voltage turn Off PMIC function
void AXP192::enableDC3LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 2); }

void AXP192::disableDC3LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 2);}

// DCDC2 85% low voltage turn Off PMIC function
void AXP192::enableDC2LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 1);}

void AXP192::disableDC2LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 1);}

// DCDC1 85% low voltage turn Off PMIC function
void AXP192::enableDC1LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 0); }

void AXP192::disableDC1LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 0);}


// Set the minimum system operating voltage inside the PMU,
// below this value will shut down the PMU,Adjustment range 2600mV~3300mV
bool AXP192::setSysPowerDownVoltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_VSYS_VOL_THRESHOLD_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_VSYS_VOL_THRESHOLD_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MIN) {
        ESP_LOGE(TAG, "Mistake ! The minimum settable voltage of VSYS is %u mV", XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MAX) {
        ESP_LOGE(TAG, "Mistake ! The maximum settable voltage of VSYS is %u mV", XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MAX);
        return false;
    }
    int val = readRegister(XPOWERS_AXP192_VOFF_SET);
    if (val == -1)return false;
    val &= 0xF8;
    return 0 == writeRegister(XPOWERS_AXP192_VOFF_SET, val | ((millivolt - XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MIN) / XPOWERS_AXP192_VSYS_VOL_THRESHOLD_STEPS));
}

uint16_t AXP192::getSysPowerDownVoltage(void)
{
    int val = readRegister(XPOWERS_AXP192_VOFF_SET);
    if (val == -1)return false;
    return (val & 0x07) * XPOWERS_AXP192_VSYS_VOL_THRESHOLD_STEPS + XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MIN;
}

//  PWROK setting and PWROFF sequence control 25.
// Check the PWROK Pin enable after all dcdc/ldo output valid 128ms
void AXP192::enablePwrOk(){ setRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 4); }

void AXP192::disablePwrOk(){ clrRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 4); }

// POWEROFF Delay 4ms after PWROK enable
void AXP192::enablePowerOffDelay(){ setRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 3); }

// POWEROFF Delay 4ms after PWROK disable
void AXP192::disablePowerOffDelay(){ clrRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 3); }

// POWEROFF Sequence Control the reverse of the Startup
void AXP192::enablePowerSequence(){ setRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 2); }

// POWEROFF Sequence Control at the same time
void AXP192::disablePowerSequence(){ clrRegisterBit(XPOWERS_AXP192_PWROK_SEQU_CTRL, 2);}

// Delay of PWROK after all power output good
bool AXP192::setPwrOkDelay(xpower_pwrok_delay_t opt)
{
    int val = readRegister(XPOWERS_AXP192_PWROK_SEQU_CTRL);
    if (val == -1)return false;
    val &= 0xFC;
    return 0 == writeRegister(XPOWERS_AXP192_PWROK_SEQU_CTRL, val | opt);
}

xpower_pwrok_delay_t AXP192::getPwrOkDelay(){
    int val = readRegister(XPOWERS_AXP192_PWROK_SEQU_CTRL);
    if (val == -1)return XPOWER_PWROK_DELAY_8MS;
    return (xpower_pwrok_delay_t)(val & 0x03);
}

//  Sleep and 26
void AXP192::wakeupControl(xpowers_wakeup_t opt, bool enable)
{
    int val = readRegister(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL);
    if (val == -1)return;
    enable ? (val |= opt) : (val &= (~opt));
    writeRegister(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL, val);
}

bool AXP192::enableWakeup(void){ return setRegisterBit(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL, 1); }

bool AXP192::disableWakeup(void){ return clrRegisterBit(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL, 1); }

bool AXP192::enableSleep(void){ return setRegisterBit(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL, 0); }

bool AXP192::disableSleep(void){ return clrRegisterBit(XPOWERS_AXP192_SLEEP_WAKEUP_CTRL, 0); }


//  RQLEVEL/OFFLEVEL/ONLEVEL setting 27
/**
 * @brief  IRQLEVEL configur
 * @param  opt: 0:1s  1:1.5s  2:2s 3:2.5s
 */
void AXP192::setIrqLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 4));
}

/**
 * @brief  OFFLEVEL configuration
 * @param  opt:  0:4s 1:6s 2:8s 3:10s
 */
void AXP192::setOffLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 2));
}

/**
 * @brief  ONLEVEL configuration
 * @param  opt: 0:128ms 1:512ms 2:1s  3:2s
 */
void AXP192::setOnLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | opt);
}


// DCDC 120%(130%) high voltage turn off PMIC function
void AXP192::setDCHighVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 5) : clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 5);
}

bool AXP192::getDCHighVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 5); }

// DCDCS force PWM control
void AXP192::setDcUVPDebounceTime(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_DC_FORCE_PWM_CTRL);
    val &= 0xFC;
    writeRegister(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, val | opt);
}

void AXP192::settDC1WorkModeToPwm(uint8_t enable)
{
    enable ?
    setRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 2)
    : clrRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 2);
}

void AXP192::settDC2WorkModeToPwm(uint8_t enable)
{
    enable ? setRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 3)
    : clrRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 3);
}

void AXP192::settDC3WorkModeToPwm(uint8_t enable)
{
    enable ?
    setRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 4)
    : clrRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 4);
}

void AXP192::setDCFreqSpreadRangeEn(bool en)
{
    en ?
    setRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 7)
    :  clrRegisterBit(XPOWERS_AXP192_DC_FORCE_PWM_CTRL, 7);
}

void AXP192::enableCCM()
{
    setRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 6);
}

void AXP192::disableCCM()
{
    clrRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 6);
}

bool AXP192::isenableCCM()
{
    return getRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 6);
}

enum DVMRamp {
    XPOWERS_AXP192_DVM_RAMP_15_625US,
    XPOWERS_AXP192_DVM_RAMP_31_250US,
};

//args:enum DVMRamp
void AXP192::setDVMRamp(uint8_t opt)
{
    if (opt > 2)return;
    opt == 0 ? clrRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 5) : setRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 5);
}



/*
 * Power control DCDC1 functions
 */
bool AXP192::isEnableDC1(void){ return getRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 0); }

bool AXP192::enableDC1(void){ return setRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 0); }

bool AXP192::disableDC1(void){ return clrRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 0); }

bool AXP192::setDC1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_DCDC1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_DCDC1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_DCDC1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DC1 minimum voltage is %u mV", XPOWERS_AXP192_DCDC1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_DCDC1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DC1 maximum voltage is %u mV", XPOWERS_AXP192_DCDC1_VOL_MAX);
        return false;
    }
    return 0 == writeRegister(XPOWERS_AXP192_DC_VOL0_CTRL, (millivolt - XPOWERS_AXP192_DCDC1_VOL_MIN) / XPOWERS_AXP192_DCDC1_VOL_STEPS);
}

uint16_t AXP192::getDC1Voltage(void)
{
    return (readRegister(XPOWERS_AXP192_DC_VOL0_CTRL) & 0x3F) * XPOWERS_AXP192_DCDC1_VOL_STEPS + XPOWERS_AXP192_DCDC1_VOL_MIN; //was 0x1F
}



// DCDC1 85% low voltage turn off PMIC function
void AXP192::setDC1LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 0) : clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 0);
}

bool AXP192::getDC1LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 0); }

/*
 * Power control DCDC2 functions
 */
bool AXP192::isEnableDC2(void){ return getRegisterBit(XPOWERS_AXP192_DC2_ONOFF_DVM_CTRL, 0); } //was 1

bool AXP192::enableDC2(void){ return setRegisterBit(XPOWERS_AXP192_DC2_ONOFF_DVM_CTRL, 0); } //was 1

bool AXP192::disableDC2(void){ return clrRegisterBit(XPOWERS_AXP192_DC2_ONOFF_DVM_CTRL, 0); } //was 1

bool AXP192::setDC2Voltage(uint16_t millivolt)
{
    int val = readRegister(XPOWERS_AXP192_DC_VOL1_CTRL);
    if (val == -1)return 0;
    val &= 0xC0; //was 0x80
    if (millivolt >= XPOWERS_AXP192_DCDC2_VOL1_MIN && millivolt <= XPOWERS_AXP192_DCDC2_VOL1_MAX) {
        if (millivolt % XPOWERS_AXP192_DCDC2_VOL_STEPS1) {
            ESP_LOGE(TAG, "Mistake !  The steps must be %umV", XPOWERS_AXP192_DCDC2_VOL_STEPS1);
            return false;
        }
        return  0 == writeRegister(XPOWERS_AXP192_DC_VOL1_CTRL, val | (millivolt - XPOWERS_AXP192_DCDC2_VOL1_MIN) / XPOWERS_AXP192_DCDC2_VOL_STEPS1);
    } else if (millivolt >= XPOWERS_AXP192_DCDC2_VOL2_MIN && millivolt <= XPOWERS_AXP192_DCDC2_VOL2_MAX) {
        if (millivolt % XPOWERS_AXP192_DCDC2_VOL_STEPS2) {
            ESP_LOGE(TAG, "Mistake !  The steps must be %umV", XPOWERS_AXP192_DCDC2_VOL_STEPS2);
            return false;
        }
        val |= (((millivolt - XPOWERS_AXP192_DCDC2_VOL2_MIN) / XPOWERS_AXP192_DCDC2_VOL_STEPS2) + XPOWERS_AXP192_DCDC2_VOL_STEPS2_BASE);
        return  0 == writeRegister(XPOWERS_AXP192_DC_VOL1_CTRL, val);
    }
    return false;
}

uint16_t AXP192::getDC2Voltage(void)
{
    int val = readRegister(XPOWERS_AXP192_DC_VOL1_CTRL);
    if (val ==  -1)return 0;
    val &= 0x3F; //was 0x7F
    if (val < XPOWERS_AXP192_DCDC2_VOL_STEPS2_BASE) {
        return (val  * XPOWERS_AXP192_DCDC2_VOL_STEPS1) +  XPOWERS_AXP192_DCDC2_VOL1_MIN;
    } else  {
        return (val  * XPOWERS_AXP192_DCDC2_VOL_STEPS2) - 200;
    }
    return 0;
}

uint8_t AXP192::getDC2WorkMode(void){ return getRegisterBit(XPOWERS_AXP192_DCDC2_VOL_STEPS2, 7); }

void AXP192::setDC2LowVoltagePowerDown(bool en){
    en ? setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 1) : clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 1);
}

bool AXP192::getDC2LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 1); }

/*
 * Power control DCDC3 functions
 */

bool AXP192::isEnableDC3(void){ return getRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 1); } //was bit 2

bool AXP192::enableDC3(void){ return setRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 1); } //was bit 2

bool AXP192::disableDC3(void){ return clrRegisterBit(XPOWERS_AXP192_DC_ONOFF_DVM_CTRL, 1); } //was bit 2

/**
    0.5~1.2V,10mV/step,71steps
    1.22~1.54V,20mV/step,17steps
    1.6~3.4V,100mV/step,19steps
 */
bool AXP192::setDC3Voltage(uint16_t millivolt)
{
    int val = readRegister(XPOWERS_AXP192_DC_VOL2_CTRL);
    if (val == -1)return false;
    val &= 0x80;
    if (millivolt >= XPOWERS_AXP192_DCDC3_VOL_MIN && millivolt <= XPOWERS_AXP192_DCDC3_VOL_MAX) {
        // if (millivolt % XPOWERS_AXP192_DCDC3_VOL_STEPS) {
        //     ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP192_DCDC3_VOL_STEPS);
        //     return false;
        // }
        val |= (((millivolt - XPOWERS_AXP192_DCDC3_VOL_MIN) / XPOWERS_AXP192_DCDC3_VOL_STEPS) + XPOWERS_AXP192_DCDC3_VOL_STEPS_BASE);
ESP_LOGCONFIG(TAG, "millivolt %u, val %u", millivolt, val);
        return  0 == writeRegister(XPOWERS_AXP192_DC_VOL2_CTRL, val);
    }
    return false;
}


uint16_t AXP192::getDC3Voltage(void)
{
    int val = readRegister(XPOWERS_AXP192_DC_VOL2_CTRL) & 0x7F;
    if (val == -1) {
        return 0;
    } else {
        return ((val & 0x7F)  * XPOWERS_AXP192_DCDC3_VOL_STEPS) +  XPOWERS_AXP192_DCDC3_VOL_MIN;
    }
}

uint8_t AXP192::getDC3WorkMode(void){ return getRegisterBit(XPOWERS_AXP192_DC_VOL2_CTRL, 7); }

// DCDC3 85% low voltage turn off PMIC function
void AXP192::setDC3LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 2) : clrRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 2);
}

bool AXP192::getDC3LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP192_DC_OVP_UVP_CTRL, 2); }

/*
* Power control ALDO1 functions
*/
bool AXP192::isEnableALDO1(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 0); }

bool AXP192::enableALDO1(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 0); }

bool AXP192::disableALDO1(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 0); }

bool AXP192::setALDO1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_ALDO1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_ALDO1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_ALDO1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO1 minimum output voltage is  %umV", XPOWERS_AXP192_ALDO1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_ALDO1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO1 maximum output voltage is  %umV", XPOWERS_AXP192_ALDO1_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL0_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP192_ALDO1_VOL_MIN) / XPOWERS_AXP192_ALDO1_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL0_CTRL, val);
}

uint16_t AXP192::getALDO1Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL0_CTRL) & 0x1F;
    return val * XPOWERS_AXP192_ALDO1_VOL_STEPS + XPOWERS_AXP192_ALDO1_VOL_MIN;
}

/*
* Power control ALDO2 functions
*/
bool AXP192::isEnableALDO2(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 1); }

bool AXP192::enableALDO2(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 1); }

bool AXP192::disableALDO2(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 1); }

bool AXP192::setALDO2Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_ALDO2_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_ALDO2_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_ALDO2_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO2 minimum output voltage is  %umV", XPOWERS_AXP192_ALDO2_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_ALDO2_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO2 maximum output voltage is  %umV", XPOWERS_AXP192_ALDO2_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL1_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP192_ALDO2_VOL_MIN) / XPOWERS_AXP192_ALDO2_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL1_CTRL, val);
}

uint16_t AXP192::getALDO2Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL1_CTRL) & 0x1F;
    return val * XPOWERS_AXP192_ALDO2_VOL_STEPS + XPOWERS_AXP192_ALDO2_VOL_MIN;
}

/*
 * Power control ALDO3 functions
 */
bool AXP192::isEnableALDO3(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); }

bool AXP192::enableALDO3(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); }

bool AXP192::disableALDO3(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); }

bool AXP192::setALDO3Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_ALDO3_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_ALDO3_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_ALDO3_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO3 minimum output voltage is  %umV", XPOWERS_AXP192_ALDO3_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_ALDO3_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO3 maximum output voltage is  %umV", XPOWERS_AXP192_ALDO3_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL2_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP192_ALDO3_VOL_MIN) / XPOWERS_AXP192_ALDO3_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL2_CTRL, val);
}

uint16_t AXP192::getALDO3Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL2_CTRL) & 0x1F;
    return val * XPOWERS_AXP192_ALDO3_VOL_STEPS + XPOWERS_AXP192_ALDO3_VOL_MIN;
}

/*
 * Power control ALDO4 functions
 */
bool AXP192::isEnableALDO4(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); }

bool AXP192::enableALDO4(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); }

bool AXP192::disableALDO4(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); }

bool AXP192::setALDO4Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_ALDO4_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_ALDO4_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_ALDO4_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO4 minimum output voltage is  %umV", XPOWERS_AXP192_ALDO4_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_ALDO4_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO4 maximum output voltage is  %umV", XPOWERS_AXP192_ALDO4_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL3_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP192_ALDO4_VOL_MIN) / XPOWERS_AXP192_ALDO4_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL3_CTRL, val);
}

uint16_t AXP192::getALDO4Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL3_CTRL) & 0x1F;
    return val * XPOWERS_AXP192_ALDO4_VOL_STEPS + XPOWERS_AXP192_ALDO4_VOL_MIN;
}


/*
* Power control DLDO3 functions (Vibration Motor)
*/
bool AXP192::isEnableDLDO3(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); } //was 7

bool AXP192::enableDLDO3(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); } //was 7

bool AXP192::disableDLDO3(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 3); } //was 7

bool AXP192::setDLDO3Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_DLDO3_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_DLDO3_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_DLDO3_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DLDO3 minimum output voltage is  %umV", XPOWERS_AXP192_DLDO3_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_DLDO3_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DLDO3 maximum output voltage is  %umV", XPOWERS_AXP192_DLDO3_VOL_MAX);
        return false;
    }
    uint16_t val = readRegister(XPOWERS_AXP192_LDO_VOL7_CTRL) & 0xF0;
    val |= (millivolt - XPOWERS_AXP192_DLDO3_VOL_MIN) / XPOWERS_AXP192_DLDO3_VOL_STEPS;
ESP_LOGCONFIG(TAG, "DLD03 millivolt %u, val %u %02X", millivolt, val, val);
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL7_CTRL, val);
}

uint16_t AXP192::getDLDO3Voltage(void)
{
    int val = readRegister(XPOWERS_AXP192_LDO_VOL7_CTRL);
    if (val == -1)return 0;
    val &= 0x0F;
    return val * XPOWERS_AXP192_DLDO3_VOL_STEPS + XPOWERS_AXP192_DLDO3_VOL_MIN;
}

/*
* Power control DLDO2 functions (Perif VDD, Display VDD)
*/
bool AXP192::isEnableDLDO2(void){ return getRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); } //LDO2

bool AXP192::enableDLDO2(void){ return setRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); }

bool AXP192::disableDLDO2(void){ return clrRegisterBit(XPOWERS_AXP192_LDO_ONOFF_CTRL0, 2); }

bool AXP192::setDLDO2Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP192_DLDO2_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP192_DLDO2_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP192_DLDO2_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DLDO2 minimum output voltage is  %umV", XPOWERS_AXP192_DLDO2_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP192_DLDO2_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DLDO2 maximum output voltage is  %umV", XPOWERS_AXP192_DLDO2_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP192_LDO_VOL8_CTRL) & 0x0F; //was 0xE0
    val |= (((millivolt - XPOWERS_AXP192_DLDO2_VOL_MIN) / XPOWERS_AXP192_DLDO2_VOL_STEPS)<<4);
ESP_LOGCONFIG(TAG, "DLD02 millivolt %u, val %u %02X", millivolt, val, val);
    return 0 == writeRegister(XPOWERS_AXP192_LDO_VOL8_CTRL, val);
}

uint16_t AXP192::getDLDO2Voltage(void)
{
    int val =  readRegister(XPOWERS_AXP192_LDO_VOL8_CTRL);
    if (val == -1)return 0;
    val &= 0xF0; //was 0x1F
    return (val>>4) * XPOWERS_AXP192_DLDO2_VOL_STEPS + XPOWERS_AXP192_DLDO2_VOL_MIN;
}


/*
 * Power ON OFF IRQ TIMMING Control method
 */

void AXP192::setIrqLevelTime(xpowers_irq_time_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    val &= 0xCF;
    writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 4));
}

xpowers_irq_time_t AXP192::getIrqLevelTime(void)
{
    return (xpowers_irq_time_t)((readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL) & 0x30) >> 4);
}

/**
* @brief Set the PEKEY power-on long press time.
* @param opt: See xpowers_press_on_time_t enum for details.
* @retval
*/
bool AXP192::setPowerKeyPressOnTime(uint8_t opt) //updated
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return false;
    val  &= 0x3F;
    return 0 ==  writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 6));
}

/**
* @brief Get the PEKEY power-on long press time.
* @retval See xpowers_press_on_time_t enum for details.
*/
uint8_t AXP192::getPowerKeyPressOnTime(void) //updated
{
    int val =  readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return 0;
    return ((val & 0xC0) >> 6) ;
}

/**
* @brief Set the PEKEY power-off long press time.
* @param opt: See xpowers_press_off_time_t enum for details.
* @retval
*/
bool AXP192::setPowerKeyPressOffTime(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return false;
    val  &= 0xFC; //was 0xF3
    return 0 == writeRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL, val | (opt ));
}

/**
* @brief Get the PEKEY power-off long press time.
* @retval See xpowers_press_off_time_t enum for details.
*/
uint8_t AXP192::getPowerKeyPressOffTime(void) //updated
{
    return ((readRegister(XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL) & 0x03) );
}

/*
 * ADC Control method
 */
//All this code not reqd, remove
bool AXP192::enableGeneralAdcChannel(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 3); } // GPIO0 ADC, GPIO1, 2, 3 also available but not coded

bool AXP192::disableGeneralAdcChannel(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 3); }

bool AXP192::enableTemperatureMeasure(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 7); } //was bit 4

bool AXP192::disableTemperatureMeasure(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 7); }

float AXP192::getTemperature(void)
{
    uint16_t raw = readRegisterH6L8(XPOWERS_AXP192_ADC_DATA_RELUST8, XPOWERS_AXP192_ADC_DATA_RELUST9);
    return XPOWERS_AXP192_CONVERSION(raw);
}

bool AXP192::enableSystemVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 5); } //was bit 3, assumes ACIN is sytem voltage - actually USB voltage

bool AXP192::disableSystemVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 5); }

uint16_t AXP192::getSystemVoltage(void)
{
    return readRegisterH6L8(XPOWERS_AXP192_ADC_DATA_RELUST6, XPOWERS_AXP192_ADC_DATA_RELUST7);
}

bool AXP192::enableVbusVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 3); } //was bit 2

bool AXP192::disableVbusVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 3); }
//end of code remove block

bool AXP192::configureAdcMeasure(void){
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 7);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 6);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 5);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 4);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 3);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 2);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 1);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 0);
    setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 7);
    clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 3);
    clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 2);
    clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 1);
    return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL2, 0);
}

float AXP192::getACINVoltage(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_ACINVH, XPOWERS_AXP192_ADC_DATA_ACINVL)*XPOWERS_AXP192_ADC_SCALE_ACINV/1000.0;
}

float AXP192::getACINCurrent(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_ACINIH, XPOWERS_AXP192_ADC_DATA_ACINIL)*XPOWERS_AXP192_ADC_SCALE_ACINI/1000.0;
}

float AXP192::getVBUSVoltage(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_VBUSVH, XPOWERS_AXP192_ADC_DATA_VBUSVL)*XPOWERS_AXP192_ADC_SCALE_VBUSV/1000.0;
}

float AXP192::getVBUSCurrent(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_VBUSIH, XPOWERS_AXP192_ADC_DATA_VBUSIL)*XPOWERS_AXP192_ADC_SCALE_VBUSI/1000.0;
}

float AXP192::getInternalTemperature(void)
{
    return (readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_INTTH, XPOWERS_AXP192_ADC_DATA_INTTL)*XPOWERS_AXP192_ADC_SCALE_INTT)+XPOWERS_AXP192_ADC_OFFSET_INTT;
}

float AXP192::getBattPower(void)
{
    return readRegisterH8M8L8(XPOWERS_AXP192_ADC_DATA_BATTPH, XPOWERS_AXP192_ADC_DATA_BATTPM, XPOWERS_AXP192_ADC_DATA_BATTPL)/1000000.0;
}

float AXP192::getBattChargeCurrent(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_BATTCHGH, XPOWERS_AXP192_ADC_DATA_BATTCHGL)*XPOWERS_AXP192_ADC_SCALE_BATTCHGI/1000.0;
}

float AXP192::getBattDischargeCurrent(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_BATTDCHGH, XPOWERS_AXP192_ADC_DATA_BATTDCHGL)*XPOWERS_AXP192_ADC_SCALE_BATTCHGI/1000.0;
}

float AXP192::getAPSVoltage(void)
{
    // if (!isBatteryConnect()) {
    //     return 0;
    // }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_APSVH, XPOWERS_AXP192_ADC_DATA_APSVL)*XPOWERS_AXP192_ADC_SCALE_APSV/1000.0;
}





// old code remove when possible
bool AXP192::enableTSPinMeasure(void) //updated
{
    // TS pin is the battery temperature sensor input and will affect the charger
    uint8_t value =  readRegister(XPOWERS_AXP192_TS_PIN_CTRL);
    value &= 0xD0; //was 0xE0
    writeRegister(XPOWERS_AXP192_TS_PIN_CTRL, value | 0x26); //was 0x07
    return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 0); //was bit 1
}

bool AXP192::disableTSPinMeasure(void) //updated
{
    // TS pin is the external fixed input and doesn't affect the charger
    uint8_t value =  readRegister(XPOWERS_AXP192_TS_PIN_CTRL);
    value &= 0xD0; //was 0xF0
    writeRegister(XPOWERS_AXP192_TS_PIN_CTRL, value | 0x00); //was 0x10
    return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 0); //was bit 1
}


bool AXP192::enableTSPinLowFreqSample(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 0); } //was bit 7

bool AXP192::disableTSPinLowFreqSample(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_DATA_RELUST2, 7); }


/**
 * Calculate temperature from TS pin ADC value using Steinhart-Hart equation.
 *
 * @param SteinhartA Steinhart-Hart coefficient A (default: 1.126e-3)
 * @param SteinhartB Steinhart-Hart coefficient B (default: 2.38e-4)
 * @param SteinhartC Steinhart-Hart coefficient C (default: 8.5e-8)
 * @return Temperature in Celsius. Returns 0 if ADC value is 0x2000 (invalid measurement).
 *
 * @details
 * This function converts the ADC reading from the TS pin to temperature using:
 * 1. Voltage calculation: V = ADC_raw × 0.0005 (V)
 * 2. Resistance calculation: R = V / I (Ω), where I = 50μA
 * 3. Temperature calculation: T = 1/(A+B*ln(R)+C*(ln(R))^3) - 273.15 (℃)
 *
 * @note
 * The calculation parameters are from the AXP192 Datasheet, using the TH11-3H103F NTC resistor
 *     as the Steinhart-Hart equation calculation parameters
 * 1. Coefficients A, B, C should be calibrated for specific NTC thermistor.
 * 2. ADC value 0x2000 indicates sensor fault (e.g., open circuit).
 * 3. Valid temperature range: typically -20℃ to 60℃. Accuracy may degrade outside this range.
 */
float AXP192::getTsTemperature(float SteinhartA,
                       float SteinhartB,
                       float SteinhartC)
{
    uint16_t  adc_raw =  getTsPinValue();  // Read raw ADC value from TS pin

    // Check for invalid measurement (0x2000 indicates sensor disconnection)
    if (adc_raw == 0x2000) {
        return 0;
    }
    float current_ma = 0.05f;  // Current source: 50μA
    float voltage = adc_raw * 0.0005f;  // Convert ADC value to voltage (V)
    float resistance = voltage / (current_ma / 1000.0f);  // Calculate resistance (Ω)
    // Convert resistance to temperature using Steinhart-Hart equation
    return resistance_to_temperature(resistance, SteinhartA, SteinhartB, SteinhartC);
}

// raw value
uint16_t AXP192::getTsPinValue(void)
{
    return readRegisterH6L8(XPOWERS_AXP192_ADC_DATA_RELUST2, XPOWERS_AXP192_ADC_DATA_RELUST3);
}

bool AXP192::enableBattVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 7); } //was 0

bool AXP192::disableBattVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP192_ADC_CHANNEL_CTRL1, 7); }

bool AXP192::enableBattDetection(void){ return setRegisterBit(XPOWERS_AXP192_BAT_DET_CTRL, 6); } //was 0

bool AXP192::disableBattDetection(void){ return clrRegisterBit(XPOWERS_AXP192_BAT_DET_CTRL, 6); } //was 0

float AXP192::getBattVoltage(void)
{
    if (!isBatteryConnect()) {
        return 0;
    }
    return readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_BATTVH, XPOWERS_AXP192_ADC_DATA_BATTVL)/1000.0;
}

int AXP192::getBatteryPercent(void)
{
    if (!isBatteryConnect()) {
        return -1; // Battery not connected so Percent not available
    }
    // No simple register so we read battery voltage and estimate % charge from that
    uint16_t batteryVoltage = readRegisterH8L4(XPOWERS_AXP192_ADC_DATA_BATTVH, XPOWERS_AXP192_ADC_DATA_BATTVL);
    int batteryPercent = ((batteryVoltage - 3200)*100)/(3670 - 3200);
ESP_LOGD(TAG, "BattVoltage %u mV, %% %u", batteryVoltage, batteryPercent);

    if (batteryVoltage > 3670) return 100; //Consider fully charged, should also check that charging is off
    return batteryPercent;
}

int AXP192::getScaledBatteryPercent(uint16_t discharge_0pc_voltage, uint16_t discharge_100pc_voltage, uint16_t charge_0pc_current, uint16_t charge_100pc_current)
{
    int batteryPercent = 0;
    if (!isBatteryConnect()) {
        return -1; // Battery not connected so Percent not available
    }
    // No simple register so we read battery voltage and estimate % charge from that
    uint16_t batteryVoltage = 1000.0*getBattVoltage(); //mV
    uint16_t batteryChargeCurrent = 1000.0*getBattChargeCurrent(); //mA
    if (isCharging()) {
        // Use charging current as indicator of state of charge
        if (batteryChargeCurrent <= charge_100pc_current){
            batteryPercent = 100;
        } else {
            if (batteryChargeCurrent >= charge_0pc_current){
                batteryPercent = 0;
            } else {
                batteryPercent = ((charge_0pc_current - batteryChargeCurrent)*100)/(charge_0pc_current - charge_100pc_current);
            }
        }
    } else {
        if (batteryVoltage >= discharge_100pc_voltage) {
            batteryPercent = 100; //Consider fully charged, should also check that charging is off
        } else {
            if (batteryVoltage <= discharge_0pc_voltage) {
                batteryPercent = 0;
            } else {
                batteryPercent = ((batteryVoltage - discharge_0pc_voltage)*100)/(discharge_100pc_voltage - discharge_0pc_voltage);
            }
        }
    }

    return batteryPercent;
}

/*
* CHG LED setting and control
*/
// void AXP192::enableChargingLed(void)
// {
//     setRegisterBit(XPOWERS_AXP192_CHGLED_SET_CTRL, 0);
// }

// void AXP192::disableChargingLed(void)
// {
//     clrRegisterBit(XPOWERS_AXP192_CHGLED_SET_CTRL, 0);
// }

/**
* @brief Set charging led mode. (actually not used in Core2 unit)
* @retval See xpowers_chg_led_mode_t enum for details.
*/
void AXP192::setChargingLedMode(uint8_t mode)
{
    int val;
    switch (mode) {
    case XPOWERS_CHG_LED_OFF:
    // clrRegisterBit(XPOWERS_AXP192_CHGLED_SET_CTRL, 0);
    // break;
    case XPOWERS_CHG_LED_BLINK_1HZ:
    case XPOWERS_CHG_LED_BLINK_4HZ:
    case XPOWERS_CHG_LED_ON:
        val = readRegister(XPOWERS_AXP192_CHGLED_SET_CTRL);
        if (val == -1)return;
        val &= 0xC7; //was 0xC8
        val |= 0x08;    //use manual ctrl was 0x05
        val |= (mode << 4);
        // in AXP192 just set GPIO1 as an output
        writeRegister(XPOWERS_AXP192_CHGLED_SET_CTRL, val);
        break;
    case XPOWERS_CHG_LED_CTRL_CHG:
        val = readRegister(XPOWERS_AXP192_CHGLED_SET_CTRL);
        if (val == -1)return;
        val &= 0xC7; //was F9
        val |= 0x00; //clear bit 3
        // writeRegister(XPOWERS_AXP192_CHGLED_SET_CTRL, val | 0x01); // use type A mode
        // writeRegister(XPOWERS_AXP192_CHGLED_SET_CTRL, val | 0x02); // use type B mode
        // in AXP192 just set GPIO1 as an output
        writeRegister(XPOWERS_AXP192_CHGLED_SET_CTRL, val);
        break;
    default:
        break;
    }
}

uint8_t AXP192::getChargingLedMode()
{
    int val = readRegister(XPOWERS_AXP192_CHGLED_SET_CTRL);
    if (val == -1)return XPOWERS_CHG_LED_OFF;
//    val >>= 1;
    if ((val & 0x08) == 0x08) { //was 0x02
        val >>= 4;
        return val & 0x03;
    }
    return XPOWERS_CHG_LED_CTRL_CHG;
}

/**
 * @brief  Precharge current limit
 * @note  Precharge current limit 25*N mA
 * @param  opt: 25 * opt
 * @retval None
 */
void AXP192::setPrechargeCurr(xpowers_prechg_t opt)
{
    int val = readRegister(XPOWERS_AXP192_IPRECHG_SET);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP192_IPRECHG_SET, val | opt);
}

xpowers_prechg_t AXP192::getPrechargeCurr(void)
{
    return (xpowers_prechg_t)(readRegister(XPOWERS_AXP192_IPRECHG_SET) & 0x03);
}


/**
* @brief Set charge current.
* @param  opt: See xpowers_axp192_chg_curr_t enum for details.
* @retval
*/
bool AXP192::setChargerConstantCurr(uint8_t opt)
{
    if (opt > XPOWERS_AXP192_CHG_CUR_1000MA)return false;
    int val = readRegister(XPOWERS_AXP192_ICC_CHG_SET);
    if (val == -1)return false;
    val &= 0xF8; //was 0xE0
    return 0 == writeRegister(XPOWERS_AXP192_ICC_CHG_SET, val | opt);
}

/**
 * @brief Get charge current settings.
*  @retval See xpowers_axp192_chg_curr_t enum for details.
 */
uint8_t AXP192::getChargerConstantCurr(void)
{
    int val = readRegister(XPOWERS_AXP192_ICC_CHG_SET);
    if (val == -1)return 0;
    return val & 0x0F; //was 0x1F
}

/**
 * @brief  Charging termination current
 * @note   Charging termination of current limit
 * @retval
 */
void AXP192::setChargerTerminationCurr(xpowers_axp192_chg_iterm_t opt) //updated
{
    int val = readRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    val &= 0xEF; //was F0
    writeRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL, val | opt);
}

xpowers_axp192_chg_iterm_t AXP192::getChargerTerminationCurr(void) //updated
{
    return (xpowers_axp192_chg_iterm_t)(readRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL) & 0x10); //was 0X0F
}

void AXP192::enableChargerTerminationLimit(void) 
{
    int val = readRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL, val | 0x10);
}

void AXP192::disableChargerTerminationLimit(void)
{
    int val = readRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP192_ITERM_CHG_SET_CTRL, val & 0xEF);
}

bool AXP192::isChargerTerminationLimit(void){ return getRegisterBit(XPOWERS_AXP192_ITERM_CHG_SET_CTRL, 4); }


/**
* @brief Set charge target voltage.
* @param  opt: See xpowers_axp192_chg_vol_t enum for details.
* @retval
*/
bool AXP192::setChargeTargetVoltage(uint8_t opt) //updated
{
    if (opt > XPOWERS_AXP192_CHG_VOL_MAX)return false;
    int val = readRegister(XPOWERS_AXP192_CV_CHG_VOL_SET);
    if (val == -1)return false;
    val &= 0x9F; //was 0xF8
    return 0 == writeRegister(XPOWERS_AXP192_CV_CHG_VOL_SET, val | (opt << 5));
}

/**
 * @brief Get charge target voltage settings.
 * @retval See xpowers_axp192_chg_vol_t enum for details.
 */
uint8_t AXP192::getChargeTargetVoltage(void){ return ((readRegister(XPOWERS_AXP192_CV_CHG_VOL_SET) & 0x06) >> 5); } //updated


/**
 * @brief  Thermal regulation threshold setting
 * @note   Thermal regulation threshold setting
 */
void AXP192::setThermaThreshold(xpowers_thermal_t opt)
{
    int val = readRegister(XPOWERS_AXP192_THE_REGU_THRES_SET);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP192_THE_REGU_THRES_SET, val | opt);
}

xpowers_thermal_t AXP192::getThermaThreshold(void)
{
    return (xpowers_thermal_t)(readRegister(XPOWERS_AXP192_THE_REGU_THRES_SET) & 0x03);
}

uint8_t AXP192::getBatteryParameter(){ return readRegister(XPOWERS_AXP192_BAT_PARAME); }

void AXP192::fuelGaugeControl(bool writeROM, bool enable)
{
    if (writeROM) {
        clrRegisterBit(XPOWERS_AXP192_FUEL_GAUGE_CTRL, 4);
    } else {
        setRegisterBit(XPOWERS_AXP192_FUEL_GAUGE_CTRL, 4);
    }
    if (enable) {
        setRegisterBit(XPOWERS_AXP192_FUEL_GAUGE_CTRL, 0);
    } else {
        clrRegisterBit(XPOWERS_AXP192_FUEL_GAUGE_CTRL, 0);
    }
}

/*
 * Interrupt status/control functions
 */

/**
* @brief  Get the interrupt controller mask value.
* @retval   Mask value corresponds to xpowers_axp192_irq_t ,
*/
uint64_t AXP192::getIrqStatus(void)
{
    statusRegister[0] = readRegister(XPOWERS_AXP192_INTSTS1);
    statusRegister[1] = readRegister(XPOWERS_AXP192_INTSTS2);
    statusRegister[2] = readRegister(XPOWERS_AXP192_INTSTS3);
    statusRegister[3] = readRegister(XPOWERS_AXP192_INTSTS4); //extra register in AXP192
    return (uint32_t)(statusRegister[0] << 24) | (uint32_t)(statusRegister[1] << 16) | (uint32_t)(statusRegister[2] << 8) | (uint32_t)(statusRegister[3]);
}


/**
 * @brief  Clear interrupt controller state.
 */
void AXP192::clearIrqStatus()
{
    for (int i = 0; i < XPOWERS_AXP192_INTSTS_CNT; i++) {
        writeRegister(XPOWERS_AXP192_INTSTS1 + i, 0xFF);
        statusRegister[i] = 0;
    }
}

/*
*  @brief  Debug interrupt setting register
* */

void AXP192::printIntRegister()
{
    for (int i = 0; i < XPOWERS_AXP192_INTSTS_CNT; i++) {
        uint8_t val =  readRegister(XPOWERS_AXP192_INTEN1 + i);
        printf("INT[%d] HEX:0x%X\n", i, val);
    }
}


/**
 * @brief  Enable PMU interrupt control mask .
 * @param  opt: View the related chip type xpowers_axp192_irq_t enumeration
 *              parameters in "axp192_cost.h"
 * @retval
 */
bool AXP192::enableIRQ(uint64_t opt){ return setInterruptImpl(opt, true); }

/**
 * @brief  Disable PMU interrupt control mask .
 * @param  opt: View the related chip type xpowers_axp192_irq_t enumeration
 *              parameters in "axp192_cost.h"
 * @retval
 */
bool AXP192::disableIRQ(uint64_t opt){ return setInterruptImpl(opt, false); }

//IRQ STATUS 0
bool AXP192::isDropWarningLevel2Irq(void)
{
    uint8_t mask = XPOWERS_AXP192_WARNING_LEVEL2_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isDropWarningLevel1Irq(void)
{
    uint8_t mask = XPOWERS_AXP192_WARNING_LEVEL1_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isGaugeWdtTimeoutIrq()
{
    uint8_t mask = XPOWERS_AXP192_WDT_TIMEOUT_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isStateOfChargeLowIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_GAUGE_NEW_SOC_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isBatChargerOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_CHG_OVER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isBatChargerUnderTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_CHG_UNDER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isBatWorkOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_NOR_OVER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP192::isBatWorkUnderTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_NOR_UNDER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

//IRQ STATUS 1
bool AXP192::isVbusInsertIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_VBUS_INSERT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isVbusRemoveIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_VBUS_REMOVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isBatInsertIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_INSERT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isBatRemoveIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_REMOVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isPekeyShortPressIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_PKEY_SHORT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;

}

bool AXP192::isPekeyLongPressIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_PKEY_LONG_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isPekeyNegativeIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_PKEY_NEGATIVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP192::isPekeyPositiveIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_PKEY_POSITIVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

//IRQ STATUS 2
bool AXP192::isWdtExpireIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_WDT_EXPIRE_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isLdoOverCurrentIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_LDO_OVER_CURR_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isBatChargeDoneIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_CHG_DONE_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isBatChargeStartIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_CHG_START_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isBatDieOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_DIE_OVER_TEMP_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isChargeOverTimeoutIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_CHARGER_TIMER_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP192::isBatOverVoltageIrq(void)
{
    uint8_t mask = XPOWERS_AXP192_BAT_OVER_VOL_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}


uint8_t AXP192::getChipID(void)
{
    return readRegister(XPOWERS_AXP192_IC_TYPE);
}

bool AXP192::getProtectedChannel(uint8_t channel) {
    return __protectedMask & _BV(channel);
}

uint16_t AXP192::getPowerChannelVoltage(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return getDC1Voltage();
    case XPOWERS_DCDC2:
        return getDC2Voltage();
    case XPOWERS_DCDC3:
        return getDC3Voltage();
     case XPOWERS_ALDO1:
        return getALDO1Voltage();
    case XPOWERS_ALDO2:
        return getALDO2Voltage();
    case XPOWERS_ALDO3:
        return getALDO3Voltage();
    case XPOWERS_ALDO4:
        return getALDO4Voltage();
    case XPOWERS_DLDO3:
        return getDLDO3Voltage();
    case XPOWERS_DLDO2:
        return getDLDO2Voltage();
    // case XPOWERS_VBACKUP:
    //     return getButtonBatteryVoltage();
    default:
        break;
    }
    return 0;
}

bool inline AXP192::enablePowerOutput(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return enableDC1();
    case XPOWERS_DCDC2:
        return enableDC2();
    case XPOWERS_DCDC3:
        return enableDC3();
    case XPOWERS_ALDO1:
        return enableALDO1();
    case XPOWERS_ALDO2:
        return enableALDO2();
    case XPOWERS_ALDO3:
        return enableALDO3();
    case XPOWERS_ALDO4:
        return enableALDO4();
    case XPOWERS_DLDO3:
        return enableDLDO3();
    case XPOWERS_DLDO2:
        return enableDLDO2();
    // case XPOWERS_VBACKUP:
    //     return enableButtonBatteryCharge();
    default:
        break;
    }
    return false;
}

bool inline AXP192::disablePowerOutput(uint8_t channel)
{
    if (getProtectedChannel(channel)) {
        ESP_LOGE(TAG, "Failed to disable the power channel, the power channel has been protected");
        return false;
    }
    switch (channel) {
    case XPOWERS_DCDC1:
        return disableDC1();
    case XPOWERS_DCDC2:
        return disableDC2();
    case XPOWERS_DCDC3:
        return disableDC3();
    case XPOWERS_ALDO1:
        return disableALDO1();
    case XPOWERS_ALDO2:
        return disableALDO2();
    case XPOWERS_ALDO3:
        return disableALDO3();
    case XPOWERS_ALDO4:
        return disableALDO4();
    case XPOWERS_DLDO3:
        return disableDLDO3();
    case XPOWERS_DLDO2:
        return disableDLDO2();
    // case XPOWERS_VBACKUP:
    //     return disableButtonBatteryCharge();
    default:
        break;
    }
    return false;
}

bool inline AXP192::isPowerChannelEnable(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return isEnableDC1();
    case XPOWERS_DCDC2:
        return isEnableDC2();
    case XPOWERS_DCDC3:
        return isEnableDC3();
    case XPOWERS_ALDO1:
        return isEnableALDO1();
    case XPOWERS_ALDO2:
        return isEnableALDO2();
    case XPOWERS_ALDO3:
        return isEnableALDO3();
    case XPOWERS_ALDO4:
        return isEnableALDO4();
    case XPOWERS_DLDO3:
        return isEnableDLDO3();
    case XPOWERS_DLDO2:
        return isEnableDLDO2();
    // case XPOWERS_VBACKUP:
    //     return isEnableButtonBatteryCharge();
    default:
        break;
    }
    return false;
}

bool inline AXP192::setPowerChannelVoltage(uint8_t channel, uint16_t millivolt)
{
    if (getProtectedChannel(channel)) {
        ESP_LOGE(TAG, "Failed to set the power channel, the power channel has been protected");
        return false;
    }
    switch (channel) {
    case XPOWERS_DCDC1:
        return setDC1Voltage(millivolt);
    case XPOWERS_DCDC2:
        return setDC2Voltage(millivolt);
    case XPOWERS_DCDC3:
        return setDC3Voltage(millivolt);
    case XPOWERS_ALDO1:
        return setALDO1Voltage(millivolt);
    case XPOWERS_ALDO2:
        return setALDO2Voltage(millivolt);
    case XPOWERS_ALDO3:
        return setALDO3Voltage(millivolt);
    case XPOWERS_ALDO4:
        return setALDO4Voltage(millivolt);
    case XPOWERS_DLDO3:
        return setDLDO3Voltage(millivolt);
    case XPOWERS_DLDO2:
        return setDLDO2Voltage(millivolt);
    // case XPOWERS_VBACKUP:
    //     return setButtonBatteryChargeVoltage(millivolt);
    default:
        break;
    }
    return false;
}

bool AXP192::initImpl()
{
    if (getChipID() == XPOWERS_AXP192_CHIP_ID) {
        disableTSPinMeasure();      //Disable NTC temperature detection by default
        return true;
    }
    return false;
}

/*
 * Interrupt control functions
 */
bool AXP192::setInterruptImpl(uint32_t opts, bool enable)
{
    int res = 0;
    uint8_t data = 0, value = 0;
    ESP_LOGD(TAG, "%s - HEX:0x %lx \n", enable ? "ENABLE" : "DISABLE", opts);
    if (opts & 0x0000FF) {
        value = opts & 0xFF;
        // ESP_LOGD(TAG, "Write INT0: %x\n", value);
        data = readRegister(XPOWERS_AXP192_INTEN1);
        intRegister[0] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP192_INTEN1, intRegister[0]);
    }
    if (opts & 0x00FF00) {
        value = opts >> 8;
        // ESP_LOGD(TAG, "Write INT1: %x\n", value);
        data = readRegister(XPOWERS_AXP192_INTEN2);
        intRegister[1] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP192_INTEN2, intRegister[1]);
    }
    if (opts & 0xFF0000) {
        value = opts >> 16;
        // ESP_LOGD(TAG, "Write INT2: %x\n", value);
        data = readRegister(XPOWERS_AXP192_INTEN3);
        intRegister[2] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP192_INTEN3, intRegister[2]);
    }
    return res == 0;
}


float AXP192::resistance_to_temperature(float resistance, float SteinhartA, 
                                         float SteinhartB, float SteinhartC)
{
    float ln_r = log(resistance);
    float t_inv = SteinhartA + SteinhartB * ln_r + SteinhartC * pow(ln_r, 3);
    return (1.0f / t_inv) - 273.15f;
}

} // namespace axp192
} // namespace esphome



