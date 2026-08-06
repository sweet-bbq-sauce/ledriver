#pragma once

#include <stdint.h>

#define LDR_HEADER_WIRE_SIZE 10

typedef struct {
    uint8_t version;
    uint32_t request_id;
    uint8_t message_type;
    uint8_t return_value;
    uint8_t command;
} ledriver_ldr_header_t;

// Magic number for LDR protocol: 'L', 'D', 'R'
#define LDR_MAGIC_0 0x4Cu
#define LDR_MAGIC_1 0x44u
#define LDR_MAGIC_2 0x52u

// Message types
#define LDR_MESSAGE_TYPE_REQUEST  0x00u
#define LDR_MESSAGE_TYPE_RESPONSE 0x01u

// Return values
#define LDR_RETURN_VALUE_SUCCESS         0x00u
#define LDR_RETURN_VALUE_INVALID_REQUEST 0x01u
#define LDR_RETURN_VALUE_UNKNOWN_COMMAND 0x02u
#define LDR_RETURN_VALUE_INVALID_PAYLOAD 0x03u
#define LDR_RETURN_VALUE_COMMAND_FAILED  0x04u
#define LDR_RETURN_VALUE_BUSY            0x05u
#define LDR_RETURN_VALUE_LOCKED          0x06u
#define LDR_RETURN_VALUE_INTERNAL_ERROR  0x07u

// Commands
#define LDR_COMMAND_DISCOVER 0x00u
#define LDR_COMMAND_STATUS   0x01u
#define LDR_COMMAND_POWER    0x02u
#define LDR_COMMAND_UPDATE   0x03u
#define LDR_COMMAND_LOCK     0x04u

// message_ctrl layout:
// bits 7..1 - return value
// bit 0     - message type
#define LDR_MESSAGE_CTRL_MESSAGE_TYPE_MASK  0x01u
#define LDR_MESSAGE_CTRL_RETURN_VALUE_MASK  0xFEu
#define LDR_MESSAGE_CTRL_RETURN_VALUE_SHIFT 1u

#define LDR_MESSAGE_CTRL_GET_MESSAGE_TYPE(msgctrl) \
    ((uint8_t)((msgctrl) & LDR_MESSAGE_CTRL_MESSAGE_TYPE_MASK))

#define LDR_MESSAGE_CTRL_GET_RETURN_VALUE(msgctrl)                 \
    ((uint8_t)(((msgctrl) & LDR_MESSAGE_CTRL_RETURN_VALUE_MASK) >> \
               LDR_MESSAGE_CTRL_RETURN_VALUE_SHIFT))

#define LDR_MESSAGE_CTRL_PACK(message_type, return_value) \
    ((uint8_t)((((uint8_t)(return_value) & 0x7Fu) << 1u) | ((uint8_t)(message_type) & 0x01u)))
