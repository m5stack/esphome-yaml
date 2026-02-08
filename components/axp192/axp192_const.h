#pragma once

#ifdef _BV
#undef _BV
#endif
#define _BV(b)                          (1ULL << (uint64_t)(b))
#define IS_BIT_SET(val,mask)            (((val)&(mask)) == (mask))

#define AXP192_SLAVE_ADDRESS                            (0x34)

#define XPOWERS_AXP192_CHIP_ID                          (0x03) //was 0x4A - only tested on my device, no idea if correct as this is an undocumented register

#define XPOWERS_AXP192_STATUS1                          (0x00)
#define XPOWERS_AXP192_STATUS1a                         (0x01) //was 00
#define XPOWERS_AXP192_STATUS2                          (0x01)
#define XPOWERS_AXP192_IC_TYPE                          (0x03)


#define XPOWERS_AXP192_DATA_BUFFER1                     (0x04)
#define XPOWERS_AXP192_DATA_BUFFER2                     (0x05)
#define XPOWERS_AXP192_DATA_BUFFER3                     (0x06)
#define XPOWERS_AXP192_DATA_BUFFER4                     (0x07)
#define XPOWERS_AXP192_DATA_BUFFER_SIZE                 (4u)

#define XPOWERS_AXP192_COMMON_CONFIG                    (0x10)
#define XPOWERS_AXP192_BATFET_CTRL                      (0x12)
#define XPOWERS_AXP192_DIE_TEMP_CTRL                    (0x13)
#define XPOWERS_AXP192_MIN_SYS_VOL_CTRL                 (0x14)
#define XPOWERS_AXP192_INPUT_VOL_LIMIT_CTRL             (0x15)
#define XPOWERS_AXP192_INPUT_CUR_LIMIT_CTRL             (0x16)
#define XPOWERS_AXP192_RESET_FUEL_GAUGE                 (0x17)
#define XPOWERS_AXP192_CHARGE_GAUGE_WDT_CTRL            (0x18)


#define XPOWERS_AXP192_WDT_CTRL                         (0x19)
#define XPOWERS_AXP192_LOW_BAT_WARN_SET                 (0x1A)


#define XPOWERS_AXP192_PWRON_STATUS                     (0x20)
#define XPOWERS_AXP192_PWROFF_STATUS                    (0x21)
#define XPOWERS_AXP192_PWROFF_EN                        (0x22)
#define XPOWERS_AXP192_DC_OVP_UVP_CTRL                  (0x23)
#define XPOWERS_AXP192_VOFF_SET                         (0x24)
#define XPOWERS_AXP192_PWROK_SEQU_CTRL                  (0x25)
#define XPOWERS_AXP192_SLEEP_WAKEUP_CTRL                (0x26)
#define XPOWERS_AXP192_IRQ_OFF_ON_LEVEL_CTRL            (0x36) //was 0x27

#define XPOWERS_AXP192_FAST_PWRON_SET0                  (0x28)
#define XPOWERS_AXP192_FAST_PWRON_SET1                  (0x29)
#define XPOWERS_AXP192_FAST_PWRON_SET2                  (0x2A)
#define XPOWERS_AXP192_FAST_PWRON_CTRL                  (0x2B)

#define XPOWERS_AXP192_ADC_CHANNEL_CTRL1                 (0x82) //was 0x30
#define XPOWERS_AXP192_ADC_CHANNEL_CTRL2                 (0x83) //new register in AXP192
#define XPOWERS_AXP192_ADC_DATA_ACINVH                  (0x56)
#define XPOWERS_AXP192_ADC_DATA_ACINVL                  (0x57)
#define XPOWERS_AXP192_ADC_DATA_ACINIH                  (0x58)
#define XPOWERS_AXP192_ADC_DATA_ACINIL                  (0x59)
#define XPOWERS_AXP192_ADC_DATA_VBUSVH                  (0x5A)
#define XPOWERS_AXP192_ADC_DATA_VBUSVL                  (0x5B)
#define XPOWERS_AXP192_ADC_DATA_VBUSIH                  (0x5C)
#define XPOWERS_AXP192_ADC_DATA_VBUSIL                  (0x5D)
#define XPOWERS_AXP192_ADC_DATA_INTTH                   (0x5E)
#define XPOWERS_AXP192_ADC_DATA_INTTL                   (0x5F)
#define XPOWERS_AXP192_ADC_DATA_TSPINH                  (0x62) //Not used shorted to GNDin Core 2
#define XPOWERS_AXP192_ADC_DATA_TSPINL                  (0x63) //Not used shorted to GNDin Core 2
#define XPOWERS_AXP192_ADC_DATA_BATTPH                  (0x70)
#define XPOWERS_AXP192_ADC_DATA_BATTPM                  (0x71)
#define XPOWERS_AXP192_ADC_DATA_BATTPL                  (0x72)
#define XPOWERS_AXP192_ADC_DATA_APSVH                   (0x7E)
#define XPOWERS_AXP192_ADC_DATA_BATTVH                  (0x78)
#define XPOWERS_AXP192_ADC_DATA_BATTVL                  (0x79)
#define XPOWERS_AXP192_ADC_DATA_BATTCHGH                (0x7A)
#define XPOWERS_AXP192_ADC_DATA_BATTCHGL                (0x7B)
#define XPOWERS_AXP192_ADC_DATA_BATTDCHGH               (0x7C)
#define XPOWERS_AXP192_ADC_DATA_BATTDCHGL               (0x7D)
#define XPOWERS_AXP192_ADC_DATA_APSVL                   (0x7F)


//next block are old code, remove when possible
#define XPOWERS_AXP192_ADC_DATA_RELUST0                 (0x34)
#define XPOWERS_AXP192_ADC_DATA_RELUST1                 (0x35)
#define XPOWERS_AXP192_ADC_DATA_RELUST2                 (0x36)
#define XPOWERS_AXP192_ADC_DATA_RELUST3                 (0x37)
#define XPOWERS_AXP192_ADC_DATA_RELUST4                 (0x38)
#define XPOWERS_AXP192_ADC_DATA_RELUST5                 (0x39)
#define XPOWERS_AXP192_ADC_DATA_RELUST6                 (0x3A)
#define XPOWERS_AXP192_ADC_DATA_RELUST7                 (0x3B)
#define XPOWERS_AXP192_ADC_DATA_RELUST8                 (0x3C)
#define XPOWERS_AXP192_ADC_DATA_RELUST9                 (0x3D)


//XPOWERS INTERRUPT REGISTER
#define XPOWERS_AXP192_INTEN1                           (0x40)
#define XPOWERS_AXP192_INTEN2                           (0x41)
#define XPOWERS_AXP192_INTEN3                           (0x42)


//XPOWERS INTERRUPT STATUS REGISTER
#define XPOWERS_AXP192_INTSTS1                          (0x44) //was 0x48
#define XPOWERS_AXP192_INTSTS2                          (0x45)
#define XPOWERS_AXP192_INTSTS3                          (0x46)
#define XPOWERS_AXP192_INTSTS4                          (0x47) //extra register in AXP192
#define XPOWERS_AXP192_INTSTS_CNT                       (4) //was 3

#define XPOWERS_AXP192_TS_PIN_CTRL                      (0x84) //was 0x50
#define XPOWERS_AXP192_TS_HYSL2H_SET                    (0x52)
#define XPOWERS_AXP192_TS_LYSL2H_SET                    (0x53)


#define XPOWERS_AXP192_VLTF_CHG_SET                     (0x54)
#define XPOWERS_AXP192_VHLTF_CHG_SET                    (0x55)
#define XPOWERS_AXP192_VLTF_WORK_SET                    (0x56)
#define XPOWERS_AXP192_VHLTF_WORK_SET                   (0x57)


#define XPOWERS_AXP192_JIETA_EN_CTRL                    (0x58)
#define XPOWERS_AXP192_JIETA_SET0                       (0x59)
#define XPOWERS_AXP192_JIETA_SET1                       (0x5A)
#define XPOWERS_AXP192_JIETA_SET2                       (0x5B)


#define XPOWERS_AXP192_IPRECHG_SET                      (0x61) //NA remove
#define XPOWERS_AXP192_ICC_CHG_SET                      (0x33) //was 0x62
#define XPOWERS_AXP192_ITERM_CHG_SET_CTRL               (0x33) //was 0x63

#define XPOWERS_AXP192_CV_CHG_VOL_SET                   (0x64)

#define XPOWERS_AXP192_THE_REGU_THRES_SET               (0x65)
#define XPOWERS_AXP192_CHG_TIMEOUT_SET_CTRL             (0x67)

#define XPOWERS_AXP192_BAT_DET_CTRL                     (0x32) //was 0x68
#define XPOWERS_AXP192_CHGLED_SET_CTRL                  (0x32) //was 0x69
#define XPOWERS_AXP192_BTN_VOL_MIN                      (2600)
#define XPOWERS_AXP192_BTN_VOL_MAX                      (3300)
#define XPOWERS_AXP192_BTN_VOL_STEPS                    (100)


#define XPOWERS_AXP192_BTN_BAT_CHG_VOL_SET              (0x35) //was 0x6A


#define XPOWERS_AXP192_DC_ONOFF_DVM_CTRL                (0x12) //was 0x80
#define XPOWERS_AXP192_DC2_ONOFF_DVM_CTRL               (0x10) //was 0x80
#define XPOWERS_AXP192_DC_FORCE_PWM_CTRL                (0x81)
#define XPOWERS_AXP192_DC_VOL0_CTRL                     (0x26) //was 0x82
#define XPOWERS_AXP192_DC_VOL1_CTRL                     (0x23) //was 0x83
#define XPOWERS_AXP192_DC_VOL2_CTRL                     (0x27) //was 0x84
#define XPOWERS_AXP192_DC_VOL3_CTRL                     (0x85) //not exist remove
#define XPOWERS_AXP192_DC_VOL4_CTRL                     (0x86) //not exist remove


#define XPOWERS_AXP192_LDO_ONOFF_CTRL0                  (0x12) //was 0x90
#define XPOWERS_AXP192_LDO_ONOFF_CTRL1                  (0x91) //unused, remove
#define XPOWERS_AXP192_LDO_VOL0_CTRL                    (0x92)
#define XPOWERS_AXP192_LDO_VOL1_CTRL                    (0x93)
#define XPOWERS_AXP192_LDO_VOL2_CTRL                    (0x94)
#define XPOWERS_AXP192_LDO_VOL3_CTRL                    (0x95)
#define XPOWERS_AXP192_LDO_VOL4_CTRL                    (0x96)
#define XPOWERS_AXP192_LDO_VOL5_CTRL                    (0x97)
#define XPOWERS_AXP192_LDO_VOL6_CTRL                    (0x98)
#define XPOWERS_AXP192_LDO_VOL7_CTRL                    (0x28) //was 99 LDO3 voltage
#define XPOWERS_AXP192_LDO_VOL8_CTRL                    (0x28) //was 9A LDO2 voltage

#define XPOWERS_AXP192_GPIO0_MODE                       (0x90)
#define XPOWERS_AXP192_GPIO1_MODE                       (0x92)
#define XPOWERS_AXP192_GPIO2_MODE                       (0x93)
#define XPOWERS_AXP192_GPIO3_MODE                       (0x94)
#define XPOWERS_AXP192_GPIO4_MODE                       (0x95)

#define XPOWERS_AXP192_BAT_PARAME                       (0xA1)
#define XPOWERS_AXP192_FUEL_GAUGE_CTRL                  (0xA2)
#define XPOWERS_AXP192_BAT_PERCENT_DATA                 (0xA4)

// DCDC 1~5
#define XPOWERS_AXP192_DCDC1_VOL_MIN                    (1500)
#define XPOWERS_AXP192_DCDC1_VOL_MAX                    (3400)
#define XPOWERS_AXP192_DCDC1_VOL_STEPS                  (100u)

#define XPOWERS_AXP192_DCDC2_VOL1_MIN                  (500u)
#define XPOWERS_AXP192_DCDC2_VOL1_MAX                  (1200u)
#define XPOWERS_AXP192_DCDC2_VOL2_MIN                  (1220u)
#define XPOWERS_AXP192_DCDC2_VOL2_MAX                  (1540u)

#define XPOWERS_AXP192_DCDC2_VOL_STEPS1                 (10u)
#define XPOWERS_AXP192_DCDC2_VOL_STEPS2                 (20u)

#define XPOWERS_AXP192_DCDC2_VOL_STEPS1_BASE            (0u)
#define XPOWERS_AXP192_DCDC2_VOL_STEPS2_BASE            (71)

//ADC scales for each step in reading
#define XPOWERS_AXP192_ADC_SCALE_BATTV                  1.1 //mV
#define XPOWERS_AXP192_ADC_SCALE_BATTI                  0.5 //mA
#define XPOWERS_AXP192_ADC_SCALE_BATTCHGI               0.5 //mA
#define XPOWERS_AXP192_ADC_SCALE_ACINV                  1.7 //mV
#define XPOWERS_AXP192_ADC_SCALE_ACINI                  0.625 //mA
#define XPOWERS_AXP192_ADC_SCALE_VBUSV                  1.7 //mV
#define XPOWERS_AXP192_ADC_SCALE_VBUSI                  0.375 //mA
#define XPOWERS_AXP192_ADC_SCALE_INTT                   0.1 //degC
#define XPOWERS_AXP192_ADC_OFFSET_INTT                  -144.7 //degC
#define XPOWERS_AXP192_ADC_SCALE_APSV                   1.4 //mV
#define XPOWERS_AXP192_ADC_SCALE_TSPINV                 0.8 //mV
#define XPOWERS_AXP192_ADC_SCALE_GPIOV                  0.5 //mV

// #define XPOWERS_AXP192_DCDC3_VOL1_MIN                  (700u)
// #define XPOWERS_AXP192_DCDC3_VOL1_MAX                  (3500u)
// #define XPOWERS_AXP192_DCDC3_VOL2_MIN                  (1220u)
// #define XPOWERS_AXP192_DCDC3_VOL2_MAX                  (1540u)
// #define XPOWERS_AXP192_DCDC3_VOL3_MIN                  (1600u)
// #define XPOWERS_AXP192_DCDC3_VOL3_MAX                  (3400u)

#define XPOWERS_AXP192_DCDC3_VOL_MIN                    (2000) // was 500
#define XPOWERS_AXP192_DCDC3_VOL_MAX                    (3500) //was 3400

#define XPOWERS_AXP192_DCDC3_VOL_STEPS                 (25u) //was 10
// #define XPOWERS_AXP192_DCDC3_VOL_STEPS2                 (20u)
// #define XPOWERS_AXP192_DCDC3_VOL_STEPS3                 (100u)

#define XPOWERS_AXP192_DCDC3_VOL_STEPS_BASE            (66)
// #define XPOWERS_AXP192_DCDC3_VOL_STEPS2_BASE            (71)
// #define XPOWERS_AXP192_DCDC3_VOL_STEPS3_BASE            (88)



#define XPOWERS_AXP192_DCDC4_VOL1_MIN                  (500u)
#define XPOWERS_AXP192_DCDC4_VOL1_MAX                  (1200u)
#define XPOWERS_AXP192_DCDC4_VOL2_MIN                  (1220u)
#define XPOWERS_AXP192_DCDC4_VOL2_MAX                  (1840u)

#define XPOWERS_AXP192_DCDC4_VOL_STEPS1                 (10u)
#define XPOWERS_AXP192_DCDC4_VOL_STEPS2                 (20u)

#define XPOWERS_AXP192_DCDC4_VOL_STEPS1_BASE            (0u)
#define XPOWERS_AXP192_DCDC4_VOL_STEPS2_BASE            (71)



#define XPOWERS_AXP192_DCDC5_VOL_1200MV                 (1200)
#define XPOWERS_AXP192_DCDC5_VOL_VAL                    (0x19)
#define XPOWERS_AXP192_DCDC5_VOL_MIN                    (1400)
#define XPOWERS_AXP192_DCDC5_VOL_MAX                    (3700)
#define XPOWERS_AXP192_DCDC5_VOL_STEPS                  (100u)

#define XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MIN          (2600)
#define XPOWERS_AXP192_VSYS_VOL_THRESHOLD_MAX          (3300)
#define XPOWERS_AXP192_VSYS_VOL_THRESHOLD_STEPS        (100)

// ALDO 1~4

#define XPOWERS_AXP192_ALDO1_VOL_MIN                    (500)
#define XPOWERS_AXP192_ALDO1_VOL_MAX                    (3500)
#define XPOWERS_AXP192_ALDO1_VOL_STEPS                  (100u)

#define XPOWERS_AXP192_ALDO2_VOL_MIN                    (500)
#define XPOWERS_AXP192_ALDO2_VOL_MAX                    (3500)
#define XPOWERS_AXP192_ALDO2_VOL_STEPS                  (100u)


#define XPOWERS_AXP192_ALDO3_VOL_MIN                    (500)
#define XPOWERS_AXP192_ALDO3_VOL_MAX                    (3500)
#define XPOWERS_AXP192_ALDO3_VOL_STEPS                  (100u)


#define XPOWERS_AXP192_ALDO4_VOL_MIN                    (500)
#define XPOWERS_AXP192_ALDO4_VOL_MAX                    (3500)
#define XPOWERS_AXP192_ALDO4_VOL_STEPS                  (100u)

// BLDO 1~2

#define XPOWERS_AXP192_BLDO1_VOL_MIN                    (500)
#define XPOWERS_AXP192_BLDO1_VOL_MAX                    (3500)
#define XPOWERS_AXP192_BLDO1_VOL_STEPS                  (100u)

#define XPOWERS_AXP192_BLDO2_VOL_MIN                    (500)
#define XPOWERS_AXP192_BLDO2_VOL_MAX                    (3500)
#define XPOWERS_AXP192_BLDO2_VOL_STEPS                  (100u)

// CPUSLDO

#define XPOWERS_AXP192_CPUSLDO_VOL_MIN                  (500)
#define XPOWERS_AXP192_CPUSLDO_VOL_MAX                  (1400)
#define XPOWERS_AXP192_CPUSLDO_VOL_STEPS                (50)


// DLDO 1~2
#define XPOWERS_AXP192_DLDO3_VOL_MIN                  (1800)
#define XPOWERS_AXP192_DLDO3_VOL_MAX                  (3300)
#define XPOWERS_AXP192_DLDO3_VOL_STEPS                (100u)

#define XPOWERS_AXP192_DLDO2_VOL_MIN                  (1800)
#define XPOWERS_AXP192_DLDO2_VOL_MAX                  (3300)
#define XPOWERS_AXP192_DLDO2_VOL_STEPS                (100u)


#define XPOWERS_AXP192_CONVERSION(raw)                 (22.0 + (7274 - raw) / 20.0)


typedef enum {
    XPOWERS_AXP192_IRQ_TIME_1S,
    XPOWERS_AXP192_IRQ_TIME_1S5,
    XPOWERS_AXP192_IRQ_TIME_2S,
    XPOWERS_AXP192_PRESSOFF_2S5,
} xpowers_irq_time_t;



typedef enum {
    XPOWERS_AXP192_PRECHARGE_0MA,
    XPOWERS_AXP192_PRECHARGE_25MA,
    XPOWERS_AXP192_PRECHARGE_50MA,
    XPOWERS_AXP192_PRECHARGE_75MA,
    XPOWERS_AXP192_PRECHARGE_100MA,
    XPOWERS_AXP192_PRECHARGE_125MA,
    XPOWERS_AXP192_PRECHARGE_150MA,
    XPOWERS_AXP192_PRECHARGE_175MA,
    XPOWERS_AXP192_PRECHARGE_200MA,
} xpowers_prechg_t;

typedef enum {
    XPOWERS_AXP192_CHG_ITERM_0MA,
    XPOWERS_AXP192_CHG_ITERM_25MA,
    XPOWERS_AXP192_CHG_ITERM_50MA,
    XPOWERS_AXP192_CHG_ITERM_75MA,
    XPOWERS_AXP192_CHG_ITERM_100MA,
    XPOWERS_AXP192_CHG_ITERM_125MA,
    XPOWERS_AXP192_CHG_ITERM_150MA,
    XPOWERS_AXP192_CHG_ITERM_175MA,
    XPOWERS_AXP192_CHG_ITERM_200MA,
} xpowers_axp192_chg_iterm_t;


typedef enum {
    XPOWERS_AXP192_THREMAL_60DEG,
    XPOWERS_AXP192_THREMAL_80DEG,
    XPOWERS_AXP192_THREMAL_100DEG,
    XPOWERS_AXP192_THREMAL_120DEG,
} xpowers_thermal_t;

typedef enum {
    XPOWERS_AXP192_CHG_TRI_STATE,   //tri_charge
    XPOWERS_AXP192_CHG_PRE_STATE,   //pre_charge
    XPOWERS_AXP192_CHG_CC_STATE,    //constant charge
    XPOWERS_AXP192_CHG_CV_STATE,    //constant voltage
    XPOWERS_AXP192_CHG_DONE_STATE,  //charge done
    XPOWERS_AXP192_CHG_STOP_STATE,  //not charge
} xpowers_chg_status_t;

typedef enum {
    XPOWERS_AXP192_WAKEUP_IRQ_PIN_TO_LOW = _BV(4),
    XPOWERS_AXP192_WAKEUP_PWROK_TO_LOW   = _BV(3),
    XPOWERS_AXP192_WAKEUP_DC_DLO_SELECT  = _BV(2),
} xpowers_wakeup_t;

typedef enum {
    XPOWERS_AXP192_FAST_DCDC1,
    XPOWERS_AXP192_FAST_DCDC2,
    XPOWERS_AXP192_FAST_DCDC3,
    XPOWERS_AXP192_FAST_DCDC4,
    XPOWERS_AXP192_FAST_DCDC5,
    XPOWERS_AXP192_FAST_ALDO1,
    XPOWERS_AXP192_FAST_ALDO2,
    XPOWERS_AXP192_FAST_ALDO3,
    XPOWERS_AXP192_FAST_ALDO4,
    XPOWERS_AXP192_FAST_BLDO1,
    XPOWERS_AXP192_FAST_BLDO2,
    XPOWERS_AXP192_FAST_CPUSLDO,
    XPOWERS_AXP192_FAST_DLDO3,
    XPOWERS_AXP192_FAST_DLDO2,
} xpowers_fast_on_opt_t;


typedef enum {
    XPOWERS_AXP192_SEQUENCE_LEVEL_0,
    XPOWERS_AXP192_SEQUENCE_LEVEL_1,
    XPOWERS_AXP192_SEQUENCE_LEVEL_2,
    XPOWERS_AXP192_SEQUENCE_DISABLE,
} xpower_start_sequence_t;

typedef enum {
    XPOWERS_AXP192_WDT_IRQ_TO_PIN,             //Just interrupt to pin
    XPOWERS_AXP192_WDT_IRQ_AND_RSET,           //IRQ to pin and reset pmu system
    XPOWERS_AXP192_WDT_IRQ_AND_RSET_PD_PWROK,  //IRQ to pin and reset pmu system,pull down pwrok
    XPOWERS_AXP192_WDT_IRQ_AND_RSET_ALL_OFF,   //IRQ to pin and reset pmu system,turn off dcdc & ldo ,pull down pwrok
} xpowers_wdt_config_t;

typedef enum {
    XPOWERS_AXP192_WDT_TIMEOUT_1S,
    XPOWERS_AXP192_WDT_TIMEOUT_2S,
    XPOWERS_AXP192_WDT_TIMEOUT_4S,
    XPOWERS_AXP192_WDT_TIMEOUT_8S,
    XPOWERS_AXP192_WDT_TIMEOUT_16S,
    XPOWERS_AXP192_WDT_TIMEOUT_32S,
    XPOWERS_AXP192_WDT_TIMEOUT_64S,
    XPOWERS_AXP192_WDT_TIMEOUT_128S,
} xpowers_wdt_timeout_t;

typedef enum {
    XPOWERS_AXP192_VSYS_VOL_4V1,
    XPOWERS_AXP192_VSYS_VOL_4V2,
    XPOWERS_AXP192_VSYS_VOL_4V3,
    XPOWERS_AXP192_VSYS_VOL_4V4,
    XPOWERS_AXP192_VSYS_VOL_4V5,
    XPOWERS_AXP192_VSYS_VOL_4V6,
    XPOWERS_AXP192_VSYS_VOL_4V7,
    XPOWERS_AXP192_VSYS_VOL_4V8,
} xpower_chg_dpm_t;

typedef enum {
    XPOWER_POWERON_SRC_POWERON_LOW,                     //POWERON low for on level when POWERON Mode as POWERON Source
    XPOWER_POWERON_SRC_IRQ_LOW,                         //IRQ PIN Pull-down as POWERON Source
    XPOWER_POWERON_SRC_VBUS_INSERT,                     //Vbus Insert and Good as POWERON Source
    XPOWER_POWERON_SRC_BAT_CHARGE,                      //Vbus Insert and Good as POWERON Source
    XPOWER_POWERON_SRC_BAT_INSERT,                      //Battery Insert and Good as POWERON Source
    XPOWER_POWERON_SRC_ENMODE,                          //POWERON always high when EN Mode as POWERON Source
    XPOWER_POWERON_SRC_UNKONW,                          //Unkonw
} xpower_power_on_source_t;

typedef enum {
    XPOWER_POWEROFF_SRC_PWEKEY_PULLDOWN,            //POWERON Pull down for off level when POWERON Mode as POWEROFF Source
    XPOWER_POWEROFF_SRC_SOFT_OFF,                   //Software configuration as POWEROFF Source
    XPOWER_POWEROFF_SRC_PWEKEY_LOW,                 //POWERON always low when EN Mode as POWEROFF Source
    XPOWER_POWEROFF_SRC_UNDER_VSYS,                 //Vsys Under Voltage as POWEROFF Source
    XPOWER_POWEROFF_SRC_OVER_VBUS,                  //VBUS Over Voltage as POWEROFF Source
    XPOWER_POWEROFF_SRC_UNDER_VOL,                  //DCDC Under Voltage as POWEROFF Source
    XPOWER_POWEROFF_SRC_OVER_VOL,                   //DCDC Over Voltage as POWEROFF Source
    XPOWER_POWEROFF_SRC_OVER_TEMP,                  //Die Over Temperature as POWEROFF Source
    XPOWER_POWEROFF_SRC_UNKONW,                     //Unkonw
} xpower_power_off_source_t;

typedef enum {
    XPOWER_PWROK_DELAY_8MS,
    XPOWER_PWROK_DELAY_16MS,
    XPOWER_PWROK_DELAY_32MS,
    XPOWER_PWROK_DELAY_64MS,
} xpower_pwrok_delay_t;



// All these need updating
typedef enum __xpowers_axp192_irq {
    //! IRQ1 REG 40H
    XPOWERS_AXP192_BAT_NOR_UNDER_TEMP_IRQ   = _BV(0),   // Battery Under Temperature in Work
    XPOWERS_AXP192_BAT_NOR_OVER_TEMP_IRQ    = _BV(1),   // Battery Over Temperature in Work mode
    XPOWERS_AXP192_BAT_CHG_UNDER_TEMP_IRQ   = _BV(2),   // Battery Under Temperature in Charge mode IRQ(bcut_irq)
    XPOWERS_AXP192_BAT_CHG_OVER_TEMP_IRQ    = _BV(3),   // Battery Over Temperature in Charge mode IRQ(bcot_irq) enable
    XPOWERS_AXP192_GAUGE_NEW_SOC_IRQ        = _BV(4),   // Gauge New SOC IRQ(lowsoc_irq) enable (low state of charge)
    XPOWERS_AXP192_WDT_TIMEOUT_IRQ          = _BV(5),   // Gauge Watchdog Timeout IRQ(gwdt_irq) enable
    XPOWERS_AXP192_WARNING_LEVEL1_IRQ       = _BV(6),   // SOC drop to Warning Level1 IRQ(socwl1_irq) enable
    XPOWERS_AXP192_WARNING_LEVEL2_IRQ       = _BV(7),   // SOC drop to Warning Level2 IRQ(socwl2_irq) enable

    //! IRQ2 REG 41H
    XPOWERS_AXP192_PKEY_POSITIVE_IRQ        = _BV(8),   // POWERON Positive Edge IRQ(ponpe_irq_en) enable
    XPOWERS_AXP192_PKEY_NEGATIVE_IRQ        = _BV(9),   // POWERON Negative Edge IRQ(ponne_irq_en) enable
    XPOWERS_AXP192_PKEY_LONG_IRQ            = _BV(10),  // POWERON Long PRESS IRQ(ponlp_irq) enable
    XPOWERS_AXP192_PKEY_SHORT_IRQ           = _BV(11),  // POWERON Short PRESS IRQ(ponsp_irq_en) enable
    XPOWERS_AXP192_BAT_REMOVE_IRQ           = _BV(12),  // Battery Remove IRQ(bremove_irq) enable
    XPOWERS_AXP192_BAT_INSERT_IRQ           = _BV(13),  // Battery Insert IRQ(binsert_irq) enabl
    XPOWERS_AXP192_VBUS_REMOVE_IRQ          = _BV(14),  // VBUS Remove IRQ(vremove_irq) enabl
    XPOWERS_AXP192_VBUS_INSERT_IRQ          = _BV(15),  // VBUS Insert IRQ(vinsert_irq) enable

    //! IRQ3 REG 42H
    XPOWERS_AXP192_BAT_OVER_VOL_IRQ         = _BV(16),  // Battery Over Voltage Protection IRQ(bovp_irq) enable
    XPOWERS_AXP192_CHARGER_TIMER_IRQ        = _BV(17),  // Charger Safety Timer1/2 expire IRQ(chgte_irq) enable
    XPOWERS_AXP192_DIE_OVER_TEMP_IRQ        = _BV(18),  // DIE Over Temperature level1 IRQ(dotl1_irq) enable
    XPOWERS_AXP192_BAT_CHG_START_IRQ        = _BV(19),  // Charger start IRQ(chgst_irq) enable
    XPOWERS_AXP192_BAT_CHG_DONE_IRQ         = _BV(20),  // Battery charge done IRQ(chgdn_irq) enable
    XPOWERS_AXP192_BATFET_OVER_CURR_IRQ     = _BV(21),  // BATFET Over Current Protection IRQ(bocp_irq) enable
    XPOWERS_AXP192_LDO_OVER_CURR_IRQ        = _BV(22),  // LDO Over Current IRQ(ldooc_irq) enable
    XPOWERS_AXP192_WDT_EXPIRE_IRQ           = _BV(23),  // Watchdog Expire IRQ(wdexp_irq) enable

    XPOWERS_AXP192_ALL_IRQ                  = (0xFFFFFFFFUL)
} xpowers_axp192_irq_t;


// @brief Each chip resource is different,please refer to the table above
typedef enum __XPowersPowerChannel {

    XPOWERS_DCDC1,
    XPOWERS_DCDC2,
    XPOWERS_DCDC3,
    XPOWERS_DCDC4,
    XPOWERS_DCDC5,

    XPOWERS_LDO1,
    XPOWERS_LDO2,
    XPOWERS_LDO3,
    XPOWERS_LDO4,
    XPOWERS_LDO5,

    XPOWERS_LDOIO,

    XPOWERS_ALDO1,
    XPOWERS_ALDO2,
    XPOWERS_ALDO3,
    XPOWERS_ALDO4,

    XPOWERS_BLDO1,
    XPOWERS_BLDO2,

    XPOWERS_DLDO3,
    XPOWERS_DLDO2,

    XPOWERS_VBACKUP,

    XPOWERS_CPULDO,

} XPowersPowerChannel_t;


/**
 * @brief Charging led mode parameters.
 */
typedef enum __xpowers_chg_led_mode {
    XPOWERS_CHG_LED_OFF,
    XPOWERS_CHG_LED_BLINK_1HZ,
    XPOWERS_CHG_LED_BLINK_4HZ,
    XPOWERS_CHG_LED_ON,
    XPOWERS_CHG_LED_CTRL_CHG,    // The charging indicator is controlled by the charger
} xpowers_chg_led_mode_t;


/**
 * @brief AXP192 charge target voltage parameters.
 */
typedef enum __xpowers_axp192_chg_vol {
    XPOWERS_AXP192_CHG_VOL_4V1 = 0,
    XPOWERS_AXP192_CHG_VOL_4V15,
    XPOWERS_AXP192_CHG_VOL_4V2,
    XPOWERS_AXP192_CHG_VOL_MAX //(4.36)
} xpowers_axp192_chg_vol_t;

/**
 * @brief AXP192 charge currnet voltage parameters.
 */
typedef enum __xpowers_axp192_chg_curr {
    XPOWERS_AXP192_CHG_CUR_0MA,
    XPOWERS_AXP192_CHG_CUR_100MA = 4,
    XPOWERS_AXP192_CHG_CUR_125MA,
    XPOWERS_AXP192_CHG_CUR_150MA,
    XPOWERS_AXP192_CHG_CUR_175MA,
    XPOWERS_AXP192_CHG_CUR_200MA,
    XPOWERS_AXP192_CHG_CUR_300MA,
    XPOWERS_AXP192_CHG_CUR_400MA,
    XPOWERS_AXP192_CHG_CUR_500MA,
    XPOWERS_AXP192_CHG_CUR_600MA,
    XPOWERS_AXP192_CHG_CUR_700MA,
    XPOWERS_AXP192_CHG_CUR_800MA,
    XPOWERS_AXP192_CHG_CUR_900MA,
    XPOWERS_AXP192_CHG_CUR_1000MA,
} xpowers_axp192_chg_curr_t;



/**
 * @brief PMU PEKEY Press off time parameters.
 */
typedef enum __xpowers_press_off_time {
    XPOWERS_POWEROFF_4S,
    XPOWERS_POWEROFF_6S,
    XPOWERS_POWEROFF_8S,
    XPOWERS_POWEROFF_10S,
} xpowers_press_off_time_t;


/**
 * @brief PMU PEKEY Press on time parameters.
 */
typedef enum __xpowers_press_on_time {
    XPOWERS_POWERON_128MS,
    XPOWERS_POWERON_512MS,
    XPOWERS_POWERON_1S,
    XPOWERS_POWERON_2S,
} xpowers_press_on_time_t;