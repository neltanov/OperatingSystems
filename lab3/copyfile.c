#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define TRUE 1
#define BUF_SIZE 4096
#define OUTPUT_MODE 0700

void reverse_string(char *str) {
    size_t len = strlen(str);
    char temp;
    for (size_t i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void create_reversed_directory(char *str) {
    char *pch = strtok(str, "/");
    while (pch) {
        str = pch;
        pch = strtok(NULL, "/");
    }
    /* Переворачиваем директорию. */
    reverse_string(str);

    DIR *dir;
    /* Создаем директорию с перевернутым именем. */
    if (!(dir = opendir(str))) {
        mkdir(str, OUTPUT_MODE);
        printf("Directory created successfully.\n");
    } else {
        printf("Directory is already exist. Current directory will be overwritten.\n");
        mkdir(str, OUTPUT_MODE);
        closedir(dir);
    }
}


void reverse_filename(char *file_path) {
    char buf[strlen(file_path)];
    for (int i = 0; i < strlen(file_path); i++) {
        buf[i] = file_path[i];
    }
    char *name = strtok(buf, "/."); // file_path
    reverse_string(name); // reversed file_path
//    buf = strtok(NULL, "/."); // file extension
//    char *new_path = "";
//    strcat(new_path, name);
//    strcat(new_path, "/.");
//    strcat(new_path, buf);
//    file_path = new_path;
}

void copy_reversed_regular_files(char *src_dir, char *dst_dir) {
    int in_fd, out_fd, rd_count, wt_count;
    DIR *dir;
    /* Открываем директорию. */
    if (!(dir = opendir(src_dir))) {
        printf("%s: This directory doesn't exist!\n", src_dir);
        return;
    }
    struct dirent *entry;
    struct stat st;
    char *src_filepath = malloc((strlen(src_dir) + 1) * sizeof(char));
    char *dst_filepath = malloc((strlen(dst_dir) + 1) * sizeof(char));

    /* Обходим все файлы в этой директории. */
    while ((entry = readdir(dir))) {
        /* Ссылки на родительскую и текущую директорию не копируем. */
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        /* Директории не копируем. */
        stat(entry->d_name, &st);
        if (S_ISDIR(st.st_mode)) {
            continue;
        }

        /* Формируем путь к файлу, из которого копируем данные. */
        strcpy(src_filepath, src_dir);
        src_filepath[strlen(src_dir)] = '\0';
        strcat(src_filepath, "/");
        strcat(src_filepath, entry->d_name);

        /* Переворачиваем имя исходного файла. */
        reverse_string(entry->d_name); // поменять на reverse_filename

        /* Формируем путь к файлу, в который копируем данные. */
        strcpy(dst_filepath, dst_dir);
        dst_filepath[strlen(dst_dir)] = '\0';
        strcat(dst_filepath, "/");
        strcat(dst_filepath, entry->d_name);

        /* Открываем файл с именем пути src_filepath. */
        in_fd = open(src_filepath, O_RDONLY);
        if (in_fd < 0) {
            exit(2);
        }
        /* Создаем файл по имени пути dst_filepath. */
        out_fd = creat(dst_filepath, OUTPUT_MODE);
        if (out_fd < 0) {
            exit(3);
        }

        char buffer[BUF_SIZE];

        while (TRUE) {
            rd_count = (int) read(in_fd, buffer, BUF_SIZE);
            if (rd_count <= 0) {
                break;
            }

            reverse_string(buffer);

            wt_count = (int) write(out_fd, buffer, rd_count);
            if (wt_count <= 0) {
                exit(4);
            }
        }

        close(in_fd);
        close(out_fd);
    }
    free(src_filepath);
    free(dst_filepath);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Too few arguments in command line!\n");
        return -1;
    }

    char *dir_name = malloc((strlen(argv[1]) + 1) * sizeof(char));
    strcpy(dir_name, argv[1]);
    /* Создание каталога с именем заданного каталога, прочитанного наоборот.
     После выполнения функции в argv[1] находится путь созданной директории.*/
    create_reversed_directory(argv[1]);

    copy_reversed_regular_files(dir_name, argv[1]);

    free(dir_name);
}