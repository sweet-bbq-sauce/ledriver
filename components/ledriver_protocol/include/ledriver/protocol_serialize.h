#pragma once

#include <stddef.h>
#include <stdint.h>

#include <ledriver/protocol.h>

int ledriver_protocol_serialize_header(const ledriver_protocol_header_t *header,
                                       uint8_t *buffer,
                                       size_t buffer_len);

int ledriver_protocol_serialize_payload_status(const ledriver_protocol_payload_status_t *payload,
                                               uint8_t *buffer,
                                               size_t buffer_len);

int ledriver_protocol_serialize_payload_update(const ledriver_protocol_payload_update_t *payload,
                                               uint8_t *buffer,
                                               size_t buffer_len);

int ledriver_protocol_serialize_payload_power(const ledriver_protocol_payload_power_t *payload,
                                              uint8_t *buffer,
                                              size_t buffer_len);
