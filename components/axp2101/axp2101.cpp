#include "axp2101.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace axp2101 {

static const char *TAG = "axp2101";

void AXP2101::AXP2101::setup()
{
    ESP_LOGCONFIG(TAG, "Setting Up AXP2101 PMU...");

    // Init, check the chip ID
    // If no valid ID (0x4a) was returned, call it failed.
    if (!this->initImpl()) {
        ESP_LOGE(TAG, "Failed to read Chip ID, Exiting...");
        this->mark_failed();
        return;
    }

    ESP_LOGCONFIG(TAG, "Chip ID:0x%x", this->getChipID());

    delay(10);
    // Disable unused channels
    this->disableDC2();
    this->disableDC3();
    this->disableDC4();
    this->disableDC5();

    this->disableCPUSLDO();
    this->disableDLDO1();

    this->clearIrqStatus();

    this->enableVbusVoltageMeasure();
    this->enableBattVoltageMeasure();
    this->enableSystemVoltageMeasure();
    this->enableTemperatureMeasure();

    // It is necessary to disable the detection function of the TS pin on the board
    // without the battery temperature detection function, otherwise it will cause abnormal charging
    this->disableTSPinMeasure();

    // Disable all interrupts
    this->disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Clear all interrupt flags
    this->clearIrqStatus();
    // Enable the required interrupt function
    this->enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
        // XPOWERS_AXP2101_PKEY_NEGATIVE_IRQ | XPOWERS_AXP2101_PKEY_POSITIVE_IRQ   |   //POWER KEY
    );

    /*
      The default setting is CHGLED is automatically controlled by the this->
    - XPOWERS_CHG_LED_OFF,
    - XPOWERS_CHG_LED_BLINK_1HZ,
    - XPOWERS_CHG_LED_BLINK_4HZ,
    - XPOWERS_CHG_LED_ON,
    - XPOWERS_CHG_LED_CTRL_CHG,
    * */

    this->setChargingLedMode(XPOWERS_CHG_LED_OFF);

    // Set the precharge charging current
    this->setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_50MA);
    // Set constant current charge current limit
    this->setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_200MA);
    // Set stop charging termination current
    this->setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);

    // Set charge cut-off voltage
    this->setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V1);

    // Set the watchdog trigger event type
    // this->setWatchdogConfig(XPOWERS_AXP2101_WDT_IRQ_TO_PIN);
    // Set watchdog timeout
    this->setWatchdogTimeout(XPOWERS_AXP2101_WDT_TIMEOUT_4S);
    // Enable watchdog to trigger interrupt event
    this->enableWatchdog();

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

    // Enable Button Battery charge
    this->enableButtonBatteryCharge();

    // Set Button Battery charge voltage
    this->setButtonBatteryChargeVoltage(3300);

    ESP_LOGI(TAG, "AXP2101 PMU Init Success!");
}



void AXP2101::AXP2101::loop(){ }


void AXP2101::AXP2101::dump_config()
{
    ESP_LOGCONFIG(TAG, "AXP2101:");
    ESP_LOGCONFIG(TAG, "DC1  : %s   Voltage:%u mV",  this->isEnableDC1()  ? "+" : "-", this->getDC1Voltage());
    ESP_LOGCONFIG(TAG, "DC2  : %s   Voltage:%u mV",  this->isEnableDC2()  ? "+" : "-", this->getDC2Voltage());
    ESP_LOGCONFIG(TAG, "DC3  : %s   Voltage:%u mV",  this->isEnableDC3()  ? "+" : "-", this->getDC3Voltage());
    ESP_LOGCONFIG(TAG, "DC4  : %s   Voltage:%u mV",  this->isEnableDC4()  ? "+" : "-", this->getDC4Voltage());
    ESP_LOGCONFIG(TAG, "DC5  : %s   Voltage:%u mV",  this->isEnableDC5()  ? "+" : "-", this->getDC5Voltage());
    ESP_LOGCONFIG(TAG, "ALDO1: %s   Voltage:%u mV",  this->isEnableALDO1()  ? "+" : "-", this->getALDO1Voltage());
    ESP_LOGCONFIG(TAG, "ALDO2: %s   Voltage:%u mV",  this->isEnableALDO2()  ? "+" : "-", this->getALDO2Voltage());
    ESP_LOGCONFIG(TAG, "ALDO3: %s   Voltage:%u mV",  this->isEnableALDO3()  ? "+" : "-", this->getALDO3Voltage());
    ESP_LOGCONFIG(TAG, "ALDO4: %s   Voltage:%u mV",  this->isEnableALDO4()  ? "+" : "-", this->getALDO4Voltage());
    ESP_LOGCONFIG(TAG, "BLDO1: %s   Voltage:%u mV",  this->isEnableBLDO1()  ? "+" : "-", this->getBLDO1Voltage());
    ESP_LOGCONFIG(TAG, "BLDO2: %s   Voltage:%u mV",  this->isEnableBLDO2()  ? "+" : "-", this->getBLDO2Voltage());
    ESP_LOGCONFIG(TAG, "CPUSLDO: %s Voltage:%u mV",  this->isEnableCPUSLDO() ? "+" : "-", this->getCPUSLDOVoltage());
    ESP_LOGCONFIG(TAG, "DLDO1: %s   Voltage:%u mV",  this->isEnableDLDO1()  ? "+" : "-", this->getDLDO1Voltage());
    ESP_LOGCONFIG(TAG, "DLDO2: %s   Voltage:%u mV",  this->isEnableDLDO2()  ? "+" : "-", this->getDLDO2Voltage());

}


/*
*  Convenient functions to operate with registers, implement with internal ESPHome method.
*/

int AXP2101::readRegister(uint8_t reg)
{
    uint8_t val;
    bool ret = this->read_byte(reg, &val);

    if (!ret) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", reg);
        return -1;
    }
    return static_cast<int>(val);
}

int AXP2101::writeRegister(uint8_t reg, uint8_t val)
{
    bool ret = this->write_byte(reg, val);
    if ( !ret ) {
         ESP_LOGE(TAG, "I2C write error at reg 0x%02X", reg);
        return -1;
    }
    return ret ? 0 : -1;
}

inline bool AXP2101::clrRegisterBit(uint8_t registers, uint8_t bit)
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

inline bool AXP2101::setRegisterBit(uint8_t registers, uint8_t bit)
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

inline bool AXP2101::getRegisterBit(uint8_t registers, uint8_t bit)
{
    uint8_t val;
    bool ret = this->read_byte(registers, &val);
    if(!ret) {
        ESP_LOGE(TAG, "I2C read error at reg 0x%02X", registers);
        return false;
    }
    return val & _BV(bit);
}

inline uint16_t AXP2101::readRegisterH6L8(uint8_t highReg, uint8_t lowReg)
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

inline uint16_t AXP2101::readRegisterH5L8(uint8_t highReg, uint8_t lowReg)
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




/*
 * PMU status functions
 */
uint16_t AXP2101::status()
{
    uint16_t status1 = readRegister(XPOWERS_AXP2101_STATUS1) & 0x1F;
    uint16_t status2 = readRegister(XPOWERS_AXP2101_STATUS2) & 0x1F;;
    return (status1 << 8) | (status2);
}

bool AXP2101::isVbusGood(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 5); }

bool AXP2101::getBatfetState(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 4); }

// getBatPresentState
bool AXP2101::isBatteryConnect(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 3); }

bool AXP2101::isBatInActiveModeState(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 2); }

bool AXP2101::getThermalRegulationStatus(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 1); }

bool AXP2101::getCurrentLimitStatus(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS1, 0); }

bool AXP2101::isCharging(void){ return (readRegister(XPOWERS_AXP2101_STATUS2) >> 5) == 0x01; }

bool AXP2101::isDischarge(void){ return (readRegister(XPOWERS_AXP2101_STATUS2) >> 5) == 0x02; }

bool AXP2101::isStandby(void){ return (readRegister(XPOWERS_AXP2101_STATUS2) >> 5) == 0x00; }

bool AXP2101::isPowerOn(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS2, 4); }

bool AXP2101::isPowerOff(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS2, 4); }

bool AXP2101::isVbusIn(void){ return getRegisterBit(XPOWERS_AXP2101_STATUS2, 3) == 0 && isVbusGood(); }

xpowers_chg_status_t AXP2101::getChargerStatus(void){
    int val = readRegister(XPOWERS_AXP2101_STATUS2);
    if (val == -1)return XPOWERS_AXP2101_CHG_STOP_STATE;
    val &= 0x07;
    return (xpowers_chg_status_t)val;
}

/*
 * PMU common configuration
 */

/**
 * @brief   Internal off-discharge enable for DCDC & LDO & SWITCH
 */

void AXP2101::enableInternalDischarge(void){ setRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 5); }

void AXP2101::disableInternalDischarge(void){ clrRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 5); }


/**
 * @brief   PWROK PIN pull low to Restart
 */
void AXP2101::enablePwrOkPinPullLow(void){ setRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 3); }

void AXP2101::disablePwrOkPinPullLow(void){ clrRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 3); }

void AXP2101::enablePwronShutPMIC(void){ setRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 2); }

void AXP2101::disablePwronShutPMIC(void){ clrRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 2); }


/**
 * @brief  Restart the SoC System, POWOFF/POWON and reset the related registers
 * @retval None
 */
void AXP2101::reset(void){ setRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 1); }

/**
 * @brief  Set shutdown, calling shutdown will turn off all power channels,
 *         only VRTC belongs to normal power supply
 * @retval None
 */
void AXP2101::shutdown(void){ setRegisterBit(XPOWERS_AXP2101_COMMON_CONFIG, 0); }

/**
 * @brief  BATFET control / REG 12H
 * @note   DIE Over Temperature Protection Level1 Configuration
 * @param  opt: 0:115 , 1:125 , 2:135
 * @retval None
 */
void AXP2101::setBatfetDieOverTempLevel1(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_BATFET_CTRL);
    if (val == -1)return;
    val &= 0xF9;
    writeRegister(XPOWERS_AXP2101_BATFET_CTRL, val | (opt << 1));
}

uint8_t AXP2101::getBatfetDieOverTempLevel1(void) { return (readRegister(XPOWERS_AXP2101_BATFET_CTRL) & 0x06); }

void AXP2101::enableBatfetDieOverTempDetect(void){ setRegisterBit(XPOWERS_AXP2101_BATFET_CTRL, 0); }

void AXP2101::disableBatfetDieOverTempDetect(void){ clrRegisterBit(XPOWERS_AXP2101_BATFET_CTRL, 0); }

/**
 * @param  opt: 0:115 , 1:125 , 2:135
 */
void AXP2101::setDieOverTempLevel1(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_DIE_TEMP_CTRL);
    if (val == -1)return;
    val &= 0xF9;
    writeRegister(XPOWERS_AXP2101_DIE_TEMP_CTRL, val | (opt << 1));
}

uint8_t AXP2101::getDieOverTempLevel1(void){ return (readRegister(XPOWERS_AXP2101_DIE_TEMP_CTRL) & 0x06); }

void AXP2101::enableDieOverTempDetect(void){ setRegisterBit(XPOWERS_AXP2101_DIE_TEMP_CTRL, 0); }

void AXP2101::disableDieOverTempDetect(void){ clrRegisterBit(XPOWERS_AXP2101_DIE_TEMP_CTRL, 0); }

// Linear Charger Vsys voltage dpm
void AXP2101::setLinearChargerVsysDpm(xpower_chg_dpm_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_MIN_SYS_VOL_CTRL);
    if (val == -1)return;
    val &= 0x8F;
    writeRegister(XPOWERS_AXP2101_MIN_SYS_VOL_CTRL, val | (opt << 4));
}

uint8_t AXP2101::getLinearChargerVsysDpm(void)
{
    int val = readRegister(XPOWERS_AXP2101_MIN_SYS_VOL_CTRL);
    if (val == -1)return 0;
    val &= 0x70;
    return (val & 0x70) >> 4;
}


/**
 * @brief  Set VBUS Voltage Input Limit.
 * @param  opt: View the related chip type xpowers_axp2101_vbus_vol_limit_t enumeration
 *              parameters in "axp2101_cost.h"
 */
void AXP2101::setVbusVoltageLimit(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_INPUT_VOL_LIMIT_CTRL);
    if (val == -1)return;
    val &= 0xF0;
    writeRegister(XPOWERS_AXP2101_INPUT_VOL_LIMIT_CTRL, val | (opt & 0x0F));
}

/**
* @brief  Get VBUS Voltage Input Limit.
* @retval View the related chip type xpowers_axp2101_vbus_vol_limit_t enumeration
*              parameters in "axp2101_cost.h"
*/
uint8_t AXP2101::getVbusVoltageLimit(void){ return ( readRegister(XPOWERS_AXP2101_INPUT_VOL_LIMIT_CTRL) & 0x0F); }

/**
* @brief  Set VBUS Current Input Limit.
* @param  opt: View the related chip type xpowers_axp2101_vbus_cur_limit_t enumeration
*              parameters in "axp2101_cost.h"
* @retval true valid false invalid
*/
bool AXP2101::setVbusCurrentLimit(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_INPUT_CUR_LIMIT_CTRL);
    if (val == -1)return false;
    val &= 0xF8;
    return 0 == writeRegister(XPOWERS_AXP2101_INPUT_CUR_LIMIT_CTRL, val | (opt & 0x07));
}

/**
* @brief  Get VBUS Current Input Limit.
* @retval View the related chip type xpowers_axp2101_vbus_cur_limit_t enumeration
*              parameters in "axp2101_cost.h"
*/
uint8_t AXP2101::getVbusCurrentLimit(void){ return (readRegister(XPOWERS_AXP2101_INPUT_CUR_LIMIT_CTRL) & 0x07); }

/**
 * @brief  Reset the fuel gauge
 */
void AXP2101::resetGauge(void){ setRegisterBit(XPOWERS_AXP2101_RESET_FUEL_GAUGE, 3); }

/**
 * @brief   reset the gauge besides reset
 */
void AXP2101::resetGaugeBesides(void){ setRegisterBit(XPOWERS_AXP2101_RESET_FUEL_GAUGE, 2); }

/**
 * @brief Gauge Module
 */
void AXP2101::enableGauge(void){ setRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 3); }

void AXP2101::disableGauge(void){ clrRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 3); }

/**
 * @brief  Button Battery charge
 */
bool AXP2101::enableButtonBatteryCharge(void){ return setRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 2); }

bool AXP2101::disableButtonBatteryCharge(void){ return clrRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 2); }

bool AXP2101::isEnableButtonBatteryCharge(){ return getRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 2); }


//Button battery charge termination voltage setting
bool AXP2101::setButtonBatteryChargeVoltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_BTN_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! Button battery charging step voltage is %u mV", XPOWERS_AXP2101_BTN_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_BTN_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! The minimum charge termination voltage of the coin cell battery is %u mV", XPOWERS_AXP2101_BTN_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_BTN_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! The minimum charge termination voltage of the coin cell battery is %u mV", XPOWERS_AXP2101_BTN_VOL_MAX);
        return false;
    }
    int val =  readRegister(XPOWERS_AXP2101_BTN_BAT_CHG_VOL_SET);
    if (val == -1)return 0;
    val  &= 0xF8;
    val |= (millivolt - XPOWERS_AXP2101_BTN_VOL_MIN) / XPOWERS_AXP2101_BTN_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_BTN_BAT_CHG_VOL_SET, val);
}

uint16_t AXP2101::getButtonBatteryVoltage(void){
    int val =  readRegister(XPOWERS_AXP2101_BTN_BAT_CHG_VOL_SET);
    if (val == -1)return 0;
    return (val & 0x07) * XPOWERS_AXP2101_BTN_VOL_STEPS + XPOWERS_AXP2101_BTN_VOL_MIN;
}


/**
 * @brief Cell Battery charge
 */
void AXP2101::enableCellbatteryCharge(void){ setRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 1); }

void AXP2101::disableCellbatteryCharge(void){ clrRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 1); }

/**
 * @brief  Watchdog Module
 */
void AXP2101::enableWatchdog(void){
    setRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 0);
    enableIRQ(XPOWERS_AXP2101_WDT_EXPIRE_IRQ);
}

void AXP2101::disableWatchdog(void)
{
    disableIRQ(XPOWERS_AXP2101_WDT_EXPIRE_IRQ);
    clrRegisterBit(XPOWERS_AXP2101_CHARGE_GAUGE_WDT_CTRL, 0);
}

/**
 * @brief Watchdog Config
 * @note
 * @param  opt: 0: IRQ Only 1: IRQ and System reset  2: IRQ, System Reset and Pull down PWROK 1s  3: IRQ, System Reset, DCDC/LDO PWROFF & PWRON
 * @retval None
 */
void AXP2101::setWatchdogConfig(xpowers_wdt_config_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_WDT_CTRL);
    if (val == -1)return;
    val &= 0xCF;
    writeRegister(XPOWERS_AXP2101_WDT_CTRL, val | (opt << 4));
}

uint8_t AXP2101::getWatchConfig(void)
{
    return (readRegister(XPOWERS_AXP2101_WDT_CTRL) & 0x30) >> 4;
}

void AXP2101::clrWatchdog(void)
{
    setRegisterBit(XPOWERS_AXP2101_WDT_CTRL, 3);
}


void AXP2101::setWatchdogTimeout(xpowers_wdt_timeout_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_WDT_CTRL);
    if (val == -1)return;
    val &= 0xF8;
    writeRegister(XPOWERS_AXP2101_WDT_CTRL, val | opt);
}

uint8_t AXP2101::getWatchdogTimerout(void){ return readRegister(XPOWERS_AXP2101_WDT_CTRL) & 0x07; }

/**
 * @brief  Low battery warning threshold 5-20%, 1% per step
 * @param  percentage:   5 ~ 20
 * @retval None
 */
void AXP2101::setLowBatWarnThreshold(uint8_t percentage)
{
    if (percentage < 5 || percentage > 20)return;
    int val = readRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET);
    if (val == -1)return;
    val &= 0x0F;
    writeRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET, val | ((percentage - 5) << 4));
}

uint8_t AXP2101::getLowBatWarnThreshold(void)
{
    int val = readRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET);
    if (val == -1)return 0;
    val &= 0xF0;
    val >>= 4;
    return val;
}

/**
 * @brief  Low battery shutdown threshold 0-15%, 1% per step
 * @param  opt:   0 ~ 15
 * @retval None
 */
void AXP2101::setLowBatShutdownThreshold(uint8_t opt)
{
    if (opt > 15) {
        opt = 15;
    }
    int val = readRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET);
    if (val == -1)return;
    val &= 0xF0;
    writeRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET, val | opt);
}

uint8_t AXP2101::getLowBatShutdownThreshold(void)
{
    return (readRegister(XPOWERS_AXP2101_LOW_BAT_WARN_SET) & 0x0F);
}

//!  PWRON statu  20
// POWERON always high when EN Mode as POWERON Source
bool AXP2101::isPoweronAlwaysHighSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 5); }

// Battery Insert and Good as POWERON Source
bool AXP2101::isBattInsertOnSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 4); }

// Battery Voltage > 3.3V when Charged as Source
bool AXP2101::isBattNormalOnSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 3); }

// Vbus Insert and Good as POWERON Source
bool AXP2101::isVbusInsertOnSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 2); }

// IRQ PIN Pull-down as POWERON Source
bool AXP2101::isIrqLowOnSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 1); }

// POWERON low for on level when POWERON Mode as POWERON Source
bool AXP2101::isPwronLowOnSource(){ return getRegisterBit(XPOWERS_AXP2101_PWRON_STATUS, 0); }

xpower_power_on_source_t AXP2101::getPowerOnSource()
{
    int val = readRegister(XPOWERS_AXP2101_PWRON_STATUS);
    if (val == -1) return XPOWER_POWERON_SRC_UNKONW;
    return (xpower_power_on_source_t)val;
}

//!  PWROFF status  21
// Die Over Temperature as POWEROFF Source
bool AXP2101::isOverTemperatureOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 7); }

// DCDC Over Voltage as POWEROFF Source
bool AXP2101::isDcOverVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 6); }

// DCDC Under Voltage as POWEROFF Source
bool AXP2101::isDcUnderVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 5); }

// VBUS Over Voltage as POWEROFF Source
bool AXP2101::isVbusOverVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 4); }

// Vsys Under Voltage as POWEROFF Source
bool AXP2101::isVsysUnderVoltageOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 3); }

// POWERON always low when EN Mode as POWEROFF Source
bool AXP2101::isPwronAlwaysLowOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 2); }

// Software configuration as POWEROFF Source
bool AXP2101::isSwConfigOffSource(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 1); }

// POWERON Pull down for off level when POWERON Mode as POWEROFF Source
bool AXP2101::isPwrSourcePullDown(){ return getRegisterBit(XPOWERS_AXP2101_PWROFF_STATUS, 0); }

xpower_power_off_source_t AXP2101::getPowerOffSource(){ 
    int val = readRegister(XPOWERS_AXP2101_PWROFF_STATUS);
    if (val == -1) return XPOWER_POWEROFF_SRC_UNKONW;
    return (xpower_power_off_source_t)val;
}

//!REG 22H
void AXP2101::enableOverTemperatureLevel2PowerOff(){ setRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 2); }

void AXP2101::disableOverTemperaturePowerOff(){ clrRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 2); }

// CHANGE:  void AXP2101::enablePwrOnOverVolOffLevelPowerOff()
void AXP2101::enableLongPressShutdown(){ setRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 1); }

// CHANGE:  void AXP2101::disablePwrOnOverVolOffLevelPowerOff()
void AXP2101::disableLongPressShutdown(){ clrRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 1); }

//CHANGE: void AXP2101::enablePwrOffSelectFunction()
void AXP2101::setLongPressRestart(){ setRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 0); }

//CHANGE: void AXP2101::disablePwrOffSelectFunction()
void AXP2101::setLongPressPowerOFF(){ clrRegisterBit(XPOWERS_AXP2101_PWROFF_EN, 0); }

//!REG 23H
// DCDC 120%(130%) high voltage turn off PMIC function
void AXP2101::enableDCHighVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 5); }

void AXP2101::disableDCHighVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 5); }

// DCDC5 85% low voltage turn Off PMIC function
void AXP2101::enableDC5LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 4); }

void AXP2101::disableDC5LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 4); }

// DCDC4 85% low voltage turn Off PMIC function
void AXP2101::enableDC4LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 3); }

void AXP2101::disableDC4LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 3); }

// DCDC3 85% low voltage turn Off PMIC function
void AXP2101::enableDC3LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 2); }

void AXP2101::disableDC3LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 2);}

// DCDC2 85% low voltage turn Off PMIC function
void AXP2101::enableDC2LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 1);}

void AXP2101::disableDC2LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 1);}

// DCDC1 85% low voltage turn Off PMIC function
void AXP2101::enableDC1LowVoltageTurnOff(){ setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 0); }

void AXP2101::disableDC1LowVoltageTurnOff(){ clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 0);}


// Set the minimum system operating voltage inside the PMU,
// below this value will shut down the PMU,Adjustment range 2600mV~3300mV
bool AXP2101::setSysPowerDownVoltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MIN) {
        ESP_LOGE(TAG, "Mistake ! The minimum settable voltage of VSYS is %u mV", XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MAX) {
        ESP_LOGE(TAG, "Mistake ! The maximum settable voltage of VSYS is %u mV", XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MAX);
        return false;
    }
    int val = readRegister(XPOWERS_AXP2101_VOFF_SET);
    if (val == -1)return false;
    val &= 0xF8;
    return 0 == writeRegister(XPOWERS_AXP2101_VOFF_SET, val | ((millivolt - XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MIN) / XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_STEPS));
}

uint16_t AXP2101::getSysPowerDownVoltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_VOFF_SET);
    if (val == -1)return false;
    return (val & 0x07) * XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_STEPS + XPOWERS_AXP2101_VSYS_VOL_THRESHOLD_MIN;
}

//  PWROK setting and PWROFF sequence control 25.
// Check the PWROK Pin enable after all dcdc/ldo output valid 128ms
void AXP2101::enablePwrOk(){ setRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 4); }

void AXP2101::disablePwrOk(){ clrRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 4); }

// POWEROFF Delay 4ms after PWROK enable
void AXP2101::enablePowerOffDelay(){ setRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 3); }

// POWEROFF Delay 4ms after PWROK disable
void AXP2101::disablePowerOffDelay(){ clrRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 3); }

// POWEROFF Sequence Control the reverse of the Startup
void AXP2101::enablePowerSequence(){ setRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 2); }

// POWEROFF Sequence Control at the same time
void AXP2101::disablePowerSequence(){ clrRegisterBit(XPOWERS_AXP2101_PWROK_SEQU_CTRL, 2);}

// Delay of PWROK after all power output good
bool AXP2101::setPwrOkDelay(xpower_pwrok_delay_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_PWROK_SEQU_CTRL);
    if (val == -1)return false;
    val &= 0xFC;
    return 0 == writeRegister(XPOWERS_AXP2101_PWROK_SEQU_CTRL, val | opt);
}

xpower_pwrok_delay_t AXP2101::getPwrOkDelay(){
    int val = readRegister(XPOWERS_AXP2101_PWROK_SEQU_CTRL);
    if (val == -1)return XPOWER_PWROK_DELAY_8MS;
    return (xpower_pwrok_delay_t)(val & 0x03);
}

//  Sleep and 26
void AXP2101::wakeupControl(xpowers_wakeup_t opt, bool enable)
{
    int val = readRegister(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL);
    if (val == -1)return;
    enable ? (val |= opt) : (val &= (~opt));
    writeRegister(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, val);
}

bool AXP2101::enableWakeup(void){ return setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 1); }

bool AXP2101::disableWakeup(void){ return clrRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 1); }

bool AXP2101::enableSleep(void){ return setRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 0); }

bool AXP2101::disableSleep(void){ return clrRegisterBit(XPOWERS_AXP2101_SLEEP_WAKEUP_CTRL, 0); }


//  RQLEVEL/OFFLEVEL/ONLEVEL setting 27
/**
 * @brief  IRQLEVEL configur
 * @param  opt: 0:1s  1:1.5s  2:2s 3:2.5s
 */
void AXP2101::setIrqLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 4));
}

/**
 * @brief  OFFLEVEL configuration
 * @param  opt:  0:4s 1:6s 2:8s 3:10s
 */
void AXP2101::setOffLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 2));
}

/**
 * @brief  ONLEVEL configuration
 * @param  opt: 0:128ms 1:512ms 2:1s  3:2s
 */
void AXP2101::setOnLevel(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | opt);
}

// Fast pwron setting 0  28
// Fast Power On Start Sequence
void AXP2101::setDc4FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | ((opt & 0x3) << 6));
}

void AXP2101::setDc3FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | ((opt & 0x3) << 4));
}
void AXP2101::setDc2FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | ((opt & 0x3) << 2));
}
void AXP2101::setDc1FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | (opt & 0x3));
}

//  Fast pwron setting 1  29
void AXP2101::setAldo3FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | ((opt & 0x3) << 6));
}
void AXP2101::setAldo2FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | ((opt & 0x3) << 4));
}
void AXP2101::setAldo1FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | ((opt & 0x3) << 2));
}

void AXP2101::setDc5FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | (opt & 0x3));
}

//  Fast pwron setting 2  2A
void AXP2101::setCpuldoFastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | ((opt & 0x3) << 6));
}

void AXP2101::setBldo2FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | ((opt & 0x3) << 4));
}

void AXP2101::setBldo1FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | ((opt & 0x3) << 2));
}

void AXP2101::setAldo4FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | (opt & 0x3));
}

//  Fast pwron setting 3  2B
void AXP2101::setDldo2FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val | ((opt & 0x3) << 2));
}

void AXP2101::setDldo1FastStartSequence(xpower_start_sequence_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val | (opt & 0x3));
}

/**
 * @brief   Setting Fast Power On Start Sequence
 */
void AXP2101::setFastPowerOnLevel(xpowers_fast_on_opt_t opt, xpower_start_sequence_t seq_level)
{
    uint8_t val = 0;
    switch (opt) {
    case XPOWERS_AXP2101_FAST_DCDC1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | seq_level);
        break;
    case XPOWERS_AXP2101_FAST_DCDC2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | (seq_level << 2));
        break;
    case XPOWERS_AXP2101_FAST_DCDC3:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | (seq_level << 4));
        break;
    case XPOWERS_AXP2101_FAST_DCDC4:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val | (seq_level << 6));
        break;
    case XPOWERS_AXP2101_FAST_DCDC5:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | seq_level);
        break;
    case XPOWERS_AXP2101_FAST_ALDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | (seq_level << 2));
        break;
    case XPOWERS_AXP2101_FAST_ALDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | (seq_level << 4));
        break;
    case XPOWERS_AXP2101_FAST_ALDO3:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val | (seq_level << 6));
        break;
    case XPOWERS_AXP2101_FAST_ALDO4:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | seq_level);
        break;
    case XPOWERS_AXP2101_FAST_BLDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | (seq_level << 2));
        break;
    case XPOWERS_AXP2101_FAST_BLDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | (seq_level << 4));
        break;
    case XPOWERS_AXP2101_FAST_CPUSLDO:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val | (seq_level << 6));
        break;
    case XPOWERS_AXP2101_FAST_DLDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val | seq_level);
        break;
    case XPOWERS_AXP2101_FAST_DLDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val | (seq_level << 2));
        break;
    default:
        break;
    }
}

void AXP2101::disableFastPowerOn(xpowers_fast_on_opt_t opt)
{
    uint8_t val = 0;
    switch (opt) {
    case XPOWERS_AXP2101_FAST_DCDC1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val & 0xFC);
        break;
    case XPOWERS_AXP2101_FAST_DCDC2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val & 0xF3);
        break;
    case XPOWERS_AXP2101_FAST_DCDC3:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val & 0xCF);
        break;
    case XPOWERS_AXP2101_FAST_DCDC4:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET0);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET0, val & 0x3F);
        break;
    case XPOWERS_AXP2101_FAST_DCDC5:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val & 0xFC);
        break;
    case XPOWERS_AXP2101_FAST_ALDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val & 0xF3);
        break;
    case XPOWERS_AXP2101_FAST_ALDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val & 0xCF);
        break;
    case XPOWERS_AXP2101_FAST_ALDO3:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET1);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET1, val & 0x3F);
        break;
    case XPOWERS_AXP2101_FAST_ALDO4:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val & 0xFC);
        break;
    case XPOWERS_AXP2101_FAST_BLDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val & 0xF3);
        break;
    case XPOWERS_AXP2101_FAST_BLDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val & 0xCF);
        break;
    case XPOWERS_AXP2101_FAST_CPUSLDO:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_SET2);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_SET2, val & 0x3F);
        break;
    case XPOWERS_AXP2101_FAST_DLDO1:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val & 0xFC);
        break;
    case XPOWERS_AXP2101_FAST_DLDO2:
        val = readRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL);
        writeRegister(XPOWERS_AXP2101_FAST_PWRON_CTRL, val & 0xF3);
        break;
    default:
        break;
    }
}

void AXP2101::enableFastPowerOn(void){ setRegisterBit(XPOWERS_AXP2101_FAST_PWRON_CTRL, 7); }

void AXP2101::disableFastPowerOn(void){ clrRegisterBit(XPOWERS_AXP2101_FAST_PWRON_CTRL, 7); }

void AXP2101::enableFastWakeup(void){ setRegisterBit(XPOWERS_AXP2101_FAST_PWRON_CTRL, 6); }

void AXP2101::disableFastWakeup(void){ clrRegisterBit(XPOWERS_AXP2101_FAST_PWRON_CTRL, 6); }

// DCDC 120%(130%) high voltage turn off PMIC function
void AXP2101::setDCHighVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 5) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 5);
}

bool AXP2101::getDCHighVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 5); }

// DCDCS force PWM control
void AXP2101::setDcUVPDebounceTime(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL);
    val &= 0xFC;
    writeRegister(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, val | opt);
}

void AXP2101::settDC1WorkModeToPwm(uint8_t enable)
{
    enable ?
    setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 2)
    : clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 2);
}

void AXP2101::settDC2WorkModeToPwm(uint8_t enable)
{
    enable ? setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 3)
    : clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 3);
}

void AXP2101::settDC3WorkModeToPwm(uint8_t enable)
{
    enable ?
    setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 4)
    : clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 4);
}

void AXP2101::settDC4WorkModeToPwm( uint8_t enable)
{
    enable ?
    setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 5)
    :  clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 5);
}

//1 = 100khz 0=50khz
void AXP2101::setDCFreqSpreadRange(uint8_t opt)
{
    opt ?
    setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 6)
    :  clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 6);
}

void AXP2101::setDCFreqSpreadRangeEn(bool en)
{
    en ?
    setRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 7)
    :  clrRegisterBit(XPOWERS_AXP2101_DC_FORCE_PWM_CTRL, 7);
}

void AXP2101::enableCCM()
{
    setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 6);
}

void AXP2101::disableCCM()
{
    clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 6);
}

bool AXP2101::isenableCCM()
{
    return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 6);
}

enum DVMRamp {
    XPOWERS_AXP2101_DVM_RAMP_15_625US,
    XPOWERS_AXP2101_DVM_RAMP_31_250US,
};

//args:enum DVMRamp
void AXP2101::setDVMRamp(uint8_t opt)
{
    if (opt > 2)return;
    opt == 0 ? clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 5) : setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 5);
}



/*
 * Power control DCDC1 functions
 */
bool AXP2101::isEnableDC1(void){ return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 0); }

bool AXP2101::enableDC1(void){ return setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 0); }

bool AXP2101::disableDC1(void){ return clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 0); }

bool AXP2101::setDC1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_DCDC1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_DCDC1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_DCDC1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DC1 minimum voltage is %u mV", XPOWERS_AXP2101_DCDC1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_DCDC1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DC1 maximum voltage is %u mV", XPOWERS_AXP2101_DCDC1_VOL_MAX);
        return false;
    }
    return 0 == writeRegister(XPOWERS_AXP2101_DC_VOL0_CTRL, (millivolt - XPOWERS_AXP2101_DCDC1_VOL_MIN) / XPOWERS_AXP2101_DCDC1_VOL_STEPS);
}

uint16_t AXP2101::getDC1Voltage(void)
{
    return (readRegister(XPOWERS_AXP2101_DC_VOL0_CTRL) & 0x1F) * XPOWERS_AXP2101_DCDC1_VOL_STEPS + XPOWERS_AXP2101_DCDC1_VOL_MIN;
}



// DCDC1 85% low voltage turn off PMIC function
void AXP2101::setDC1LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 0) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 0);
}

bool AXP2101::getDC1LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 0); }

/*
 * Power control DCDC2 functions
 */
bool AXP2101::isEnableDC2(void){ return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 1); }

bool AXP2101::enableDC2(void){ return setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 1); }

bool AXP2101::disableDC2(void){ return clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 1); }

bool AXP2101::setDC2Voltage(uint16_t millivolt)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL1_CTRL);
    if (val == -1)return 0;
    val &= 0x80;
    if (millivolt >= XPOWERS_AXP2101_DCDC2_VOL1_MIN && millivolt <= XPOWERS_AXP2101_DCDC2_VOL1_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC2_VOL_STEPS1) {
            ESP_LOGE(TAG, "Mistake !  The steps must be %umV", XPOWERS_AXP2101_DCDC2_VOL_STEPS1);
            return false;
        }
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL1_CTRL, val | (millivolt - XPOWERS_AXP2101_DCDC2_VOL1_MIN) / XPOWERS_AXP2101_DCDC2_VOL_STEPS1);
    } else if (millivolt >= XPOWERS_AXP2101_DCDC2_VOL2_MIN && millivolt <= XPOWERS_AXP2101_DCDC2_VOL2_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC2_VOL_STEPS2) {
            ESP_LOGE(TAG, "Mistake !  The steps must be %umV", XPOWERS_AXP2101_DCDC2_VOL_STEPS2);
            return false;
        }
        val |= (((millivolt - XPOWERS_AXP2101_DCDC2_VOL2_MIN) / XPOWERS_AXP2101_DCDC2_VOL_STEPS2) + XPOWERS_AXP2101_DCDC2_VOL_STEPS2_BASE);
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL1_CTRL, val);
    }
    return false;
}

uint16_t AXP2101::getDC2Voltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL1_CTRL);
    if (val ==  -1)return 0;
    val &= 0x7F;
    if (val < XPOWERS_AXP2101_DCDC2_VOL_STEPS2_BASE) {
        return (val  * XPOWERS_AXP2101_DCDC2_VOL_STEPS1) +  XPOWERS_AXP2101_DCDC2_VOL1_MIN;
    } else  {
        return (val  * XPOWERS_AXP2101_DCDC2_VOL_STEPS2) - 200;
    }
    return 0;
}

uint8_t AXP2101::getDC2WorkMode(void){ return getRegisterBit(XPOWERS_AXP2101_DCDC2_VOL_STEPS2, 7); }

void AXP2101::setDC2LowVoltagePowerDown(bool en){
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 1) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 1);
}

bool AXP2101::getDC2LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 1); }

/*
 * Power control DCDC3 functions
 */

bool AXP2101::isEnableDC3(void){ return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 2); }

bool AXP2101::enableDC3(void){ return setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 2); }

bool AXP2101::disableDC3(void){ return clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 2); }

/**
    0.5~1.2V,10mV/step,71steps
    1.22~1.54V,20mV/step,17steps
    1.6~3.4V,100mV/step,19steps
 */
bool AXP2101::setDC3Voltage(uint16_t millivolt)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL2_CTRL);
    if (val == -1)return false;
    val &= 0x80;
    if (millivolt >= XPOWERS_AXP2101_DCDC3_VOL1_MIN && millivolt <= XPOWERS_AXP2101_DCDC3_VOL1_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC3_VOL_STEPS1) {
            ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP2101_DCDC3_VOL_STEPS1);
            return false;
        }
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL2_CTRL, val | (millivolt - XPOWERS_AXP2101_DCDC3_VOL_MIN) / XPOWERS_AXP2101_DCDC3_VOL_STEPS1);
    } else if (millivolt >= XPOWERS_AXP2101_DCDC3_VOL2_MIN && millivolt <= XPOWERS_AXP2101_DCDC3_VOL2_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC3_VOL_STEPS2) {
            ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP2101_DCDC3_VOL_STEPS2);
            return false;
        }
        val |= (((millivolt - XPOWERS_AXP2101_DCDC3_VOL2_MIN) / XPOWERS_AXP2101_DCDC3_VOL_STEPS2) + XPOWERS_AXP2101_DCDC3_VOL_STEPS2_BASE);
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL2_CTRL, val);
    } else if (millivolt >= XPOWERS_AXP2101_DCDC3_VOL3_MIN && millivolt <= XPOWERS_AXP2101_DCDC3_VOL3_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC3_VOL_STEPS3) {
            ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP2101_DCDC3_VOL_STEPS3);
            return false;
        }
        val |= (((millivolt - XPOWERS_AXP2101_DCDC3_VOL3_MIN) / XPOWERS_AXP2101_DCDC3_VOL_STEPS3) + XPOWERS_AXP2101_DCDC3_VOL_STEPS3_BASE);
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL2_CTRL, val);
    }
    return false;
}


uint16_t AXP2101::getDC3Voltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL2_CTRL) & 0x7F;
    if (val == -1)
        return 0;
    if (val < XPOWERS_AXP2101_DCDC3_VOL_STEPS2_BASE) {
        return (val  * XPOWERS_AXP2101_DCDC3_VOL_STEPS1) +  XPOWERS_AXP2101_DCDC3_VOL_MIN;
    } else if (val >= XPOWERS_AXP2101_DCDC3_VOL_STEPS2_BASE && val < XPOWERS_AXP2101_DCDC3_VOL_STEPS3_BASE) {
        return (val  * XPOWERS_AXP2101_DCDC3_VOL_STEPS2) - 200;
    } else  {
        return (val  * XPOWERS_AXP2101_DCDC3_VOL_STEPS3)  - 7200;
    }
    return 0;
}

uint8_t AXP2101::getDC3WorkMode(void){ return getRegisterBit(XPOWERS_AXP2101_DC_VOL2_CTRL, 7); }

// DCDC3 85% low voltage turn off PMIC function
void AXP2101::setDC3LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 2) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 2);
}

bool AXP2101::getDC3LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 2); }


/*
* Power control DCDC4 functions
*/
/**
    0.5~1.2V,10mV/step,71steps
    1.22~1.84V,20mV/step,32steps
 */
bool AXP2101::isEnableDC4(void){ return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 3); }

bool AXP2101::enableDC4(void){ return setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 3); }

bool AXP2101::disableDC4(void){ return clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 3); }

bool AXP2101::setDC4Voltage(uint16_t millivolt)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL3_CTRL);
    if (val == -1)return false;
    val &= 0x80;
    if (millivolt >= XPOWERS_AXP2101_DCDC4_VOL1_MIN && millivolt <= XPOWERS_AXP2101_DCDC4_VOL1_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC4_VOL_STEPS1) {
            ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP2101_DCDC4_VOL_STEPS1);
            return false;
        }
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL3_CTRL, val | (millivolt - XPOWERS_AXP2101_DCDC4_VOL1_MIN) / XPOWERS_AXP2101_DCDC4_VOL_STEPS1);

    } else if (millivolt >= XPOWERS_AXP2101_DCDC4_VOL2_MIN && millivolt <= XPOWERS_AXP2101_DCDC4_VOL2_MAX) {
        if (millivolt % XPOWERS_AXP2101_DCDC4_VOL_STEPS2) {
            ESP_LOGE(TAG, "Mistake ! The steps must be %umV", XPOWERS_AXP2101_DCDC4_VOL_STEPS2);
            return false;
        }
        val |= (((millivolt - XPOWERS_AXP2101_DCDC4_VOL2_MIN) / XPOWERS_AXP2101_DCDC4_VOL_STEPS2) + XPOWERS_AXP2101_DCDC4_VOL_STEPS2_BASE);
        return  0 == writeRegister(XPOWERS_AXP2101_DC_VOL3_CTRL, val);

    }
    return false;
}

uint16_t AXP2101::getDC4Voltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL3_CTRL);
    if (val == -1)return 0;
    val &= 0x7F;
    if (val < XPOWERS_AXP2101_DCDC4_VOL_STEPS2_BASE) {
        return (val  * XPOWERS_AXP2101_DCDC4_VOL_STEPS1) +  XPOWERS_AXP2101_DCDC4_VOL1_MIN;
    } else  {
        return (val  * XPOWERS_AXP2101_DCDC4_VOL_STEPS2) - 200;
    }
    return 0;
}

// DCDC4 85% low voltage turn off PMIC function
void AXP2101::setDC4LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 3) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 3);
}

bool AXP2101::getDC4LowVoltagePowerDownEn()
{
    return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 3);
}

/*
* Power control DCDC5 functions,Output to gpio pin
*/
bool AXP2101::isEnableDC5(void){ return getRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 4); }

bool AXP2101::enableDC5(void){ return setRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 4); }

bool AXP2101::disableDC5(void){ return clrRegisterBit(XPOWERS_AXP2101_DC_ONOFF_DVM_CTRL, 4); }

bool AXP2101::setDC5Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_DCDC5_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_DCDC5_VOL_STEPS);
        return false;
    }
    if (millivolt != XPOWERS_AXP2101_DCDC5_VOL_1200MV && millivolt < XPOWERS_AXP2101_DCDC5_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DC5 minimum voltage is %umV ,%umV", XPOWERS_AXP2101_DCDC5_VOL_1200MV, XPOWERS_AXP2101_DCDC5_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_DCDC5_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DC5 maximum voltage is %umV", XPOWERS_AXP2101_DCDC5_VOL_MAX);
        return false;
    }

    int val =  readRegister(XPOWERS_AXP2101_DC_VOL4_CTRL);
    if (val == -1)return false;
    val &= 0xE0;
    if (millivolt == XPOWERS_AXP2101_DCDC5_VOL_1200MV) {
        return 0 == writeRegister(XPOWERS_AXP2101_DC_VOL4_CTRL, val | XPOWERS_AXP2101_DCDC5_VOL_VAL);
    }
    val |= (millivolt - XPOWERS_AXP2101_DCDC5_VOL_MIN) / XPOWERS_AXP2101_DCDC5_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_DC_VOL4_CTRL, val);
}

uint16_t AXP2101::getDC5Voltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_DC_VOL4_CTRL) ;
    if (val == -1)return 0;
    val &= 0x1F;
    if (val == XPOWERS_AXP2101_DCDC5_VOL_VAL)return XPOWERS_AXP2101_DCDC5_VOL_1200MV;
    return  (val * XPOWERS_AXP2101_DCDC5_VOL_STEPS) + XPOWERS_AXP2101_DCDC5_VOL_MIN;
}

bool AXP2101::isDC5FreqCompensationEn(void){ return getRegisterBit(XPOWERS_AXP2101_DC_VOL4_CTRL, 5); }

void AXP2101::enableDC5FreqCompensation(){ setRegisterBit(XPOWERS_AXP2101_DC_VOL4_CTRL, 5); }

void AXP2101::disableFreqCompensation(){ clrRegisterBit(XPOWERS_AXP2101_DC_VOL4_CTRL, 5); }

// DCDC4 85% low voltage turn off PMIC function
void AXP2101::setDC5LowVoltagePowerDown(bool en)
{
    en ? setRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 4) : clrRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 4);
}

bool AXP2101::getDC5LowVoltagePowerDownEn(){ return getRegisterBit(XPOWERS_AXP2101_DC_OVP_UVP_CTRL, 4); }

/*
* Power control ALDO1 functions
*/
bool AXP2101::isEnableALDO1(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 0); }

bool AXP2101::enableALDO1(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 0); }

bool AXP2101::disableALDO1(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 0); }

bool AXP2101::setALDO1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_ALDO1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_ALDO1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_ALDO1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO1 minimum output voltage is  %umV", XPOWERS_AXP2101_ALDO1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_ALDO1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO1 maximum output voltage is  %umV", XPOWERS_AXP2101_ALDO1_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL0_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_ALDO1_VOL_MIN) / XPOWERS_AXP2101_ALDO1_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL0_CTRL, val);
}

uint16_t AXP2101::getALDO1Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL0_CTRL) & 0x1F;
    return val * XPOWERS_AXP2101_ALDO1_VOL_STEPS + XPOWERS_AXP2101_ALDO1_VOL_MIN;
}

/*
* Power control ALDO2 functions
*/
bool AXP2101::isEnableALDO2(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 1); }

bool AXP2101::enableALDO2(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 1); }

bool AXP2101::disableALDO2(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 1); }

bool AXP2101::setALDO2Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_ALDO2_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_ALDO2_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_ALDO2_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO2 minimum output voltage is  %umV", XPOWERS_AXP2101_ALDO2_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_ALDO2_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO2 maximum output voltage is  %umV", XPOWERS_AXP2101_ALDO2_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL1_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_ALDO2_VOL_MIN) / XPOWERS_AXP2101_ALDO2_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL1_CTRL, val);
}

uint16_t AXP2101::getALDO2Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL1_CTRL) & 0x1F;
    return val * XPOWERS_AXP2101_ALDO2_VOL_STEPS + XPOWERS_AXP2101_ALDO2_VOL_MIN;
}

/*
 * Power control ALDO3 functions
 */
bool AXP2101::isEnableALDO3(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 2); }

bool AXP2101::enableALDO3(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 2); }

bool AXP2101::disableALDO3(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 2); }

bool AXP2101::setALDO3Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_ALDO3_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_ALDO3_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_ALDO3_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO3 minimum output voltage is  %umV", XPOWERS_AXP2101_ALDO3_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_ALDO3_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO3 maximum output voltage is  %umV", XPOWERS_AXP2101_ALDO3_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL2_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_ALDO3_VOL_MIN) / XPOWERS_AXP2101_ALDO3_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL2_CTRL, val);
}

uint16_t AXP2101::getALDO3Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL2_CTRL) & 0x1F;
    return val * XPOWERS_AXP2101_ALDO3_VOL_STEPS + XPOWERS_AXP2101_ALDO3_VOL_MIN;
}

/*
 * Power control ALDO4 functions
 */
bool AXP2101::isEnableALDO4(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 3); }

bool AXP2101::enableALDO4(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 3); }

bool AXP2101::disableALDO4(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 3); }

bool AXP2101::setALDO4Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_ALDO4_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_ALDO4_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_ALDO4_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! ALDO4 minimum output voltage is  %umV", XPOWERS_AXP2101_ALDO4_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_ALDO4_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! ALDO4 maximum output voltage is  %umV", XPOWERS_AXP2101_ALDO4_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL3_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_ALDO4_VOL_MIN) / XPOWERS_AXP2101_ALDO4_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL3_CTRL, val);
}

uint16_t AXP2101::getALDO4Voltage(void)
{
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL3_CTRL) & 0x1F;
    return val * XPOWERS_AXP2101_ALDO4_VOL_STEPS + XPOWERS_AXP2101_ALDO4_VOL_MIN;
}

/*
* Power control BLDO1 functions
*/
bool AXP2101::isEnableBLDO1(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 4); }

bool AXP2101::enableBLDO1(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 4); }

bool AXP2101::disableBLDO1(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 4); }

bool AXP2101::setBLDO1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_BLDO1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_BLDO1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_BLDO1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! BLDO1 minimum output voltage is  %umV", XPOWERS_AXP2101_BLDO1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_BLDO1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! BLDO1 maximum output voltage is  %umV", XPOWERS_AXP2101_BLDO1_VOL_MAX);
        return false;
    }
    int val =  readRegister(XPOWERS_AXP2101_LDO_VOL4_CTRL);
    if (val == -1)return  false;
    val &= 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_BLDO1_VOL_MIN) / XPOWERS_AXP2101_BLDO1_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL4_CTRL, val);
}

uint16_t AXP2101::getBLDO1Voltage(void)
{
    int val =  readRegister(XPOWERS_AXP2101_LDO_VOL4_CTRL);
    if (val == -1)return 0;
    val &= 0x1F;
    return val * XPOWERS_AXP2101_BLDO1_VOL_STEPS + XPOWERS_AXP2101_BLDO1_VOL_MIN;
}

/*
* Power control BLDO2 functions
*/
bool AXP2101::isEnableBLDO2(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 5); }

bool AXP2101::enableBLDO2(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 5); }

bool AXP2101::disableBLDO2(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 5); }

bool AXP2101::setBLDO2Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_BLDO2_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_BLDO2_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_BLDO2_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! BLDO2 minimum output voltage is  %umV", XPOWERS_AXP2101_BLDO2_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_BLDO2_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! BLDO2 maximum output voltage is  %umV", XPOWERS_AXP2101_BLDO2_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL5_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_BLDO2_VOL_MIN) / XPOWERS_AXP2101_BLDO2_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL5_CTRL, val);
}

uint16_t AXP2101::getBLDO2Voltage(void)
{
    int val =  readRegister(XPOWERS_AXP2101_LDO_VOL5_CTRL);
    if (val == -1)return 0;
    val &= 0x1F;
    return val * XPOWERS_AXP2101_BLDO2_VOL_STEPS + XPOWERS_AXP2101_BLDO2_VOL_MIN;
}

/*
* Power control CPUSLDO functions
*/
bool AXP2101::isEnableCPUSLDO(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 6); }

bool AXP2101::enableCPUSLDO(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 6); }

bool AXP2101::disableCPUSLDO(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 6); }

bool AXP2101::setCPUSLDOVoltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_CPUSLDO_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_CPUSLDO_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_CPUSLDO_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! CPULDO minimum output voltage is  %umV", XPOWERS_AXP2101_CPUSLDO_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_CPUSLDO_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! CPULDO maximum output voltage is  %umV", XPOWERS_AXP2101_CPUSLDO_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL6_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_CPUSLDO_VOL_MIN) / XPOWERS_AXP2101_CPUSLDO_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL6_CTRL, val);
}

uint16_t AXP2101::getCPUSLDOVoltage(void)
{
    int val =  readRegister(XPOWERS_AXP2101_LDO_VOL6_CTRL);
    if (val == -1)return 0;
    val &= 0x1F;
    return val * XPOWERS_AXP2101_CPUSLDO_VOL_STEPS + XPOWERS_AXP2101_CPUSLDO_VOL_MIN;
}


/*
* Power control DLDO1 functions
*/
bool AXP2101::isEnableDLDO1(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 7); }

bool AXP2101::enableDLDO1(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 7); }

bool AXP2101::disableDLDO1(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL0, 7); }

bool AXP2101::setDLDO1Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_DLDO1_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_DLDO1_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_DLDO1_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DLDO1 minimum output voltage is  %umV", XPOWERS_AXP2101_DLDO1_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_DLDO1_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DLDO1 maximum output voltage is  %umV", XPOWERS_AXP2101_DLDO1_VOL_MAX);
        return false;
    }
    uint16_t val = readRegister(XPOWERS_AXP2101_LDO_VOL7_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_DLDO1_VOL_MIN) / XPOWERS_AXP2101_DLDO1_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL7_CTRL, val);
}

uint16_t AXP2101::getDLDO1Voltage(void)
{
    int val = readRegister(XPOWERS_AXP2101_LDO_VOL7_CTRL);
    if (val == -1)return 0;
    val &= 0x1F;
    return val * XPOWERS_AXP2101_DLDO1_VOL_STEPS + XPOWERS_AXP2101_DLDO1_VOL_MIN;
}

/*
* Power control DLDO2 functions
*/
bool AXP2101::isEnableDLDO2(void){ return getRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL1, 0); }

bool AXP2101::enableDLDO2(void){ return setRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL1, 0); }

bool AXP2101::disableDLDO2(void){ return clrRegisterBit(XPOWERS_AXP2101_LDO_ONOFF_CTRL1, 0); }

bool AXP2101::setDLDO2Voltage(uint16_t millivolt)
{
    if (millivolt % XPOWERS_AXP2101_DLDO2_VOL_STEPS) {
        ESP_LOGE(TAG, "Mistake ! The steps must be %u mV", XPOWERS_AXP2101_DLDO2_VOL_STEPS);
        return false;
    }
    if (millivolt < XPOWERS_AXP2101_DLDO2_VOL_MIN) {
        ESP_LOGE(TAG, "Mistake ! DLDO2 minimum output voltage is  %umV", XPOWERS_AXP2101_DLDO2_VOL_MIN);
        return false;
    } else if (millivolt > XPOWERS_AXP2101_DLDO2_VOL_MAX) {
        ESP_LOGE(TAG, "Mistake ! DLDO2 maximum output voltage is  %umV", XPOWERS_AXP2101_DLDO2_VOL_MAX);
        return false;
    }
    uint16_t val =  readRegister(XPOWERS_AXP2101_LDO_VOL8_CTRL) & 0xE0;
    val |= (millivolt - XPOWERS_AXP2101_DLDO2_VOL_MIN) / XPOWERS_AXP2101_DLDO2_VOL_STEPS;
    return 0 == writeRegister(XPOWERS_AXP2101_LDO_VOL8_CTRL, val);
}

uint16_t AXP2101::getDLDO2Voltage(void)
{
    int val =  readRegister(XPOWERS_AXP2101_LDO_VOL8_CTRL);
    if (val == -1)return 0;
    val &= 0x1F;
    return val * XPOWERS_AXP2101_DLDO2_VOL_STEPS + XPOWERS_AXP2101_DLDO2_VOL_MIN;
}


/*
 * Power ON OFF IRQ TIMMING Control method
 */

void AXP2101::setIrqLevelTime(xpowers_irq_time_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return;
    val &= 0xCF;
    writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 4));
}

xpowers_irq_time_t AXP2101::getIrqLevelTime(void)
{
    return (xpowers_irq_time_t)((readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL) & 0x30) >> 4);
}

/**
* @brief Set the PEKEY power-on long press time.
* @param opt: See xpowers_press_on_time_t enum for details.
* @retval
*/
bool AXP2101::setPowerKeyPressOnTime(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return false;
    val  &= 0xFC;
    return 0 ==  writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | opt);
}

/**
* @brief Get the PEKEY power-on long press time.
* @retval See xpowers_press_on_time_t enum for details.
*/
uint8_t AXP2101::getPowerKeyPressOnTime(void)
{
    int val =  readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return 0;
    return (val & 0x03) ;
}

/**
* @brief Set the PEKEY power-off long press time.
* @param opt: See xpowers_press_off_time_t enum for details.
* @retval
*/
bool AXP2101::setPowerKeyPressOffTime(uint8_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL);
    if (val == -1)return false;
    val  &= 0xF3;
    return 0 == writeRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL, val | (opt << 2));
}

/**
* @brief Get the PEKEY power-off long press time.
* @retval See xpowers_press_off_time_t enum for details.
*/
uint8_t AXP2101::getPowerKeyPressOffTime(void)
{
    return ((readRegister(XPOWERS_AXP2101_IRQ_OFF_ON_LEVEL_CTRL) & 0x0C) >> 2);
}

/*
 * ADC Control method
 */
bool AXP2101::enableGeneralAdcChannel(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 5); }

bool AXP2101::disableGeneralAdcChannel(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 5); }

bool AXP2101::enableTemperatureMeasure(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 4); }

bool AXP2101::disableTemperatureMeasure(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 4); }

float AXP2101::getTemperature(void)
{
    uint16_t raw = readRegisterH6L8(XPOWERS_AXP2101_ADC_DATA_RELUST8, XPOWERS_AXP2101_ADC_DATA_RELUST9);
    return XPOWERS_AXP2101_CONVERSION(raw);
}

bool AXP2101::enableSystemVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 3); }

bool AXP2101::disableSystemVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 3); }

uint16_t AXP2101::getSystemVoltage(void)
{
    return readRegisterH6L8(XPOWERS_AXP2101_ADC_DATA_RELUST6, XPOWERS_AXP2101_ADC_DATA_RELUST7);
}

bool AXP2101::enableVbusVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 2); }

bool AXP2101::disableVbusVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 2); }

uint16_t AXP2101::getVbusVoltage(void)
{
    if (!isVbusIn()) {
        return 0;
    }
    return readRegisterH6L8(XPOWERS_AXP2101_ADC_DATA_RELUST4, XPOWERS_AXP2101_ADC_DATA_RELUST5);
}

bool AXP2101::enableTSPinMeasure(void)
{
    // TS pin is the battery temperature sensor input and will affect the charger
    uint8_t value =  readRegister(XPOWERS_AXP2101_TS_PIN_CTRL);
    value &= 0xE0;
    writeRegister(XPOWERS_AXP2101_TS_PIN_CTRL, value | 0x07);
    return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 1);
}

bool AXP2101::disableTSPinMeasure(void)
{
    // TS pin is the external fixed input and doesn't affect the charger
    uint8_t value =  readRegister(XPOWERS_AXP2101_TS_PIN_CTRL);
    value &= 0xF0;
    writeRegister(XPOWERS_AXP2101_TS_PIN_CTRL, value | 0x10);
    return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 1);
}


bool AXP2101::enableTSPinLowFreqSample(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 7); }

bool AXP2101::disableTSPinLowFreqSample(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_DATA_RELUST2, 7); }


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
 * The calculation parameters are from the AXP2101 Datasheet, using the TH11-3H103F NTC resistor
 *     as the Steinhart-Hart equation calculation parameters
 * 1. Coefficients A, B, C should be calibrated for specific NTC thermistor.
 * 2. ADC value 0x2000 indicates sensor fault (e.g., open circuit).
 * 3. Valid temperature range: typically -20℃ to 60℃. Accuracy may degrade outside this range.
 */
float AXP2101::getTsTemperature(float SteinhartA,
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
uint16_t AXP2101::getTsPinValue(void)
{
    return readRegisterH6L8(XPOWERS_AXP2101_ADC_DATA_RELUST2, XPOWERS_AXP2101_ADC_DATA_RELUST3);
}

bool AXP2101::enableBattVoltageMeasure(void){ return setRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 0); }

bool AXP2101::disableBattVoltageMeasure(void){ return clrRegisterBit(XPOWERS_AXP2101_ADC_CHANNEL_CTRL, 0); }

bool AXP2101::enableBattDetection(void){ return setRegisterBit(XPOWERS_AXP2101_BAT_DET_CTRL, 0); }

bool AXP2101::disableBattDetection(void){ return clrRegisterBit(XPOWERS_AXP2101_BAT_DET_CTRL, 0); }

uint16_t AXP2101::getBattVoltage(void)
{
    if (!isBatteryConnect()) {
        return 0;
    }
    return readRegisterH5L8(XPOWERS_AXP2101_ADC_DATA_RELUST0, XPOWERS_AXP2101_ADC_DATA_RELUST1);
}

int AXP2101::getBatteryPercent(void)
{
    if (!isBatteryConnect()) {
        return -1;
    }
    return readRegister(XPOWERS_AXP2101_BAT_PERCENT_DATA);
}

/*
* CHG LED setting and control
*/
// void AXP2101::enableChargingLed(void)
// {
//     setRegisterBit(XPOWERS_AXP2101_CHGLED_SET_CTRL, 0);
// }

// void AXP2101::disableChargingLed(void)
// {
//     clrRegisterBit(XPOWERS_AXP2101_CHGLED_SET_CTRL, 0);
// }

/**
* @brief Set charging led mode.
* @retval See xpowers_chg_led_mode_t enum for details.
*/
void AXP2101::setChargingLedMode(uint8_t mode)
{
    int val;
    switch (mode) {
    case XPOWERS_CHG_LED_OFF:
    // clrRegisterBit(XPOWERS_AXP2101_CHGLED_SET_CTRL, 0);
    // break;
    case XPOWERS_CHG_LED_BLINK_1HZ:
    case XPOWERS_CHG_LED_BLINK_4HZ:
    case XPOWERS_CHG_LED_ON:
        val = readRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL);
        if (val == -1)return;
        val &= 0xC8;
        val |= 0x05;    //use manual ctrl
        val |= (mode << 4);
        writeRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL, val);
        break;
    case XPOWERS_CHG_LED_CTRL_CHG:
        val = readRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL);
        if (val == -1)return;
        val &= 0xF9;
        writeRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL, val | 0x01); // use type A mode
        // writeRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL, val | 0x02); // use type B mode
        break;
    default:
        break;
    }
}

uint8_t AXP2101::getChargingLedMode()
{
    int val = readRegister(XPOWERS_AXP2101_CHGLED_SET_CTRL);
    if (val == -1)return XPOWERS_CHG_LED_OFF;
    val >>= 1;
    if ((val & 0x02) == 0x02) {
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
void AXP2101::setPrechargeCurr(xpowers_prechg_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_IPRECHG_SET);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP2101_IPRECHG_SET, val | opt);
}

xpowers_prechg_t AXP2101::getPrechargeCurr(void)
{
    return (xpowers_prechg_t)(readRegister(XPOWERS_AXP2101_IPRECHG_SET) & 0x03);
}


/**
* @brief Set charge current.
* @param  opt: See xpowers_axp2101_chg_curr_t enum for details.
* @retval
*/
bool AXP2101::setChargerConstantCurr(uint8_t opt)
{
    if (opt > XPOWERS_AXP2101_CHG_CUR_1000MA)return false;
    int val = readRegister(XPOWERS_AXP2101_ICC_CHG_SET);
    if (val == -1)return false;
    val &= 0xE0;
    return 0 == writeRegister(XPOWERS_AXP2101_ICC_CHG_SET, val | opt);
}

/**
 * @brief Get charge current settings.
*  @retval See xpowers_axp2101_chg_curr_t enum for details.
 */
uint8_t AXP2101::getChargerConstantCurr(void)
{
    int val = readRegister(XPOWERS_AXP2101_ICC_CHG_SET);
    if (val == -1)return 0;
    return val & 0x1F;
}

/**
 * @brief  Charging termination current
 * @note   Charging termination of current limit
 * @retval
 */
void AXP2101::setChargerTerminationCurr(xpowers_axp2101_chg_iterm_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    val &= 0xF0;
    writeRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL, val | opt);
}

xpowers_axp2101_chg_iterm_t AXP2101::getChargerTerminationCurr(void)
{
    return (xpowers_axp2101_chg_iterm_t)(readRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL) & 0x0F);
}

void AXP2101::enableChargerTerminationLimit(void)
{
    int val = readRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL, val | 0x10);
}

void AXP2101::disableChargerTerminationLimit(void)
{
    int val = readRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL);
    if (val == -1)return;
    writeRegister(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL, val & 0xEF);
}

bool AXP2101::isChargerTerminationLimit(void){ return getRegisterBit(XPOWERS_AXP2101_ITERM_CHG_SET_CTRL, 4); }


/**
* @brief Set charge target voltage.
* @param  opt: See xpowers_axp2101_chg_vol_t enum for details.
* @retval
*/
bool AXP2101::setChargeTargetVoltage(uint8_t opt)
{
    if (opt >= XPOWERS_AXP2101_CHG_VOL_MAX)return false;
    int val = readRegister(XPOWERS_AXP2101_CV_CHG_VOL_SET);
    if (val == -1)return false;
    val &= 0xF8;
    return 0 == writeRegister(XPOWERS_AXP2101_CV_CHG_VOL_SET, val | opt);
}

/**
 * @brief Get charge target voltage settings.
 * @retval See xpowers_axp2101_chg_vol_t enum for details.
 */
uint8_t AXP2101::getChargeTargetVoltage(void){ return (readRegister(XPOWERS_AXP2101_CV_CHG_VOL_SET) & 0x07); }


/**
 * @brief  Thermal regulation threshold setting
 * @note   Thermal regulation threshold setting
 */
void AXP2101::setThermaThreshold(xpowers_thermal_t opt)
{
    int val = readRegister(XPOWERS_AXP2101_THE_REGU_THRES_SET);
    if (val == -1)return;
    val &= 0xFC;
    writeRegister(XPOWERS_AXP2101_THE_REGU_THRES_SET, val | opt);
}

xpowers_thermal_t AXP2101::getThermaThreshold(void)
{
    return (xpowers_thermal_t)(readRegister(XPOWERS_AXP2101_THE_REGU_THRES_SET) & 0x03);
}

uint8_t AXP2101::getBatteryParameter(){ return readRegister(XPOWERS_AXP2101_BAT_PARAME); }

void AXP2101::fuelGaugeControl(bool writeROM, bool enable)
{
    if (writeROM) {
        clrRegisterBit(XPOWERS_AXP2101_FUEL_GAUGE_CTRL, 4);
    } else {
        setRegisterBit(XPOWERS_AXP2101_FUEL_GAUGE_CTRL, 4);
    }
    if (enable) {
        setRegisterBit(XPOWERS_AXP2101_FUEL_GAUGE_CTRL, 0);
    } else {
        clrRegisterBit(XPOWERS_AXP2101_FUEL_GAUGE_CTRL, 0);
    }
}

/*
 * Interrupt status/control functions
 */

/**
* @brief  Get the interrupt controller mask value.
* @retval   Mask value corresponds to xpowers_axp2101_irq_t ,
*/
uint64_t AXP2101::getIrqStatus(void)
{
    statusRegister[0] = readRegister(XPOWERS_AXP2101_INTSTS1);
    statusRegister[1] = readRegister(XPOWERS_AXP2101_INTSTS2);
    statusRegister[2] = readRegister(XPOWERS_AXP2101_INTSTS3);
    return (uint32_t)(statusRegister[0] << 16) | (uint32_t)(statusRegister[1] << 8) | (uint32_t)(statusRegister[2]);
}


/**
 * @brief  Clear interrupt controller state.
 */
void AXP2101::clearIrqStatus()
{
    for (int i = 0; i < XPOWERS_AXP2101_INTSTS_CNT; i++) {
        writeRegister(XPOWERS_AXP2101_INTSTS1 + i, 0xFF);
        statusRegister[i] = 0;
    }
}

/*
*  @brief  Debug interrupt setting register
* */

void AXP2101::printIntRegister()
{
    for (int i = 0; i < XPOWERS_AXP2101_INTSTS_CNT; i++) {
        uint8_t val =  readRegister(XPOWERS_AXP2101_INTEN1 + i);
        printf("INT[%d] HEX:0x%X\n", i, val);
    }
}


/**
 * @brief  Enable PMU interrupt control mask .
 * @param  opt: View the related chip type xpowers_axp2101_irq_t enumeration
 *              parameters in "axp2101_cost.h"
 * @retval
 */
bool AXP2101::enableIRQ(uint64_t opt){ return setInterruptImpl(opt, true); }

/**
 * @brief  Disable PMU interrupt control mask .
 * @param  opt: View the related chip type xpowers_axp2101_irq_t enumeration
 *              parameters in "axp2101_cost.h"
 * @retval
 */
bool AXP2101::disableIRQ(uint64_t opt){ return setInterruptImpl(opt, false); }

//IRQ STATUS 0
bool AXP2101::isDropWarningLevel2Irq(void)
{
    uint8_t mask = XPOWERS_AXP2101_WARNING_LEVEL2_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isDropWarningLevel1Irq(void)
{
    uint8_t mask = XPOWERS_AXP2101_WARNING_LEVEL1_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isGaugeWdtTimeoutIrq()
{
    uint8_t mask = XPOWERS_AXP2101_WDT_TIMEOUT_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isStateOfChargeLowIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_GAUGE_NEW_SOC_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isBatChargerOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_CHG_OVER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isBatChargerUnderTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_CHG_UNDER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isBatWorkOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_NOR_OVER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

bool AXP2101::isBatWorkUnderTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_NOR_UNDER_TEMP_IRQ;
    if (intRegister[0] & mask) {
        return IS_BIT_SET(statusRegister[0], mask);
    }
    return false;
}

//IRQ STATUS 1
bool AXP2101::isVbusInsertIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_VBUS_INSERT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isVbusRemoveIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_VBUS_REMOVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isBatInsertIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_INSERT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isBatRemoveIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_REMOVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isPekeyShortPressIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_PKEY_SHORT_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;

}

bool AXP2101::isPekeyLongPressIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_PKEY_LONG_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isPekeyNegativeIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_PKEY_NEGATIVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

bool AXP2101::isPekeyPositiveIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_PKEY_POSITIVE_IRQ  >> 8;
    if (intRegister[1] & mask) {
        return IS_BIT_SET(statusRegister[1], mask);
    }
    return false;
}

//IRQ STATUS 2
bool AXP2101::isWdtExpireIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_WDT_EXPIRE_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isLdoOverCurrentIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_LDO_OVER_CURR_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isBatfetOverCurrentIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BATFET_OVER_CURR_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isBatChargeDoneIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isBatChargeStartIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_CHG_START_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isBatDieOverTemperatureIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_DIE_OVER_TEMP_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isChargeOverTimeoutIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_CHARGER_TIMER_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}

bool AXP2101::isBatOverVoltageIrq(void)
{
    uint8_t mask = XPOWERS_AXP2101_BAT_OVER_VOL_IRQ  >> 16;
    if (intRegister[2] & mask) {
        return IS_BIT_SET(statusRegister[2], mask);
    }
    return false;
}


uint8_t AXP2101::getChipID(void)
{
    return readRegister(XPOWERS_AXP2101_IC_TYPE);
}

bool AXP2101::getProtectedChannel(uint8_t channel) {
    return __protectedMask & _BV(channel);
}

uint16_t AXP2101::getPowerChannelVoltage(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return getDC1Voltage();
    case XPOWERS_DCDC2:
        return getDC2Voltage();
    case XPOWERS_DCDC3:
        return getDC3Voltage();
    case XPOWERS_DCDC4:
        return getDC4Voltage();
    case XPOWERS_DCDC5:
        return getDC5Voltage();
    case XPOWERS_ALDO1:
        return getALDO1Voltage();
    case XPOWERS_ALDO2:
        return getALDO2Voltage();
    case XPOWERS_ALDO3:
        return getALDO3Voltage();
    case XPOWERS_ALDO4:
        return getALDO4Voltage();
    case XPOWERS_BLDO1:
        return getBLDO1Voltage();
    case XPOWERS_BLDO2:
        return getBLDO2Voltage();
    case XPOWERS_DLDO1:
        return getDLDO1Voltage();
    case XPOWERS_DLDO2:
        return getDLDO2Voltage();
    case XPOWERS_VBACKUP:
        return getButtonBatteryVoltage();
    default:
        break;
    }
    return 0;
}

bool inline AXP2101::enablePowerOutput(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return enableDC1();
    case XPOWERS_DCDC2:
        return enableDC2();
    case XPOWERS_DCDC3:
        return enableDC3();
    case XPOWERS_DCDC4:
        return enableDC4();
    case XPOWERS_DCDC5:
        return enableDC5();
    case XPOWERS_ALDO1:
        return enableALDO1();
    case XPOWERS_ALDO2:
        return enableALDO2();
    case XPOWERS_ALDO3:
        return enableALDO3();
    case XPOWERS_ALDO4:
        return enableALDO4();
    case XPOWERS_BLDO1:
        return enableBLDO1();
    case XPOWERS_BLDO2:
        return enableBLDO2();
    case XPOWERS_DLDO1:
        return enableDLDO1();
    case XPOWERS_DLDO2:
        return enableDLDO2();
    case XPOWERS_VBACKUP:
        return enableButtonBatteryCharge();
    default:
        break;
    }
    return false;
}

bool inline AXP2101::disablePowerOutput(uint8_t channel)
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
    case XPOWERS_DCDC4:
        return disableDC4();
    case XPOWERS_DCDC5:
        return disableDC5();
    case XPOWERS_ALDO1:
        return disableALDO1();
    case XPOWERS_ALDO2:
        return disableALDO2();
    case XPOWERS_ALDO3:
        return disableALDO3();
    case XPOWERS_ALDO4:
        return disableALDO4();
    case XPOWERS_BLDO1:
        return disableBLDO1();
    case XPOWERS_BLDO2:
        return disableBLDO2();
    case XPOWERS_DLDO1:
        return disableDLDO1();
    case XPOWERS_DLDO2:
        return disableDLDO2();
    case XPOWERS_VBACKUP:
        return disableButtonBatteryCharge();
    case XPOWERS_CPULDO:
        return disableCPUSLDO();
    default:
        break;
    }
    return false;
}

bool inline AXP2101::isPowerChannelEnable(uint8_t channel)
{
    switch (channel) {
    case XPOWERS_DCDC1:
        return isEnableDC1();
    case XPOWERS_DCDC2:
        return isEnableDC2();
    case XPOWERS_DCDC3:
        return isEnableDC3();
    case XPOWERS_DCDC4:
        return isEnableDC4();
    case XPOWERS_DCDC5:
        return isEnableDC5();
    case XPOWERS_ALDO1:
        return isEnableALDO1();
    case XPOWERS_ALDO2:
        return isEnableALDO2();
    case XPOWERS_ALDO3:
        return isEnableALDO3();
    case XPOWERS_ALDO4:
        return isEnableALDO4();
    case XPOWERS_BLDO1:
        return isEnableBLDO1();
    case XPOWERS_BLDO2:
        return isEnableBLDO2();
    case XPOWERS_DLDO1:
        return isEnableDLDO1();
    case XPOWERS_DLDO2:
        return isEnableDLDO2();
    case XPOWERS_VBACKUP:
        return isEnableButtonBatteryCharge();
    case XPOWERS_CPULDO:
        return isEnableCPUSLDO();
    default:
        break;
    }
    return false;
}

bool inline AXP2101::setPowerChannelVoltage(uint8_t channel, uint16_t millivolt)
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
    case XPOWERS_DCDC4:
        return setDC4Voltage(millivolt);
    case XPOWERS_DCDC5:
        return setDC5Voltage(millivolt);
    case XPOWERS_ALDO1:
        return setALDO1Voltage(millivolt);
    case XPOWERS_ALDO2:
        return setALDO2Voltage(millivolt);
    case XPOWERS_ALDO3:
        return setALDO3Voltage(millivolt);
    case XPOWERS_ALDO4:
        return setALDO4Voltage(millivolt);
    case XPOWERS_BLDO1:
        return setBLDO1Voltage(millivolt);
    case XPOWERS_BLDO2:
        return setBLDO1Voltage(millivolt);
    case XPOWERS_DLDO1:
        return setDLDO1Voltage(millivolt);
    case XPOWERS_DLDO2:
        return setDLDO1Voltage(millivolt);
    case XPOWERS_VBACKUP:
        return setButtonBatteryChargeVoltage(millivolt);
    case XPOWERS_CPULDO:
        return setCPUSLDOVoltage(millivolt);
    default:
        break;
    }
    return false;
}

bool AXP2101::initImpl()
{
    if (getChipID() == XPOWERS_AXP2101_CHIP_ID) {
        disableTSPinMeasure();      //Disable NTC temperature detection by default
        return true;
    }
    return false;
}

/*
 * Interrupt control functions
 */
bool AXP2101::setInterruptImpl(uint32_t opts, bool enable)
{
    int res = 0;
    uint8_t data = 0, value = 0;
    ESP_LOGD(TAG, "%s - HEX:0x %lx \n", enable ? "ENABLE" : "DISABLE", opts);
    if (opts & 0x0000FF) {
        value = opts & 0xFF;
        // ESP_LOGD(TAG, "Write INT0: %x\n", value);
        data = readRegister(XPOWERS_AXP2101_INTEN1);
        intRegister[0] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP2101_INTEN1, intRegister[0]);
    }
    if (opts & 0x00FF00) {
        value = opts >> 8;
        // ESP_LOGD(TAG, "Write INT1: %x\n", value);
        data = readRegister(XPOWERS_AXP2101_INTEN2);
        intRegister[1] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP2101_INTEN2, intRegister[1]);
    }
    if (opts & 0xFF0000) {
        value = opts >> 16;
        // ESP_LOGD(TAG, "Write INT2: %x\n", value);
        data = readRegister(XPOWERS_AXP2101_INTEN3);
        intRegister[2] =  enable ? (data | value) : (data & (~value));
        res |= writeRegister(XPOWERS_AXP2101_INTEN3, intRegister[2]);
    }
    return res == 0;
}


float AXP2101::resistance_to_temperature(float resistance, float SteinhartA, 
                                         float SteinhartB, float SteinhartC)
{
    float ln_r = log(resistance);
    float t_inv = SteinhartA + SteinhartB * ln_r + SteinhartC * pow(ln_r, 3);
    return (1.0f / t_inv) - 273.15f;
}

} // namespace axp2101
} // namespace esphome



