#include <app_config.h>

#include <esp_err.h>
#include <nvs.h>

#include "nvs_config.h"

esp_err_t ledriver_app_config_reset(void) {
    nvs_handle_t user_handle;
    esp_err_t err = nvs_open_from_partition(LEDRIVER_APP_CONFIG_USER_PARTITION,
                                            LEDRIVER_APP_CONFIG_NAMESPACE,
                                            NVS_READWRITE,
                                            &user_handle);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return ESP_OK;
    }

    if (err != ESP_OK) {
        return err;
    }

    err = nvs_erase_all(user_handle);
    if (err != ESP_OK) {
        goto cleanup;
    }

    err = nvs_commit(user_handle);

cleanup:
    nvs_close(user_handle);
    return err;
}
