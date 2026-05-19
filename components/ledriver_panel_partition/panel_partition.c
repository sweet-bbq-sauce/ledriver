#include <stdbool.h>

#include <esp_err.h>
#include <esp_vfs_fat.h>

#include <ledriver/panel_partition.h>

static bool is_mounted = false;

#define LEDRIVER_PANEL_PARTITION_LABEL "webpanel"
#define LEDRIVER_PANEL_BASE_PATH       "/webpanel"

esp_err_t ledriver_panel_partition_mount(void) {
    if (is_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    const esp_vfs_fat_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 8,
        .allocation_unit_size = 0,
    };

    esp_err_t err = esp_vfs_fat_spiflash_mount_ro(LEDRIVER_PANEL_BASE_PATH,
                                                  LEDRIVER_PANEL_PARTITION_LABEL,
                                                  &mount_config);
    if (err != ESP_OK) {
        return err;
    }

    is_mounted = true;
    return ESP_OK;
}

esp_err_t ledriver_panel_partition_unmount(void) {
    if (!is_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t err =
        esp_vfs_fat_spiflash_unmount_ro(LEDRIVER_PANEL_BASE_PATH, LEDRIVER_PANEL_PARTITION_LABEL);
    if (err != ESP_OK) {
        return err;
    }

    is_mounted = false;
    return ESP_OK;
}

bool ledriver_panel_partition_is_mounted(void) {
    return is_mounted;
}

esp_err_t ledriver_panel_partition_get_info(ledriver_panel_partition_info_t *info) {
    if (!is_mounted) {
        return ESP_ERR_INVALID_STATE;
    }

    uint64_t total_bytes, free_bytes;
    esp_err_t err = esp_vfs_fat_info(LEDRIVER_PANEL_BASE_PATH, &total_bytes, &free_bytes);
    if (err != ESP_OK) {
        return err;
    }

    info->total_bytes = total_bytes;
    info->used_bytes = total_bytes - free_bytes;
    info->percent_used = (total_bytes > 0) ? (info->used_bytes * 100 / total_bytes) : 0;

    return ESP_OK;
}
