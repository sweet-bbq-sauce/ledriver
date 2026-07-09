#pragma once

#include <stdint.h>

#include <esp_err.h>

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} ledriver_led_color_t;

typedef struct {
    ledriver_led_color_t color;
    bool enabled;
} ledriver_led_state_t;

esp_err_t ledriver_led_init(int red_gpio, int green_gpio, int blue_gpio);

esp_err_t ledriver_led_get_state(ledriver_led_state_t *state);
esp_err_t ledriver_led_set_state(const ledriver_led_state_t *state);

esp_err_t ledriver_led_get_color(ledriver_led_color_t *color);
esp_err_t ledriver_led_set_color(const ledriver_led_color_t *color);

esp_err_t ledriver_led_get_power(bool *enabled);
esp_err_t ledriver_led_set_power(bool enabled);
