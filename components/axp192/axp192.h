// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// This was captured from M5Unified

// to cooperate with ESPHome

#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "axp192_const.h"

namespace esphome {
namespace axp192 {

class AXP192 : public Component, public i2c::I2CDevice {

public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    float get_setup_priority() const override { return setup_priority::IO; }
    
    int readRegister(uint8_t reg);
    int writeRegister(uint8_t reg, uint8_t val);
    // int readRegister(uint8_t reg, uint8_t *buf, uint8_t len);
    // int writeRegister(uint8_t reg, uint8_t *buf, uint8_t len);
    bool inline clrRegisterBit(uint8_t registers, uint8_t bit);
    bool inline setRegisterBit(uint8_t registers, uint8_t bit);
    bool inline getRegisterBit(uint8_t registers, uint8_t bit);
    uint32_t inline readRegisterH8M8L8(uint8_t highReg, uint8_t medReg, uint8_t lowReg);
    uint16_t inline readRegisterH8L4(uint8_t highReg, uint8_t lowReg);
    uint16_t inline readRegisterH6L8(uint8_t highReg, uint8_t lowReg);
    uint16_t inline readRegisterH5L8(uint8_t highReg, uint8_t lowReg);
    uint16_t status();
    bool isACINPresent(void);
    bool isACINValid(void);
    bool isVbusIn(void);
    bool isVbusGood(void);
    bool isDischarge(void);
    bool getThermalRegulationStatus(void);
    bool isCharging(void);
    bool isBatteryConnect(void);
    bool isBatInActiveModeState(void);
    bool isChargingCurrentLessThanExpected(void);

    // //remove these
    // bool getCurrentLimitStatus(void);
    // bool isStandby(void);
    // bool isPowerOn(void);
    // bool isPowerOff(void);
    
    xpowers_chg_status_t getChargerStatus(void);

    //ADC channels
    float getACINVoltage(void);
    float getACINCurrent(void);
    float getVBUSVoltage(void);
    float getVBUSCurrent(void);
    float getInternalTemperature(void);
    float getBattPower(void);
    float getBattChargeCurrent(void);
    float getBattDischargeCurrent(void);
    float getAPSVoltage(void); 

    //GPIO pins
    void setGpioPin(uint8_t gpio, uint8_t pin_state);

    // PMU common configuration
    void enableInternalDischarge(void);
    void disableInternalDischarge(void);
    void enablePwrOkPinPullLow(void);
    void disablePwrOkPinPullLow(void);
    void enablePwronShutPMIC(void);
    void disablePwronShutPMIC(void);
    void reset(void);
    void shutdown(void);

    // // DIE Over Temperature
    // void setDieOverTempLevel1(uint8_t opt);
    // uint8_t getDieOverTempLevel1(void);
    // void enableDieOverTempDetect(void);
    // void disableDieOverTempDetect(void);

    // Linear Charger Vsys voltage dpm
    void setLinearChargerVsysDpm(xpower_chg_dpm_t opt);
    uint8_t getLinearChargerVsysDpm(void);

    // VBUS Voltage/Current Input Limit
    void setVbusVoltageLimit(uint8_t opt);
    uint8_t getVbusVoltageLimit(void);
    bool setVbusCurrentLimit(uint8_t opt);
    uint8_t getVbusCurrentLimit(void);

    // Fuel Gauge
    void resetGauge(void);
    void resetGaugeBesides(void);
    void enableGauge(void);
    void disableGauge(void);

    // Button Battery charge
    bool enableButtonBatteryCharge(void);
    bool disableButtonBatteryCharge(void);
    bool isEnableButtonBatteryCharge();
    bool setButtonBatteryChargeVoltage(uint16_t millivolt);
    uint16_t getButtonBatteryVoltage(void);

    // // Cell Battery charge
    // void enableCellbatteryCharge(void);
    // void disableCellbatteryCharge(void);

    // // Watchdog
    // void enableWatchdog(void);
    // void disableWatchdog(void);
    // void setWatchdogConfig(xpowers_wdt_config_t opt);
    // uint8_t getWatchConfig(void);
    // void clrWatchdog(void);
    // void setWatchdogTimeout(xpowers_wdt_timeout_t opt);
    // uint8_t getWatchdogTimerout(void);

    // // Low battery warning/shutdown threshold
    // void setLowBatWarnThreshold(uint8_t percentage);
    // uint8_t getLowBatWarnThreshold(void);
    // void setLowBatShutdownThreshold(uint8_t opt);
    // uint8_t getLowBatShutdownThreshold(void);

    // Power ON/OFF Source
    bool isPoweronAlwaysHighSource();
    bool isBattInsertOnSource();
    bool isBattNormalOnSource();
    bool isVbusInsertOnSource();
    bool isIrqLowOnSource();
    bool isPwronLowOnSource();
    xpower_power_on_source_t getPowerOnSource();

    bool isOverTemperatureOffSource();
    bool isDcOverVoltageOffSource();
    bool isDcUnderVoltageOffSource();
    bool isVbusOverVoltageOffSource();
    bool isVsysUnderVoltageOffSource();
    bool isPwronAlwaysLowOffSource();
    bool isSwConfigOffSource();
    bool isPwrSourcePullDown();
    xpower_power_off_source_t getPowerOffSource();

    // Power Off/Restart
    void enableOverTemperatureLevel2PowerOff();
    void disableOverTemperaturePowerOff();
    void enableLongPressShutdown();
    void disableLongPressShutdown();
    void setLongPressRestart();
    void setLongPressPowerOFF();

    // DCDC High/Low Voltage Turn Off
    void enableDCHighVoltageTurnOff();
    void disableDCHighVoltageTurnOff();
    void enableDC3LowVoltageTurnOff();
    void disableDC3LowVoltageTurnOff();
    void enableDC2LowVoltageTurnOff();
    void disableDC2LowVoltageTurnOff();
    void enableDC1LowVoltageTurnOff();
    void disableDC1LowVoltageTurnOff();

    // System Power Down Voltage
    bool setSysPowerDownVoltage(uint16_t millivolt);
    uint16_t getSysPowerDownVoltage(void);

    // PWROK/Sequence/Delay
    void enablePwrOk();
    void disablePwrOk();
    void enablePowerOffDelay();
    void disablePowerOffDelay();
    void enablePowerSequence();
    void disablePowerSequence();
    bool setPwrOkDelay(xpower_pwrok_delay_t opt);
    xpower_pwrok_delay_t getPwrOkDelay();

    // Sleep/Wakeup
    void wakeupControl(xpowers_wakeup_t opt, bool enable);
    bool enableWakeup(void);
    bool disableWakeup(void);
    bool enableSleep(void);
    bool disableSleep(void);

    // IRQ/OFFLEVEL/ONLEVEL
    void setIrqLevel(uint8_t opt);
    void setOffLevel(uint8_t opt);
    void setOnLevel(uint8_t opt);

    // DCDC High/Low Voltage Power Down
    void setDCHighVoltagePowerDown(bool en);
    bool getDCHighVoltagePowerDownEn();
    void setDcUVPDebounceTime(uint8_t opt);
    void settDC1WorkModeToPwm(uint8_t enable);
    void settDC2WorkModeToPwm(uint8_t enable);
    void settDC3WorkModeToPwm(uint8_t enable);
    void setDCFreqSpreadRange(uint8_t opt);
    void setDCFreqSpreadRangeEn(bool en);

    // CCM
    void enableCCM();
    void disableCCM();
    bool isenableCCM();
    void setDVMRamp(uint8_t opt);

    // DCDC1
    bool isEnableDC1(void);
    bool enableDC1(void);
    bool disableDC1(void);
    bool setDC1Voltage(uint16_t millivolt);
    uint16_t getDC1Voltage(void);
    void setDC1LowVoltagePowerDown(bool en);
    bool getDC1LowVoltagePowerDownEn();

    // DCDC2
    bool isEnableDC2(void);
    bool enableDC2(void);
    bool disableDC2(void);
    bool setDC2Voltage(uint16_t millivolt);
    uint16_t getDC2Voltage(void);
    uint8_t getDC2WorkMode(void);
    void setDC2LowVoltagePowerDown(bool en);
    bool getDC2LowVoltagePowerDownEn();

    // DCDC3
    bool isEnableDC3(void);
    bool enableDC3(void);
    bool disableDC3(void);
    bool setDC3Voltage(uint16_t millivolt);
    uint16_t getDC3Voltage(void);
    uint8_t getDC3WorkMode(void);
    void setDC3LowVoltagePowerDown(bool en);
    bool getDC3LowVoltagePowerDownEn();


    // ALDO1
    bool isEnableALDO1(void);
    bool enableALDO1(void);
    bool disableALDO1(void);
    bool setALDO1Voltage(uint16_t millivolt);
    uint16_t getALDO1Voltage(void);

    // ALDO2
    bool isEnableALDO2(void);
    bool enableALDO2(void);
    bool disableALDO2(void);
    bool setALDO2Voltage(uint16_t millivolt);
    uint16_t getALDO2Voltage(void);

    // ALDO3
    bool isEnableALDO3(void);
    bool enableALDO3(void);
    bool disableALDO3(void);
    bool setALDO3Voltage(uint16_t millivolt);
    uint16_t getALDO3Voltage(void);


    // ALDO4
    bool isEnableALDO4(void);
    bool enableALDO4(void);
    bool disableALDO4(void);
    bool setALDO4Voltage(uint16_t millivolt);
    uint16_t getALDO4Voltage(void);

    // DLDO3
    bool isEnableDLDO3(void);
    bool enableDLDO3(void);
    bool disableDLDO3(void);
    bool setDLDO3Voltage(uint16_t millivolt);
    uint16_t getDLDO3Voltage(void);

    // DLDO2
    bool isEnableDLDO2(void);
    bool enableDLDO2(void);
    bool disableDLDO2(void);
    bool setDLDO2Voltage(uint16_t millivolt);
    uint16_t getDLDO2Voltage(void);


    void setIrqLevelTime(xpowers_irq_time_t opt);
    xpowers_irq_time_t getIrqLevelTime(void);
    bool setPowerKeyPressOnTime(uint8_t opt);
    uint8_t getPowerKeyPressOnTime(void);
    bool setPowerKeyPressOffTime(uint8_t opt);
    uint8_t getPowerKeyPressOffTime(void);
    bool enableGeneralAdcChannel(void);
    bool disableGeneralAdcChannel(void);
    bool enableTemperatureMeasure(void);
    bool disableTemperatureMeasure(void);

    float getTemperature(void);
    bool enableSystemVoltageMeasure(void);
    bool disableSystemVoltageMeasure(void);
    uint16_t getSystemVoltage(void);
    bool configureAdcMeasure(void);
    bool enableVbusVoltageMeasure(void);
    bool disableVbusVoltageMeasure(void);
    uint16_t getVbusVoltage(void);
    bool enableTSPinMeasure(void);
    bool disableTSPinMeasure(void);
    bool enableTSPinLowFreqSample(void);
    bool disableTSPinLowFreqSample(void);
    float getTsTemperature(float SteinhartA = 1.126e-3,
                            float SteinhartB = 2.38e-4,
                            float SteinhartC = 8.5e-8);
    uint16_t getTsPinValue(void);
    bool enableBattVoltageMeasure(void);
    bool disableBattVoltageMeasure(void);
    bool enableBattDetection(void);
    bool disableBattDetection(void);
    float getBattVoltage(void);
    int getBatteryPercent(void);
    int getScaledBatteryPercent(uint16_t min_voltage, uint16_t max_voltage, uint16_t min_current, uint16_t max_current); //New function
    void setChargingLedMode(uint8_t mode);
    uint8_t getChargingLedMode();
    void setPrechargeCurr(xpowers_prechg_t opt);
    xpowers_prechg_t getPrechargeCurr(void);
    bool setChargerConstantCurr(uint8_t opt);
    uint8_t getChargerConstantCurr(void);
    void setChargerTerminationCurr(xpowers_axp192_chg_iterm_t opt);
    xpowers_axp192_chg_iterm_t getChargerTerminationCurr(void);
    void enableChargerTerminationLimit(void);
    void disableChargerTerminationLimit(void);
    bool isChargerTerminationLimit(void);
    bool setChargeTargetVoltage(uint8_t opt);
    uint8_t getChargeTargetVoltage(void);
    void setThermaThreshold(xpowers_thermal_t opt);
    xpowers_thermal_t getThermaThreshold(void);

    uint8_t getBatteryParameter();
    void fuelGaugeControl(bool writeROM, bool enable);
    uint64_t getIrqStatus(void);
    void clearIrqStatus();
    void printIntRegister();
    bool enableIRQ(uint64_t opt);
    bool disableIRQ(uint64_t opt);
    bool isDropWarningLevel2Irq(void);
    bool isDropWarningLevel1Irq(void);
    bool isGaugeWdtTimeoutIrq();
    bool isStateOfChargeLowIrq(void);
    bool isBatChargerOverTemperatureIrq(void);
    bool isBatChargerUnderTemperatureIrq(void);
    bool isBatWorkOverTemperatureIrq(void);
    bool isBatWorkUnderTemperatureIrq(void);
    bool isVbusInsertIrq(void);
    bool isVbusRemoveIrq(void);
    bool isBatInsertIrq(void);
    bool isBatRemoveIrq(void);
    bool isPekeyShortPressIrq(void);
    bool isPekeyLongPressIrq(void);
    bool isPekeyNegativeIrq(void);
    bool isPekeyPositiveIrq(void);
    bool isWdtExpireIrq(void);
    bool isLdoOverCurrentIrq(void);
    bool isBatChargeDoneIrq(void);
    bool isBatChargeStartIrq(void);
    bool isBatDieOverTemperatureIrq(void);
    bool isChargeOverTimeoutIrq(void);
    bool isBatOverVoltageIrq(void);
    uint8_t getChipID(void);


protected:
    bool getProtectedChannel(uint8_t channel);
    uint16_t getPowerChannelVoltage(uint8_t channel);
    bool inline enablePowerOutput(uint8_t channel);
    bool inline disablePowerOutput(uint8_t channel);
    bool inline isPowerChannelEnable(uint8_t channel);
    bool inline setPowerChannelVoltage(uint8_t channel, uint16_t millivolt);
    bool initImpl();
    bool setInterruptImpl(uint32_t opts, bool enable);

    void setChipModel(uint8_t m){  this->__chipModel = m; }

private:
    uint8_t statusRegister[XPOWERS_AXP192_INTSTS_CNT];
    uint8_t intRegister[XPOWERS_AXP192_INTSTS_CNT];
    float resistance_to_temperature(float resistance, float SteinhartA,
                                    float SteinhartB,
                                    float SteinhartC);

    uint8_t __chipModel{0};
    uint32_t __protectedMask{0};

};
    
    
} // namespace axp192
    
} // namespace esphome