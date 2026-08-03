#include <stdint.h>

#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sys/socket.h>

#include <ledriver/udp_server.h>

static const char *TAG = "udp_server";

static void udp_server_task(void *parameters) {
    const int listen_fd = (intptr_t)parameters;

    while (1) {
        char buffer[1024];
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        const ssize_t recv_len = recvfrom(listen_fd,
                                          buffer,
                                          sizeof(buffer) - 1,
                                          0,
                                          (struct sockaddr *)&client_addr,
                                          &addr_len);
        if (recv_len < 0) {
            ESP_LOGE(TAG, "Failed to receive data: %s", strerror(errno));
            continue;
        }

        buffer[recv_len] = '\0';
        ESP_LOGI(TAG,
                 "Received data from %s:%d: %s",
                 inet_ntoa(client_addr.sin_addr),
                 ntohs(client_addr.sin_port),
                 buffer);
    }
}

esp_err_t ledriver_udp_server_start(uint16_t port) {
    const int listen_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (listen_fd < 0) {
        ESP_LOGE(TAG, "Failed to create socket: %s", strerror(errno));
        return ESP_FAIL;
    }

    const struct sockaddr_in listen_addr = {.sin_family = AF_INET,
                                            .sin_port = htons(port),
                                            .sin_addr.s_addr = htonl(INADDR_ANY)};

    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind socket: %s", strerror(errno));
        close(listen_fd);
        return ESP_FAIL;
    }

    if (xTaskCreate(udp_server_task,
                    "udp_server_task",
                    4096,
                    (void *)(intptr_t)listen_fd,
                    5,
                    NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create UDP server task");
        close(listen_fd);
        return ESP_FAIL;
    }

    return ESP_OK;
}