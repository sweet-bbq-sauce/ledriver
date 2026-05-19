#pragma once

#include <esp_err.h>

typedef enum {
    LEDRIVER_WIFI_STATE_IDLE = 0,
    LEDRIVER_WIFI_STATE_CONNECTING,
    LEDRIVER_WIFI_STATE_CONNECTED,
    LEDRIVER_WIFI_STATE_DISCONNECTED,
    LEDRIVER_WIFI_STATE_FAILED
} ledriver_wifi_state_t;

typedef struct {
    const char *ssid;
    const char *password;
    int max_retries;
    int connect_timeout_ms;
} ledriver_wifi_config_t;

esp_err_t ledriver_wifi_init(void);
esp_err_t ledriver_wifi_start(const ledriver_wifi_config_t *config);
esp_err_t ledriver_wifi_stop(void);

ledriver_wifi_state_t ledriver_wifi_get_state(void);
