#include <ledriver/wifi.h>

#include <stdbool.h>
#include <string.h>

#include <esp_event.h>
#include <esp_log.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

static const char *TAG = "wifi";

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAILED_BIT    BIT1

#define DEFAULT_MAX_RETRIES        5
#define DEFAULT_CONNECT_TIMEOUT_MS 15000

static EventGroupHandle_t s_event_group;
static esp_netif_t *s_netif;
static ledriver_wifi_state_t s_state = LEDRIVER_WIFI_STATE_IDLE;

static bool s_initialized;
static bool s_started;
static int s_retry_count;
static int s_max_retries;

static esp_event_handler_instance_t s_wifi_event_instance;
static esp_event_handler_instance_t s_ip_event_instance;

static void set_state(ledriver_wifi_state_t state) {
    s_state = state;
}

static void
wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        set_state(LEDRIVER_WIFI_STATE_CONNECTING);
        esp_wifi_connect();
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (!s_started) {
            set_state(LEDRIVER_WIFI_STATE_IDLE);
            return;
        }

        if (s_retry_count < s_max_retries) {
            s_retry_count++;
            set_state(LEDRIVER_WIFI_STATE_CONNECTING);
            ESP_LOGW(TAG, "WiFi disconnected, retrying %d/%d", s_retry_count, s_max_retries);
            esp_wifi_connect();
        } else {
            set_state(LEDRIVER_WIFI_STATE_FAILED);
            xEventGroupSetBits(s_event_group, WIFI_FAILED_BIT);
        }

        return;
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        const ip_event_got_ip_t *event = event_data;

        s_retry_count = 0;
        set_state(LEDRIVER_WIFI_STATE_CONNECTED);

        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_event_group, WIFI_CONNECTED_BIT);
    }
}

static esp_err_t validate_config(const ledriver_wifi_config_t *config) {
    if (config == NULL || config->ssid == NULL || config->ssid[0] == '\0') {
        return ESP_ERR_INVALID_ARG;
    }

    size_t ssid_len = strlen(config->ssid);
    if (ssid_len > sizeof(((wifi_config_t *)0)->sta.ssid)) {
        return ESP_ERR_INVALID_ARG;
    }

    if (config->password != NULL) {
        size_t password_len = strlen(config->password);
        if (password_len >= sizeof(((wifi_config_t *)0)->sta.password)) {
            return ESP_ERR_INVALID_ARG;
        }
    }

    return ESP_OK;
}

esp_err_t ledriver_wifi_init(void) {
    if (s_initialized) {
        return ESP_OK;
    }

    s_event_group = xEventGroupCreate();
    if (s_event_group == NULL) {
        return ESP_ERR_NO_MEM;
    }

    esp_err_t err = esp_netif_init();
    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    s_netif = esp_netif_create_default_wifi_sta();
    if (s_netif == NULL) {
        return ESP_FAIL;
    }

    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();

    err = esp_wifi_init(&init_config);
    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_handler_instance_register(WIFI_EVENT,
                                              ESP_EVENT_ANY_ID,
                                              wifi_event_handler,
                                              NULL,
                                              &s_wifi_event_instance);
    if (err != ESP_OK) {
        return err;
    }

    err = esp_event_handler_instance_register(IP_EVENT,
                                              IP_EVENT_STA_GOT_IP,
                                              wifi_event_handler,
                                              NULL,
                                              &s_ip_event_instance);
    if (err != ESP_OK) {
        return err;
    }

    s_initialized = true;
    set_state(LEDRIVER_WIFI_STATE_IDLE);

    return ESP_OK;
}

esp_err_t ledriver_wifi_start(const ledriver_wifi_config_t *config) {
    esp_err_t err = validate_config(config);
    if (err != ESP_OK) {
        return err;
    }

    err = ledriver_wifi_init();
    if (err != ESP_OK) {
        return err;
    }

    s_retry_count = 0;
    s_max_retries = config->max_retries > 0 ? config->max_retries : DEFAULT_MAX_RETRIES;

    const int timeout_ms =
        config->connect_timeout_ms > 0 ? config->connect_timeout_ms : DEFAULT_CONNECT_TIMEOUT_MS;

    xEventGroupClearBits(s_event_group, WIFI_CONNECTED_BIT | WIFI_FAILED_BIT);

    wifi_config_t wifi_config = {0};

    size_t ssid_len = strlen(config->ssid);
    memcpy(wifi_config.sta.ssid, config->ssid, ssid_len);

    if (config->password != NULL) {
        size_t password_len = strlen(config->password);
        memcpy(wifi_config.sta.password, config->password, password_len);

        if (password_len > 0) {
            wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        }
    }

    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;

    err = esp_wifi_set_mode(WIFI_MODE_STA);
    if (err != ESP_OK) {
        return err;
    }

    err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if (err != ESP_OK) {
        return err;
    }

    s_started = true;
    set_state(LEDRIVER_WIFI_STATE_CONNECTING);

    err = esp_wifi_start();
    if (err != ESP_OK && err != ESP_ERR_WIFI_CONN) {
        s_started = false;
        set_state(LEDRIVER_WIFI_STATE_FAILED);
        return err;
    }

    EventBits_t bits = xEventGroupWaitBits(s_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAILED_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           pdMS_TO_TICKS(timeout_ms));

    if (bits & WIFI_CONNECTED_BIT) {
        return ESP_OK;
    }

    if (bits & WIFI_FAILED_BIT) {
        return ESP_FAIL;
    }

    set_state(LEDRIVER_WIFI_STATE_FAILED);
    return ESP_ERR_TIMEOUT;
}

esp_err_t ledriver_wifi_stop(void) {
    if (!s_initialized || !s_started) {
        set_state(LEDRIVER_WIFI_STATE_IDLE);
        return ESP_OK;
    }

    s_started = false;
    s_retry_count = 0;

    esp_err_t err = esp_wifi_stop();
    if (err != ESP_OK && err != ESP_ERR_WIFI_NOT_INIT) {
        return err;
    }

    xEventGroupClearBits(s_event_group, WIFI_CONNECTED_BIT | WIFI_FAILED_BIT);
    set_state(LEDRIVER_WIFI_STATE_IDLE);

    return ESP_OK;
}

ledriver_wifi_state_t ledriver_wifi_get_state(void) {
    return s_state;
}
