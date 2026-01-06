Example yaml file


substitutions:
  friendly_name: "AXP192_test" #Core2
  esphome_name: "axp192test"
  sensor_update: "10s" #Usually 60s but shortened for demo purposes

esphome:
  name: ${esphome_name}
  build_path: C:/EsphomeTemp/${esphome_name}

esp32:
  board: m5stack-core2
  framework:
    type: esp-idf
  flash_size: 16MB # Need to manually override defaults so M5STACK FIRE memory is correct

psram: #Need to include this so logs will display PSRAM detected, note only 4MB detected even though 8MB exists (probably because himem not implemented)
  mode: quad
  speed: 80MHz

api:

wifi:
  networks:
    - ssid: !secret wifi_comp_ssid
      password: !secret wifi_comp_password
    - ssid: !secret wifi_home_ssid
      password: !secret wifi_home_password
    - ssid: !secret wifi_phone_ssid
      password: !secret wifi_phone_password
    - ssid: !secret wifi_bshed_ssid
      password: !secret wifi_bshed_password

# Enable logging
logger:
  logs:
    component: DEBUG 

# Define I/O: I²C and SPI
i2c:
  id: bus_internal
  sda: 21
  scl: 22
  scan: true

spi:
  clk_pin: 18
  mosi_pin: 23
  miso_pin: 38

external_components:
  # - source: C:\Users\jims9\OneDrive\Documents\ESPComponents\esphome-yaml\components
  - source: github://jimwsmith/esphome-yaml/components
    components: [ axp192 ]
    refresh: 0s

axp192:
  id: axp192_pmu
  i2c_id: bus_internal

binary_sensor:

sensor:
# PMU
  - platform: axp192
    update_interval: ${sensor_update}
    id: axp192_sensor
    battery_voltage:
      name: "Battery Voltage"
    usb_voltage:
      name: "USB Voltage"
    usb_current:
      name: "USB Current"
    vbus_voltage:
      name: "VBus Voltage"
    vbus_current:
      name: "VBus Current"
    internal_temperature:
      name: "Int Temp"
    battery_power:
      name: "Battery Power"
    battery_charge_current:
      name: "Batt Charge I"
    battery_discharge_current:
      name: "Batt Discharge I"
    aps_voltage:
      name: "APS Voltage"
    battery_charging:
      id: charger_connected
      name: "Battery Charging"
    battery_level:
      name: "Battery Level"
      id: battery_percent

#Some useful standard sensors
  - platform: wifi_signal # Reports the WiFi signal strength/RSSI in dB
    icon: mdi:wifi
    name: "WiFi Signal dB"
    id: wifi_signal_db
    unit_of_measurement: "dB"
    update_interval: ${sensor_update}
    entity_category: "diagnostic"
                
  - platform: uptime
    name: ${esphome_name} Uptime

text_sensor:
  - platform: wifi_info
    ssid:
      icon: mdi:wifi
      id: wifi_ssid
      name: WiFi SSID
    dns_address:
      icon: mdi:wifi
      name: WiFi DNS Address
    ip_address:
      icon: mdi:wifi
      id: wifi_ip
      name: WiFi IP Address
      entity_category: "diagnostic"

output:
  - platform: axp192
    type: range
    channel: DCDC3
    id: lcd_backlight_output
    min_voltage: 2100
    max_voltage: 3500

  - platform: axp192
    channel: DLDO3
    id: vibration_motor
    voltage: 3300

  - platform: axp192
    channel: DLDO2
    id: lcd_power
    voltage: 3000

touchscreen:
  - platform: ft63x6
    id: touch
    # reset_pin:
    #   aw9523b: aw9523b_hub
    #   number: 0
    calibration:
      x_min: 0
      x_max: 320
      y_min: 0
      y_max: 240

display:
  - platform: mipi_spi
    model: M5CORE
    dc_pin:
      number: GPIO15
      ignore_strapping_warning: true
#     reset_pin:
#       aw9523b: aw9523b_hub
#       number: 9
    cs_pin: 
      number: GPIO5
      ignore_strapping_warning: true
    data_rate: 40MHz
    invert_colors: true
    id: m5core_lcd
    show_test_card: true

light:
  - platform: monochromatic
    id: display_backlight
    name: "LCD Backlight"
    icon: "mdi:television"
    entity_category: config
    output: lcd_backlight_output
    restore_mode: ALWAYS_ON #Always want to have a backlight on when we boot up
    default_transition_length: 250ms
  - platform: binary
    name: "Vibration"
    output: vibration_motor
  - platform: binary
    name: "LCD Power"
    output: lcd_power
    restore_mode: ALWAYS_ON #Always want to have a backlight on when we boot up

switch:
  - platform: axp192
    led_pwr:
      name: "Green LED"
#    name: "Vibration"
#    output: vibration_motor

