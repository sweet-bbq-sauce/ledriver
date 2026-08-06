#include <stdint.h>

#include <unity.h>

#include <ledriver/protocol.h>
#include <ledriver/protocol_serialize.h>

TEST_CASE("serializes a valid LDR header", "[parser]") {
    const ledriver_ldr_header_t header = {
        .version = 0x00,
        .request_id = 0x12345678,
        .message_type = LDR_MESSAGE_TYPE_REQUEST,
        .return_value = LDR_RETURN_VALUE_SUCCESS,
        .command = LDR_COMMAND_STATUS,
    };

    uint8_t output[LDR_HEADER_WIRE_SIZE];

    int result = ledriver_ldr_serialize_header(&header, output, sizeof(output));

    const uint8_t expected[] = {
        0x4C,
        0x44,
        0x52,
        0x00,
        0x12,
        0x34,
        0x56,
        0x78,
        0x00,
        LDR_COMMAND_STATUS,
    };

    TEST_ASSERT_EQUAL_INT(LDR_HEADER_WIRE_SIZE, result);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, output, sizeof(expected));
}

TEST_CASE("rejects buffer shorter than header", "[parser]") {
    const ledriver_ldr_header_t header = {0};
    uint8_t output[LDR_HEADER_WIRE_SIZE - 1];

    int result = ledriver_ldr_serialize_header(&header, output, sizeof(output));

    TEST_ASSERT_EQUAL_INT(-1, result);
}

TEST_CASE("rejects null arguments", "[parser]") {
    uint8_t output[LDR_HEADER_WIRE_SIZE];
    const ledriver_ldr_header_t header = {0};

    TEST_ASSERT_EQUAL_INT(-1, ledriver_ldr_serialize_header(NULL, output, sizeof(output)));

    TEST_ASSERT_EQUAL_INT(-1, ledriver_ldr_serialize_header(&header, NULL, sizeof(output)));
}
