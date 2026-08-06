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

static int
serialize_color(const ledriver_protocol_color_t *color, uint8_t *buffer, size_t buffer_len) {
    if (!color || !buffer || buffer_len < 6) {
        return -1; // Invalid arguments
    }

    buffer[0] = (uint8_t)((color->red_be >> 8) & 0xFF);
    buffer[1] = (uint8_t)(color->red_be & 0xFF);
    buffer[2] = (uint8_t)((color->green_be >> 8) & 0xFF);
    buffer[3] = (uint8_t)(color->green_be & 0xFF);
    buffer[4] = (uint8_t)((color->blue_be >> 8) & 0xFF);
    buffer[5] = (uint8_t)(color->blue_be & 0xFF);

    return 6; // Number of bytes written for color
}

int ledriver_protocol_serialize_payload_status(const ledriver_protocol_payload_status_t *payload,
                                               uint8_t *buffer,
                                               size_t buffer_len) {
    if (!payload || !buffer || buffer_len < sizeof(ledriver_protocol_payload_status_t)) {
        return -1; // Invalid arguments
    }

    int color_bytes = serialize_color(&payload->color, buffer, buffer_len);
    if (color_bytes < 0) {
        return -1; // Error serializing color
    }

    buffer[6] = payload->power;

    return sizeof(ledriver_protocol_payload_status_t);
}

int ledriver_protocol_serialize_payload_update(const ledriver_protocol_payload_update_t *payload,
                                               uint8_t *buffer,
                                               size_t buffer_len) {
    if (!payload || !buffer || buffer_len < sizeof(ledriver_protocol_payload_update_t)) {
        return -1; // Invalid arguments
    }

    int color_bytes = serialize_color(&payload->color, buffer, buffer_len);
    if (color_bytes < 0) {
        return -1; // Error serializing color
    }

    buffer[6] = payload->no_ack;

    return sizeof(ledriver_protocol_payload_update_t);
}

int ledriver_protocol_serialize_payload_power(const ledriver_protocol_payload_power_t *payload,
                                              uint8_t *buffer,
                                              size_t buffer_len) {
    if (!payload || !buffer || buffer_len < sizeof(ledriver_protocol_payload_power_t)) {
        return -1; // Invalid arguments
    }

    buffer[0] = payload->power;

    return sizeof(ledriver_protocol_payload_power_t);
}
