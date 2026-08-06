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

static int parse_color(const uint8_t *buffer, size_t buffer_len, ledriver_protocol_color_t *color) {
    if (!buffer || !color || buffer_len < 6) {
        return -1; // Invalid arguments
    }

    color->red_be = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
    color->green_be = ((uint16_t)buffer[2] << 8) | (uint16_t)buffer[3];
    color->blue_be = ((uint16_t)buffer[4] << 8) | (uint16_t)buffer[5];

    return 0; // Success
}

int ledriver_protocol_parse_payload_status(const uint8_t *buffer,
                                           size_t buffer_len,
                                           ledriver_protocol_payload_status_t *payload) {
    if (!buffer || !payload || buffer_len < sizeof(ledriver_protocol_payload_status_t)) {
        return -1;
    }

    int color_result = parse_color(buffer, buffer_len, &payload->color);
    if (color_result < 0) {
        return -1; // Error parsing color
    }

    payload->power = buffer[6];

    return 0;
}

int ledriver_protocol_parse_payload_update(const uint8_t *buffer,
                                           size_t buffer_len,
                                           ledriver_protocol_payload_update_t *payload) {
    if (!buffer || !payload || buffer_len < sizeof(ledriver_protocol_payload_update_t)) {
        return -1;
    }

    int color_result = parse_color(buffer, buffer_len, &payload->color);
    if (color_result < 0) {
        return -1; // Error parsing color
    }

    payload->no_ack = buffer[6];

    return 0;
}

int ledriver_protocol_parse_payload_power(const uint8_t *buffer,
                                          size_t buffer_len,
                                          ledriver_protocol_payload_power_t *payload) {
    if (!buffer || !payload || buffer_len < sizeof(ledriver_protocol_payload_power_t)) {
        return -1;
    }

    payload->power = buffer[0];

    return 0;
}
