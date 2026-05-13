#include <dirent.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include <sys/stat.h>

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

    size_t total = 0;
    size_t used = 0;
    ESP_ERROR_CHECK(esp_spiffs_info(SPIFFS_PARTITION_LABEL, &total, &used));

    ESP_LOGI(TAG, "SPIFFS total: %d, used: %d", total, used);

    list_www_files();
}
