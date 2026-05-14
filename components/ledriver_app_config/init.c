#include <ledriver_app_config.h>

#include <esp_err.h>
#include <nvs.h>
#include <nvs_flash.h>

#include "nvs_config.h"

esp_err_t ledriver_app_config_init(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        err = nvs_flash_erase();
        if (err != ESP_OK) {
            return err;
        }
        err = nvs_flash_init();
    }

    if (err != ESP_OK) {
        return err;
    }

    return nvs_flash_init_partition(LEDRIVER_APP_CONFIG_FACTORY_PARTITION);
}
