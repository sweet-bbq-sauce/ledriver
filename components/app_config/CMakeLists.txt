idf_component_register(
    SRCS
        "init.c"
        "load.c"
        "save.c"
        "reset.c"
        
    INCLUDE_DIRS
        "include"

    PRIV_REQUIRES
        nvs_flash
)

nvs_create_partition_image(
    factory_nvs
    "factory_nvs.csv"
    FLASH_IN_PROJECT
)
