#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#define BUF_SIZE 1
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
    reverse_string(str);
    DIR *dir;
    /* Создаем директорию с перевернутым именем. */
    if (!(dir = opendir(str))) {
        mkdir(str, OUTPUT_MODE);
    } else {
        mkdir(str, OUTPUT_MODE);
        closedir(dir);
    }
}


void reverse_filename(char *file_name) {
    char *buf = malloc((strlen(file_name) + 1) * sizeof(char));
    strcpy(buf, file_name);
    char *name = strtok(buf, ".");
    reverse_string(name);
    for (int i = 0; i < strlen(name); i++) {
        file_name[i] = name[i];
    }
    free(buf);
}

void copy_reversed_regular_files(char *src_dir) {
    DIR *dir;
    if (!(dir = opendir(src_dir))) {
        printf("%s: No such file or directory", src_dir);
        exit(2);
    }

    char *dst_dir = malloc((strlen(src_dir) + 1) * sizeof(char));
    strcpy(dst_dir, src_dir);
    create_reversed_directory(dst_dir);

    struct dirent *entry;
    struct stat st;
    char *src_filepath = malloc((strlen(src_dir) + 1) * sizeof(char));
    char *dst_filepath = malloc((strlen(dst_dir) + 1) * sizeof(char));
    int rd_count, wt_count, in_fd, out_fd;
    long offset;
    char *text_buffer = malloc(BUF_SIZE * sizeof(char));
    /* Обходим все файлы в этой директории. */
    while ((entry = readdir(dir))) {
        /* Ссылки на родительскую и текущую директорию не копируем. */
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        /* Формируем путь к файлу, из которого копируем данные. */
        strcpy(src_filepath, src_dir);
        src_filepath[strlen(src_dir)] = '\0';
        src_filepath = realloc(src_filepath,
                               (strlen(src_dir) + strlen("/") + strlen(entry->d_name) + 1) * sizeof(char));
        strcat(src_filepath, "/");
        strcat(src_filepath, entry->d_name);

        if (stat(src_filepath, &st) != 0) {
            printf("%s: Cannot get stats of directory", entry->d_name);
            exit(2);
        }
        /* Директории не копируем. */
        if (S_ISDIR(st.st_mode)) {
            continue;
        }

        /* Переворачиваем имя исходного файла. */
        reverse_filename(entry->d_name);

        /* Формируем путь к файлу, в который копируем данные. */
        strcpy(dst_filepath, dst_dir);
        dst_filepath[strlen(dst_dir)] = '\0';
        dst_filepath = realloc(dst_filepath,
                               (strlen(dst_dir) + strlen("/") + strlen(entry->d_name) + 1) * sizeof(char));
        strcat(dst_filepath, "/");
        strcat(dst_filepath, entry->d_name);

        /* Открываем файл с именем пути src_filepath. */
        in_fd = open(src_filepath, O_RDONLY);
        if (in_fd < 0) {
            printf("%s: This file cannot be opened", src_filepath);
            exit(3);
        }
        /* Создаем файл по имени пути dst_filepath. */
        out_fd = creat(dst_filepath, OUTPUT_MODE);
        if (out_fd < 0) {
            printf("%s: This file cannot be created", src_filepath);
            exit(4);
        }

        offset = lseek(in_fd, 0, SEEK_END) - 1;
        if (offset == -1) {
            exit(10);
        }
        while (offset > 0) {
            lseek(in_fd, --offset, SEEK_SET);
            rd_count = (int) read(in_fd, text_buffer, 1);
            wt_count = (int) write(out_fd, text_buffer, rd_count);
            if (wt_count <= 0) {
                exit(5);
            }
        }

        close(in_fd);
        close(out_fd);
    }
    free(text_buffer);
    closedir(dir);
    free(dst_dir);
    free(src_filepath);
    free(dst_filepath);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Too few arguments in command line!\n");
        exit(1);
    }
    copy_reversed_regular_files(argv[1]);
    return 0;
}