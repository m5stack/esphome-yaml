// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// This was captured from M5Unified

// to cooperate with ESPHome

#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "axp2101_const.h"

namespace esphome {
namespace axp2101 {

class AXP2101 : public Component, public i2c::I2CDevice {

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
    uint16_t inline readRegisterH6L8(uint8_t highReg, uint8_t lowReg);
    uint16_t inline readRegisterH5L8(uint8_t highReg, uint8_t lowReg);
    uint16_t status();
    bool isVbusGood(void);
    bool getBatfetState(void);
    bool isBatteryConnect(void);
    bool isBatInActiveModeState(void);
    bool getThermalRegulationStatus(void);
    bool getCurrentLimitStatus(void);
    bool isCharging(void);
    bool isDischarge(void);
    bool isStandby(void);
    bool isPowerOn(void);
    bool isPowerOff(void);
    bool isVbusIn(void);
    xpowers_chg_status_t getChargerStatus(void);

    // PMU common configuration
    void enableInternalDischarge(void);
    void disableInternalDischarge(void);
    void enablePwrOkPinPullLow(void);
    void disablePwrOkPinPullLow(void);
    void enablePwronShutPMIC(void);
    void disablePwronShutPMIC(void);
    void reset(void);
    void shutdown(void);

    // BATFET control
    void setBatfetDieOverTempLevel1(uint8_t opt);
    uint8_t getBatfetDieOverTempLevel1(void);
    void enableBatfetDieOverTempDetect(void);
    void disableBatfetDieOverTempDetect(void);

    // DIE Over Temperature
    void setDieOverTempLevel1(uint8_t opt);
    uint8_t getDieOverTempLevel1(void);
    void enableDieOverTempDetect(void);
    void disableDieOverTempDetect(void);

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

    // Cell Battery charge
    void enableCellbatteryCharge(void);
    void disableCellbatteryCharge(void);

    // Watchdog
    void enableWatchdog(void);
    void disableWatchdog(void);
    void setWatchdogConfig(xpowers_wdt_config_t opt);
    uint8_t getWatchConfig(void);
    void clrWatchdog(void);
    void setWatchdogTimeout(xpowers_wdt_timeout_t opt);
    uint8_t getWatchdogTimerout(void);

    // Low battery warning/shutdown threshold
    void setLowBatWarnThreshold(uint8_t percentage);
    uint8_t getLowBatWarnThreshold(void);
    void setLowBatShutdownThreshold(uint8_t opt);
    uint8_t getLowBatShutdownThreshold(void);

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
    void enableDC5LowVoltageTurnOff();
    void disableDC5LowVoltageTurnOff();
    void enableDC4LowVoltageTurnOff();
    void disableDC4LowVoltageTurnOff();
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

    // Fast Power On Sequence
    void setDc4FastStartSequence(xpower_start_sequence_t opt);
    void setDc3FastStartSequence(xpower_start_sequence_t opt);
    void setDc2FastStartSequence(xpower_start_sequence_t opt);
    void setDc1FastStartSequence(xpower_start_sequence_t opt);
    void setAldo3FastStartSequence(xpower_start_sequence_t opt);
    void setAldo2FastStartSequence(xpower_start_sequence_t opt);
    void setAldo1FastStartSequence(xpower_start_sequence_t opt);
    void setDc5FastStartSequence(xpower_start_sequence_t opt);
    void setCpuldoFastStartSequence(xpower_start_sequence_t opt);
    void setBldo2FastStartSequence(xpower_start_sequence_t opt);
    void setBldo1FastStartSequence(xpower_start_sequence_t opt);
    void setAldo4FastStartSequence(xpower_start_sequence_t opt);
    void setDldo2FastStartSequence(xpower_start_sequence_t opt);
    void setDldo1FastStartSequence(xpower_start_sequence_t opt);
    void setFastPowerOnLevel(xpowers_fast_on_opt_t opt, xpower_start_sequence_t seq_level);
    void disableFastPowerOn(xpowers_fast_on_opt_t opt);
    void enableFastPowerOn(void);
    void disableFastPowerOn(void);
    void enableFastWakeup(void);
    void disableFastWakeup(void);

    // DCDC High/Low Voltage Power Down
    void setDCHighVoltagePowerDown(bool en);
    bool getDCHighVoltagePowerDownEn();
    void setDcUVPDebounceTime(uint8_t opt);
    void settDC1WorkModeToPwm(uint8_t enable);
    void settDC2WorkModeToPwm(uint8_t enable);
    void settDC3WorkModeToPwm(uint8_t enable);
    void settDC4WorkModeToPwm(uint8_t enable);
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

    // DCDC4
    bool isEnableDC4(void);
    bool enableDC4(void);
    bool disableDC4(void);
    bool setDC4Voltage(uint16_t millivolt);
    uint16_t getDC4Voltage(void);
    void setDC4LowVoltagePowerDown(bool en);
    bool getDC4LowVoltagePowerDownEn();

    // DCDC5
    bool isEnableDC5(void);
    bool enableDC5(void);
    bool disableDC5(void);
    bool setDC5Voltage(uint16_t millivolt);
    uint16_t getDC5Voltage(void);
    bool isDC5FreqCompensationEn(void);
    void enableDC5FreqCompensation();
    void disableFreqCompensation();
    void setDC5LowVoltagePowerDown(bool en);
    bool getDC5LowVoltagePowerDownEn();

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

    // BLDO1
    bool isEnableBLDO1(void);
    bool enableBLDO1(void);
    bool disableBLDO1(void);
    bool setBLDO1Voltage(uint16_t millivolt);
    uint16_t getBLDO1Voltage(void);

    // BLDO2
    bool isEnableBLDO2(void);
    bool enableBLDO2(void);
    bool disableBLDO2(void);
    bool setBLDO2Voltage(uint16_t millivolt);
    uint16_t getBLDO2Voltage(void);

    // CPUSLDO
    bool isEnableCPUSLDO(void);
    bool enableCPUSLDO(void);
    bool disableCPUSLDO(void);
    bool setCPUSLDOVoltage(uint16_t millivolt);
    uint16_t getCPUSLDOVoltage(void);

    // DLDO1
    bool isEnableDLDO1(void);
    bool enableDLDO1(void);
    bool disableDLDO1(void);
    bool setDLDO1Voltage(uint16_t millivolt);
    uint16_t getDLDO1Voltage(void);

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
    uint16_t getBattVoltage(void);
    int getBatteryPercent(void);

    void setChargingLedMode(uint8_t mode);
    uint8_t getChargingLedMode();
    void setPrechargeCurr(xpowers_prechg_t opt);
    xpowers_prechg_t getPrechargeCurr(void);
    bool setChargerConstantCurr(uint8_t opt);
    uint8_t getChargerConstantCurr(void);
    void setChargerTerminationCurr(xpowers_axp2101_chg_iterm_t opt);
    xpowers_axp2101_chg_iterm_t getChargerTerminationCurr(void);
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
    bool isBatfetOverCurrentIrq(void);
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
    uint8_t statusRegister[XPOWERS_AXP2101_INTSTS_CNT];
    uint8_t intRegister[XPOWERS_AXP2101_INTSTS_CNT];
    float resistance_to_temperature(float resistance, float SteinhartA,
                                    float SteinhartB,
                                    float SteinhartC);

    uint8_t __chipModel{0};
    uint32_t __protectedMask{0};

};
    
    
} // namespace axp2101
    
} // namespace esphome