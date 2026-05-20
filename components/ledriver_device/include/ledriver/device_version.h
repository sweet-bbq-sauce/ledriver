#pragma once

#include <stdint.h>

#include <esp_err.h>

typedef struct {
    const char *hardware_version;
    uint32_t firmware_build_number;
    uint32_t webpanel_build_number;
} ledriver_device_version_t;

esp_err_t ledriver_device_version_get(ledriver_device_version_t *version);
