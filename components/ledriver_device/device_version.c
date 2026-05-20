#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include <esp_err.h>

#include <ledriver/device_info.h>
#include <ledriver/device_version.h>
#include <ledriver/panel_partition.h>

static uint32_t get_webpanel_build_number() {
    if (!ledriver_panel_partition_is_mounted()) {
        return 0;
    }

    FILE *build_number_file = fopen(LEDRIVER_WEBPANEL_MOUNT_POINT "/build_number", "r");
    if (build_number_file == NULL) {
        return 0;
    }

    uint32_t build_number = 0;
    if (fscanf(build_number_file, "%" PRIu32, &build_number) != 1) {
        fclose(build_number_file);
        return 0;
    }

    fclose(build_number_file);
    return build_number;
}

esp_err_t ledriver_device_version_get(ledriver_device_version_t *version) {
    if (version == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    version->hardware_version = LEDRIVER_HARDWARE_VERSION;
    version->firmware_build_number = LEDRIVER_FIRMWARE_BUILD_NUMBER;
    version->webpanel_build_number = get_webpanel_build_number();

    return ESP_OK;
}
