#include "powerhub_light.h"


namespace esphome {
namespace powerhub {

static const char *TAG = "powerhub.light";

/// @brief convert the float input to (0 - scale_factor) uint8_t value
/// @param value input float value
/// @param scale_factor scaling factor
/// @return uint8_t value
static uint8_t to_uint8_t(float value, int scale_factor) {
    if ( value < -1e-6f || value > 1.0f + 1e-6f ) {
        ESP_LOGW(TAG, "Invalid range, support 0.0 - 1.0 only");
    }
    return static_cast<uint8_t>(value * scale_factor);
}

light::LightTraits USBCLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void USBCLight::write_state(light::LightState *state) {
    float r, g, b;
    float brightness;

    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_usb_c_color(color);
    this->parent_->set_brightness_usb_c(to_uint8_t(brightness, 100));

    state->publish_state();
}

light::LightTraits USBALight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void USBALight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_usb_a_color(color);
    this->parent_->set_brightness_usb_a(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits GroveBlueLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void GroveBlueLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_grove_blue_color(color);
    this->parent_->set_brightness_grove_blue(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits GroveRedLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void GroveRedLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_grove_red_color(color);
    this->parent_->set_brightness_grove_red(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits RS485CANLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void RS485CANLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_rs485_can_color(color);
    this->parent_->set_brightness_rs485_can(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits BATChargeLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void BATChargeLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_bat_charge_color(color);
    this->parent_->set_brightness_bat_charge(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits PowerLeftLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void PowerLeftLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_pwr_l_color(color);
    this->parent_->set_brightness_pwr_l(to_uint8_t(brightness, 100));
    state->publish_state();
}

light::LightTraits PowerRightLight::get_traits() {
    auto traits = light::LightTraits();
    traits.set_supported_color_modes({light::ColorMode::RGB});
    return traits;
}

void PowerRightLight::write_state(light::LightState *state) {
    float r, g, b, brightness;
    state->current_values_as_rgb(&r, &g, &b);
    state->current_values_as_brightness(&brightness);

    BGR_t color {
        .b = to_uint8_t(b, 255),
        .g = to_uint8_t(g, 255),
        .r = to_uint8_t(r, 255)
    };

    this->parent_->set_led_pwr_r_color(color);
    this->parent_->set_brightness_pwr_r(to_uint8_t(brightness, 100));
    state->publish_state();
}

} // namespace powerhub
} // namespace esphome
