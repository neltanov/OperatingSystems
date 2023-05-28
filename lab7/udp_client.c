#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_aton(SERVER_IP, &server_addr.sin_addr) == 0) {
        perror("Ошибка при преобразовании IP-адреса");
        exit(1);
    }

    while (1) {
        printf("Введите данные для отправки (или 'q' для выхода): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (strcmp(buffer, "q\n") == 0) {
            break;
        }

        ssize_t num_bytes = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (num_bytes < 0) {
            perror("Ошибка при отправке данных");
            exit(1);
        }

        num_bytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (num_bytes < 0) {
            perror("Ошибка при получении данных");
            exit(1);
        }

        printf("Получен ответ от сервера: %.*s\n", (int)num_bytes, buffer);
    }

    close(sockfd);

    return 0;
}