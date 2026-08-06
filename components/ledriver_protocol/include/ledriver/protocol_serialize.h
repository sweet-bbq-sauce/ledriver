#pragma once

#include <stddef.h>
#include <stdint.h>

#include <ledriver/protocol.h>

int ledriver_ldr_serialize_header(const ledriver_ldr_header_t *header,
                                  uint8_t *buffer,
                                  size_t buffer_len);
