idf_component_register(
    SRCS
        "main.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        fatfs

        ledriver_app_config
        ledriver_wifi
        ledriver_device
        ledriver_panel_partition
)
