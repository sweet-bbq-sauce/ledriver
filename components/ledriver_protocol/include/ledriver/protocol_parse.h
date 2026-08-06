#pragma once

#include <stdint.h>
#include <stddef.h>

#include <esp_err.h>

#include <ledriver/protocol.h>

int ledriver_protocol_parse_header(const uint8_t *buffer,
                                         size_t buffer_len,
                                         ledriver_protocol_header_t *header);
