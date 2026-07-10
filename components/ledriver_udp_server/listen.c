#include <stdatomic.h>
#include <stdint.h>

#include <esp_err.h>
#include <esp_log.h>
#include <lwip/sockets.h>

#include <ledriver/udp_server.h>

static const char *TAG = "udp_server";

static atomic_bool stop_requested = false;

static void server_loop(void *arg) {
    const int fd = (int)(intptr_t)arg;

    if (fd < 0) {
        ESP_LOGE(TAG, "Invalid socket file descriptor");
        vTaskDelete(NULL);
        return;
    }

    while (atomic_load(&stop_requested) == false) {
        char buffer[32];
        struct sockaddr_storage client_addr;
        socklen_t addr_len = sizeof(client_addr);

        const ssize_t recv_len =
            recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0) {
            ESP_LOGE(TAG, "Failed to receive packet: %s", strerror(errno));
            continue;
        }

        ESP_LOGI(TAG,
                 "Received %zd bytes from client %s",
                 recv_len,
                 inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr));
    }
}

esp_err_t ledriver_udp_server_listen(const struct sockaddr_storage *addr) {
    if (!addr) {
        return ESP_ERR_INVALID_ARG;
    }

    const int listen_fd = socket(addr->ss_family, SOCK_DGRAM, IPPROTO_UDP);
    if (listen_fd < 0) {
        ESP_LOGE(TAG, "Failed to create socket: %s", strerror(errno));
        return ESP_FAIL;
    }

    if (bind(listen_fd, (const struct sockaddr *)addr, sizeof(*addr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind socket: %s", strerror(errno));
        close(listen_fd);
        return ESP_FAIL;
    }

    if (xTaskCreate(server_loop, "udp_server", 4096, (void *)(intptr_t)listen_fd, 0, NULL) !=
        pdTRUE) {
        ESP_LOGE(TAG, "Failed to create server task");
        close(listen_fd);
        return ESP_FAIL;
    }

    ESP_LOGI(TAG,
             "UDP server listening on port %d",
             ntohs(((const struct sockaddr_in *)addr)->sin_port));

    return ESP_OK;
}
