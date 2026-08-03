#pragma once

#include <stddef.h>
#include <stdint.h>

#include <ledriver/ldr.h>

// int ledriver_ldr_parse_header(const uint8_t *buffer, size_t buffer_len, ledriver_ldr_header_t
// *header);
int ledriver_ldr_serialize_header(const ledriver_ldr_header_t *header,
                                  uint8_t *buffer,
                                  size_t buffer_len);
