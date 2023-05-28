#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define BACKLOG 5

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) < 0) {
        perror("Ошибка при ожидании входящих подключений");
        exit(1);
    }

    printf("TCP сервер запущен на порту %d\n", PORT);

    while (1) {
        socklen_t client_len = sizeof(client_addr);

        new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (new_sockfd < 0) {
            perror("Ошибка при принятии подключения");
            exit(1);
        }

        printf("Подключен новый клиент\n");

        while (1) {
            ssize_t num_bytes = recv(new_sockfd, buffer, BUFFER_SIZE, 0);
            if (num_bytes < 0) {
                perror("Ошибка при получении данных");
                exit(1);
            }

            if (num_bytes == 0) {
                printf("Клиент отключился\n");
                break;
            }

            printf("Получено от клиента: %.*s\n", (int)num_bytes, buffer);

            if (send(new_sockfd, buffer, num_bytes, 0) < 0) {
                perror("Ошибка при отправке данных");
                exit(1);
            }
        }

        close(new_sockfd);
    }

    close(sockfd);

    return 0;
}