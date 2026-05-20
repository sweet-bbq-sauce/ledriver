#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <esp_err.h>

#define LEDRIVER_WEBPANEL_MOUNT_POINT "/webpanel"

typedef struct {
    uint64_t total_bytes;
    uint64_t used_bytes;
    int percent_used;
} ledriver_panel_partition_info_t;

esp_err_t ledriver_panel_partition_mount(void);
esp_err_t ledriver_panel_partition_unmount(void);

bool ledriver_panel_partition_is_mounted(void);

esp_err_t ledriver_panel_partition_get_info(ledriver_panel_partition_info_t *info);
