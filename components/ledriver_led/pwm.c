#include <esp_err.h>
#include <driver/ledc.h>

#include <ledriver/led.h>

esp_err_t ledriver_pwm_set(const ledriver_led_color_t *color) {
    esp_err_t err;

    if (!color) {
        return ESP_ERR_INVALID_ARG;
    }

    // RED channel
    err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, color->r);
    if (err != ESP_OK) {
        return err;
    }

    err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    if (err != ESP_OK) {
        return err;
    }

    // GREEN channel
    err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, color->g);
    if (err != ESP_OK) {
        return err;
    }

    err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    if (err != ESP_OK) {
        return err;
    }

    // BLUE channel
    err = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, color->b);
    if (err != ESP_OK) {
        return err;
    }

    err = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);
    if (err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}
