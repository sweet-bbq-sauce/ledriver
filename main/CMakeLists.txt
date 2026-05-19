idf_component_register(
    SRCS
        "main.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        spiffs
        ledriver_app_config
        ledriver_wifi
        ledriver_device
)
