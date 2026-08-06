#pragma once

#include <stddef.h>
#include <stdint.h>

#include <esp_err.h>

#include <ledriver/protocol.h>

int ledriver_protocol_parse_header(const uint8_t *buffer,
                                   size_t buffer_len,
                                   ledriver_protocol_header_t *header);

int ledriver_protocol_parse_payload_status(const uint8_t *buffer,
                                           size_t buffer_len,
                                           ledriver_protocol_payload_status_t *payload);

int ledriver_protocol_parse_payload_update(const uint8_t *buffer,
                                           size_t buffer_len,
                                           ledriver_protocol_payload_update_t *payload);

int ledriver_protocol_parse_payload_power(const uint8_t *buffer,
                                          size_t buffer_len,
                                          ledriver_protocol_payload_power_t *payload);
