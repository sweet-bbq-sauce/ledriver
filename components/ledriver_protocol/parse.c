#include <esp_err.h>

#include <ledriver/protocol.h>
#include <ledriver/protocol_parse.h>

int ledriver_protocol_parse_header(const uint8_t *buffer,
                                   size_t buffer_len,
                                   ledriver_protocol_header_t *header) {
    if (!buffer || !header || buffer_len < LDR_HEADER_WIRE_SIZE) {
        return -1;
    }

    if (buffer[0] != LDR_MAGIC_0 || buffer[1] != LDR_MAGIC_1 || buffer[2] != LDR_MAGIC_2) {
        return -1; // Invalid magic number
    }

    header->version = buffer[3];
    header->request_id = ((uint32_t)buffer[4] << 24) | ((uint32_t)buffer[5] << 16) |
                         ((uint32_t)buffer[6] << 8) | (uint32_t)buffer[7];

    header->message_type = LDR_MESSAGE_CTRL_GET_MESSAGE_TYPE(buffer[8]);
    header->return_value = LDR_MESSAGE_CTRL_GET_RETURN_VALUE(buffer[8]);

    header->command = buffer[9];

    return 0;
}
