#pragma once

#include <esp_err.h>
#include <lwip/sockets.h>

esp_err_t ledriver_udp_server_listen(const struct sockaddr_storage *addr);
