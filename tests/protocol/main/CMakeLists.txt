idf_component_register(
    SRCS
        "test_parser.c"
        "test_serialize.c"
        "test_runner.c"

    PRIV_REQUIRES
        unity
        ledriver_protocol
)
