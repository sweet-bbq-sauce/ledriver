#pragma once

#include <esp_err.h>

#define LEDRIVER_APP_CONFIG_WIFI_SSID_MAX       32
#define LEDRIVER_APP_CONFIG_WIFI_PASSWORD_MAX   63
#define LEDRIVER_APP_CONFIG_UPDATE_BASE_URL_MAX 255

typedef struct {
    char wifi_ssid[LEDRIVER_APP_CONFIG_WIFI_SSID_MAX + 1];
    char wifi_password[LEDRIVER_APP_CONFIG_WIFI_PASSWORD_MAX + 1];
    char update_base_url[LEDRIVER_APP_CONFIG_UPDATE_BASE_URL_MAX + 1];
} ledriver_app_config_t;

esp_err_t ledriver_app_config_init(void);

esp_err_t ledriver_app_config_load(ledriver_app_config_t *config);
esp_err_t ledriver_app_config_save(const ledriver_app_config_t *config);

esp_err_t ledriver_app_config_reset(void);
