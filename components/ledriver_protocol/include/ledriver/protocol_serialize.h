#pragma once

#include <stddef.h>
#include <stdint.h>

#include <ledriver/protocol.h>

int ledriver_protocol_serialize_header(const ledriver_protocol_header_t *header,
                                       uint8_t *buffer,
                                       size_t buffer_len);
