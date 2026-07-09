idf_component_register(
    SRCS
        "control.c"
        "init.c"
        "pwm.c"
        
    INCLUDE_DIRS
        "include"

    PRIV_REQUIRES
        esp_driver_ledc
)
