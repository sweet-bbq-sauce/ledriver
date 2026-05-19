#include <ledriver/app_config.h>

#include <esp_err.h>
#include <nvs.h>

#include "nvs_config.h"

static esp_err_t read_string(const char *key,
                             char *dest,
                             size_t dest_size,
                             nvs_handle_t factory_handle,
                             nvs_handle_t user_handle) {
    if (key == NULL || dest == NULL || dest_size == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t len = dest_size;
    esp_err_t err = nvs_get_str(user_handle, key, dest, &len);

    if (err == ESP_OK) {
        return ESP_OK;
    }

    if (err != ESP_ERR_NVS_NOT_FOUND) {
        return err;
    }

    len = dest_size;
    err = nvs_get_str(factory_handle, key, dest, &len);

    if (err == ESP_OK) {
        return ESP_OK;
    }

    return err;
}

esp_err_t ledriver_app_config_load(ledriver_app_config_t *config) {
    nvs_handle_t factory_handle;
    nvs_handle_t user_handle;

    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t err = nvs_open_from_partition(LEDRIVER_APP_CONFIG_FACTORY_PARTITION,
                                            LEDRIVER_APP_CONFIG_NAMESPACE,
                                            NVS_READONLY,
                                            &factory_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_open_from_partition(LEDRIVER_APP_CONFIG_USER_PARTITION,
                                  LEDRIVER_APP_CONFIG_NAMESPACE,
                                  NVS_READWRITE,
                                  &user_handle);
    if (err != ESP_OK) {
        nvs_close(factory_handle);
        return err;
    }

    err = read_string(LEDRIVER_APP_CONFIG_KEY_WIFI_SSID,
                      config->wifi_ssid,
                      sizeof(config->wifi_ssid),
                      factory_handle,
                      user_handle);

    if (err != ESP_OK) {
        goto cleanup;
    }

    err = read_string(LEDRIVER_APP_CONFIG_KEY_WIFI_PASSWORD,
                      config->wifi_password,
                      sizeof(config->wifi_password),
                      factory_handle,
                      user_handle);

    if (err != ESP_OK) {
        goto cleanup;
    }

    err = read_string(LEDRIVER_APP_CONFIG_KEY_UPDATE_BASE_URL,
                      config->update_base_url,
                      sizeof(config->update_base_url),
                      factory_handle,
                      user_handle);

    if (err != ESP_OK) {
        goto cleanup;
    }

cleanup:
    nvs_close(factory_handle);
    nvs_close(user_handle);
    return err;
}