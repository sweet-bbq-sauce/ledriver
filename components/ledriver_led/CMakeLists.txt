idf_component_register(
    SRCS
        "init.c"
        
    INCLUDE_DIRS
        "include"

    PRIV_REQUIRES
        esp_driver_ledc
)
