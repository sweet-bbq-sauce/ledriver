#include <app_config.h>

#include <esp_err.h>
#include <nvs.h>

#include "nvs_config.h"

esp_err_t ledriver_app_config_save(const ledriver_app_config_t *config) {
    if (config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    nvs_handle_t user_handle;
    esp_err_t err = nvs_open_from_partition(LEDRIVER_APP_CONFIG_USER_PARTITION,
                                            LEDRIVER_APP_CONFIG_NAMESPACE,
                                            NVS_READWRITE,
                                            &user_handle);
    if (err != ESP_OK) {
        return err;
    }

    err = nvs_set_str(user_handle, LEDRIVER_APP_CONFIG_KEY_WIFI_SSID, config->wifi_ssid);
    if (err != ESP_OK) {
        goto cleanup;
    }

    err = nvs_set_str(user_handle, LEDRIVER_APP_CONFIG_KEY_WIFI_PASSWORD, config->wifi_password);
    if (err != ESP_OK) {
        goto cleanup;
    }

    err =
        nvs_set_str(user_handle, LEDRIVER_APP_CONFIG_KEY_UPDATE_BASE_URL, config->update_base_url);
    if (err != ESP_OK) {
        goto cleanup;
    }

    err = nvs_commit(user_handle);

cleanup:
    nvs_close(user_handle);
    return err;
}