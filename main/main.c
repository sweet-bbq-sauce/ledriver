#include <dirent.h>
#include <sys/stat.h>

#include <esp_log.h>
#include <esp_spiffs.h>

#include <ledriver_app_config.h>
#include <ledriver_device_info.h>
#include <ledriver_wifi.h>

static const char *TAG = "webpanel";
static const char *SPIFFS_PARTITION_LABEL = "webpanel";
static const char *SPIFFS_BASE_PATH = "/webpanel";

static void list_www_files(void) {
    DIR *dir = opendir(SPIFFS_BASE_PATH);
    if (!dir) {
        ESP_LOGE(TAG, "Failed to open %s", SPIFFS_BASE_PATH);
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char path[512];
        struct stat st;

        snprintf(path, sizeof(path), "%s/%s", SPIFFS_BASE_PATH, entry->d_name);

        if (stat(path, &st) == 0) {
            ESP_LOGI(TAG, "%s (%ld bytes)", path, (long)st.st_size);
        } else {
            ESP_LOGI(TAG, "%s", path);
        }
    }

    closedir(dir);
}

void app_main(void) {
    const esp_vfs_spiffs_conf_t conf = {.base_path = SPIFFS_BASE_PATH,
                                        .partition_label = SPIFFS_PARTITION_LABEL,
                                        .max_files = 8,
                                        .format_if_mount_failed = false};

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    ESP_LOGI(TAG, "Hardware version: %s", LEDRIVER_HARDWARE_VERSION);
    ESP_LOGI(TAG, "Firmware build number: %d", LEDRIVER_FIRMWARE_BUILD_NUMBER);

    size_t total = 0;
    size_t used = 0;
    ESP_ERROR_CHECK(esp_spiffs_info(SPIFFS_PARTITION_LABEL, &total, &used));

    ESP_LOGI(TAG, "SPIFFS total: %d, used: %d", total, used);

    list_www_files();

    esp_err_t err = ledriver_app_config_init();
    if (err == ESP_OK) {
        ledriver_app_config_t config;
        err = ledriver_app_config_load(&config);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Loaded config:");
            ESP_LOGI(TAG, "  WiFi SSID: %s", config.wifi_ssid);
            ESP_LOGI(TAG, "  Update Base URL: %s", config.update_base_url);

            const ledriver_wifi_config_t wifi_config = {.ssid = config.wifi_ssid,
                                                        .password = config.wifi_password,
                                                        .max_retries = 5,
                                                        .connect_timeout_ms = 10000};

            err = ledriver_wifi_start(&wifi_config);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Failed to start WiFi: %s", esp_err_to_name(err));
            }

        } else {
            ESP_LOGE(TAG, "Failed to load config: %s", esp_err_to_name(err));
        }
    } else {
        ESP_LOGE(TAG, "Failed to initialize config: %s", esp_err_to_name(err));
        return;
    }
}
