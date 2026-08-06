#include <stdint.h>

#include <unity.h>

#include <ledriver/protocol.h>
#include <ledriver/protocol_parse.h>

TEST_CASE("parses a valid LDR header", "[parser]") {
    const uint8_t input[] = {
        LDR_MAGIC_0,
        LDR_MAGIC_1,
        LDR_MAGIC_2,
        0x00,
        0x12,
        0x34,
        0x56,
        0x78,
        LDR_MESSAGE_CTRL_PACK(LDR_MESSAGE_TYPE_RESPONSE, LDR_RETURN_VALUE_BUSY),
        LDR_COMMAND_STATUS,
    };
    ledriver_protocol_header_t header = {0};

    int result = ledriver_protocol_parse_header(input, sizeof(input), &header);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_HEX8(0x00, header.version);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, header.request_id);
    TEST_ASSERT_EQUAL_HEX8(LDR_MESSAGE_TYPE_RESPONSE, header.message_type);
    TEST_ASSERT_EQUAL_HEX8(LDR_RETURN_VALUE_BUSY, header.return_value);
    TEST_ASSERT_EQUAL_HEX8(LDR_COMMAND_STATUS, header.command);
}

TEST_CASE("parser rejects invalid magic", "[parser]") {
    const uint8_t input[] = {
        0x00, // Invalid byte for LDR_MAGIC_0
        LDR_MAGIC_1,
        LDR_MAGIC_2,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
    };
    ledriver_protocol_header_t header = {0};

    TEST_ASSERT_EQUAL_INT(-1, ledriver_protocol_parse_header(input, sizeof(input), &header));
}

TEST_CASE("parser rejects buffer shorter than header", "[parser]") {
    const uint8_t input[LDR_HEADER_WIRE_SIZE - 1] = {0};
    ledriver_protocol_header_t header = {0};

    TEST_ASSERT_EQUAL_INT(-1, ledriver_protocol_parse_header(input, sizeof(input), &header));
}

TEST_CASE("parser rejects null arguments", "[parser]") {
    const uint8_t input[LDR_HEADER_WIRE_SIZE] = {0};
    ledriver_protocol_header_t header = {0};

    TEST_ASSERT_EQUAL_INT(-1, ledriver_protocol_parse_header(NULL, sizeof(input), &header));
    TEST_ASSERT_EQUAL_INT(-1, ledriver_protocol_parse_header(input, sizeof(input), NULL));
}
