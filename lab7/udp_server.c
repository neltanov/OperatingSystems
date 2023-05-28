#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind: socket binding error");
        exit(1);
    }

    printf("UDP server has been run on port %d\n", PORT);

    while (1) {
        socklen_t client_len = sizeof(client_addr);

        ssize_t num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (num_bytes < 0) {
            perror("Ошибка при получении данных");
            exit(1);
        }

        printf("Получено от клиента: %.*s\n", (int)num_bytes, buffer);

        if (sendto(sockfd, buffer, num_bytes, 0, (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("Ошибка при отправке данных");
            exit(1);
        }
    }

    close(sockfd);

    return 0;
}
