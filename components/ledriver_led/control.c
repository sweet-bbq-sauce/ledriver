#include <esp_err.h>

#include <ledriver/led.h>

static ledriver_led_color_t d_color = {.r = 0, .g = 0, .b = 0};
static bool d_enabled = false;

extern esp_err_t ledriver_pwm_set(const ledriver_led_color_t *color);

esp_err_t ledriver_led_get_state(ledriver_led_state_t *state) {
    if (!state) {
        return ESP_ERR_INVALID_ARG;
    }
    state->color = d_color;
    state->enabled = d_enabled;
    return ESP_OK;
}

esp_err_t ledriver_led_set_state(const ledriver_led_state_t *state) {
    esp_err_t err;
    ledriver_led_color_t old_color;
    bool old_enabled;

    if (!state) {
        return ESP_ERR_INVALID_ARG;
    }

    old_color = d_color;
    old_enabled = d_enabled;

    err = ledriver_led_set_color(&state->color);
    if (err != ESP_OK) {
        return err;
    }

    err = ledriver_led_set_power(state->enabled);
    if (err != ESP_OK) {
        d_color = old_color;
        d_enabled = old_enabled;
        return err;
    }

    return ESP_OK;
}

esp_err_t ledriver_led_get_color(ledriver_led_color_t *color) {
    if (!color) {
        return ESP_ERR_INVALID_ARG;
    }
    *color = d_color;
    return ESP_OK;
}

esp_err_t ledriver_led_set_color(const ledriver_led_color_t *color) {
    esp_err_t err;

    if (!color) {
        return ESP_ERR_INVALID_ARG;
    }

    d_color = *color;

    if (d_enabled) {
        err = ledriver_pwm_set(&d_color);
        if (err != ESP_OK) {
            return err;
        }
    }

    return ESP_OK;
}

esp_err_t ledriver_led_set_power(bool enabled) {
    esp_err_t err;

    if (d_enabled == enabled) {
        return ESP_OK;
    }

    if (enabled) {
        err = ledriver_pwm_set(&d_color);
    } else {
        err = ledriver_pwm_set(&(ledriver_led_color_t){.r = 0, .g = 0, .b = 0});
    }

    if (err != ESP_OK) {
        return err;
    }

    d_enabled = enabled;

    return ESP_OK;
}

esp_err_t ledriver_led_get_power(bool *enabled) {
    if (!enabled) {
        return ESP_ERR_INVALID_ARG;
    }

    *enabled = d_enabled;
    return ESP_OK;
}
