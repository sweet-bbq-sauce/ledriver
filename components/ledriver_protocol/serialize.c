#include <stddef.h>
#include <stdint.h>

#include <ledriver/protocol.h>
#include <ledriver/protocol_serialize.h>

int ledriver_protocol_serialize_header(const ledriver_protocol_header_t *header,
                                       uint8_t *buffer,
                                       size_t buffer_len) {
    if (!header || !buffer || buffer_len < LDR_HEADER_WIRE_SIZE) {
        return -1; // Invalid arguments
    }

    buffer[0] = LDR_MAGIC_0;
    buffer[1] = LDR_MAGIC_1;
    buffer[2] = LDR_MAGIC_2;
    buffer[3] = header->version;
    buffer[4] = (uint8_t)((header->request_id >> 24) & 0xFF);
    buffer[5] = (uint8_t)((header->request_id >> 16) & 0xFF);
    buffer[6] = (uint8_t)((header->request_id >> 8) & 0xFF);
    buffer[7] = (uint8_t)(header->request_id & 0xFF);
    buffer[8] = LDR_MESSAGE_CTRL_PACK(header->message_type, header->return_value);
    buffer[9] = header->command;

    return LDR_HEADER_WIRE_SIZE;
}
