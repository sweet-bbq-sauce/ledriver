#include <driver/ledc.h>
#include <esp_err.h>

#define RGB_LEDC_MODE    LEDC_LOW_SPEED_MODE
#define RGB_LEDC_TIMER   LEDC_TIMER_0
#define RGB_LEDC_FREQ_HZ 1000
#define RGB_LEDC_RES     LEDC_TIMER_16_BIT

esp_err_t ledriver_led_init(int red_gpio, int green_gpio, int blue_gpio) {
    esp_err_t err;

    const ledc_timer_config_t timer = {.speed_mode = RGB_LEDC_MODE,
                                       .timer_num = RGB_LEDC_TIMER,
                                       .duty_resolution = RGB_LEDC_RES,
                                       .freq_hz = RGB_LEDC_FREQ_HZ,
                                       .clk_cfg = LEDC_AUTO_CLK};

    err = ledc_timer_config(&timer);
    if (err != ESP_OK) {
        return err;
    }

    const ledc_channel_t channels[3] = {LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2};

    const int gpios[3] = {red_gpio, green_gpio, blue_gpio};

    for (int i = 0; i < 3; i++) {
        const ledc_channel_config_t channel = {.gpio_num = gpios[i],
                                               .speed_mode = RGB_LEDC_MODE,
                                               .channel = channels[i],
                                               .timer_sel = RGB_LEDC_TIMER,
                                               .duty = 0,
                                               .hpoint = 0};

        err = ledc_channel_config(&channel);
        if (err != ESP_OK) {
            return err;
        }
    }

    return ESP_OK;
}
