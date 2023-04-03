#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define OUTPUT_MODE 00700
#define BUFFER_SIZE 4096
#define TRUE 1
#define NUM_OF_PERMISSIONS 9

void mk_dir(char *dir_path) {
    if (mkdir(dir_path, OUTPUT_MODE) == -1) {
        printf("mkdir: cannot create directory '%s'\n", dir_path);
        return;
    }
}

void show_files(char *dir_path) {
    DIR *dir;
    if (!(dir = opendir(dir_path))) {
        printf("show_files: cannot open directory '%s' to show files\n", dir_path);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        printf("%s\n", entry->d_name);
    }
}

void rm_dir(char *dir_path) {
    DIR *dir;
    if (!(dir = opendir(dir_path))) {
        printf("rm_dir: failed to remove '%s': No such file or directory\n", dir_path);
        return;
    }

    struct dirent *entry;
    struct stat st;
    char *filepath = malloc((strlen(dir_path) + 1) * sizeof(char));
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        strcpy(filepath, dir_path);
        filepath[strlen(dir_path)] = '\0';
        filepath = realloc(filepath, (strlen(dir_path) + strlen("/") + strlen(entry->d_name) + 1) * sizeof(char));
        strcat(filepath, "/");
        strcat(filepath, entry->d_name);
        stat(filepath, &st);
        if (S_ISDIR(st.st_mode)) {
            rm_dir(filepath);
        } else {
            remove(filepath);
        }
    }
    remove(dir_path);

    closedir(dir);
    free(filepath);
}

void create_file(char *file_path) {
    if (creat(file_path, OUTPUT_MODE) == -1) {
        printf("create_file: cannot create file '%s'\n", file_path);
        return;
    }
}

void show_file_content(char *file_path) {
    char buffer[BUFFER_SIZE];
    int in_fd = open(file_path, O_RDONLY);
    if (in_fd < 0) {
        printf("show_file_content: cannot open file '%s'\n", file_path);
        return;
    }
    while (TRUE) {
        size_t rd_count = read(in_fd, buffer, BUFFER_SIZE);
        if (rd_count <= 0) {
            break;
        }
        write(1, buffer, rd_count);
    }
}

void rm_file(char *file_path) {
    if (remove(file_path) == -1) {
        printf("rm_file: cannot remove file '%s'\n", file_path);
        return;
    }
}

void create_sym_link(char *file_path, char *sym_link_path) {
    if (symlink(file_path, sym_link_path)) {
        printf("create_sym_link: cannot create symbolic link '%s' to file '%s'\n", sym_link_path, file_path);
        return;
    }
}

void show_sym_link_content(char *sym_link_path) {
    char buffer[BUFFER_SIZE];
    size_t rd_count = readlink(sym_link_path, buffer, BUFFER_SIZE);
    if (rd_count <= 0) {
        printf("show_sym_link_content: file '%s' doesn't link to any file\n", sym_link_path);
    }
    write(1, buffer, rd_count);
}

void show_sym_linked_file_content(char *sym_link_path) {
    char buffer[BUFFER_SIZE];
    size_t rd_link_count = readlink(sym_link_path, buffer, BUFFER_SIZE);
    if (rd_link_count == -1) {
        printf("show_sym_linked_file_content: file '%s' doesn't link to any file\n", sym_link_path);
        return;
    }
    printf("show_sym_linked_file_content: file '%s' links to file '%s'\n", sym_link_path, buffer);
    show_file_content(buffer);
}

void rm_sym_link(char *sym_link_path) {
    if (remove(sym_link_path) == -1) {
        printf("rm_sym_link: cannot remove symbol link '%s'\n", sym_link_path);
        return;
    }
}

void create_hard_link(char *file_path, char *hard_link_path) {
    if (link(hard_link_path, file_path)) {
        printf("create_hard_link: cannot create hard link '%s' to file '%s'\n", hard_link_path, file_path);
        return;
    }
}

void rm_hard_link(char *hard_link_path) {
    if (remove(hard_link_path) == -1) {
        printf("rm_hard_link: cannot remove hard link '%s'\n", hard_link_path);
        return;
    }
}

void show_mod_and_nlink(char *file_path) {
    char access_permissions[NUM_OF_PERMISSIONS];
    struct stat st;
    if (stat(file_path, &st) == -1) {
        printf("show_mod_and_nlink: cannot get file status of file '%s'\n", file_path);
        return;
    }

    (st.st_mode & S_IRUSR) ? (access_permissions[0] = 'r') : (access_permissions[0] = '-');
    (st.st_mode & S_IWUSR) ? (access_permissions[1] = 'w') : (access_permissions[1] = '-');
    (st.st_mode & S_IXUSR) ? (access_permissions[2] = 'x') : (access_permissions[2] = '-');
    (st.st_mode & S_IRGRP) ? (access_permissions[3] = 'r') : (access_permissions[3] = '-');
    (st.st_mode & S_IWGRP) ? (access_permissions[4] = 'w') : (access_permissions[4] = '-');
    (st.st_mode & S_IXGRP) ? (access_permissions[5] = 'x') : (access_permissions[5] = '-');
    (st.st_mode & S_IROTH) ? (access_permissions[6] = 'r') : (access_permissions[6] = '-');
    (st.st_mode & S_IWOTH) ? (access_permissions[7] = 'w') : (access_permissions[7] = '-');
    (st.st_mode & S_IXOTH) ? (access_permissions[8] = 'x') : (access_permissions[8] = '-');

    printf("Access permissions of file '%s': %s\n", file_path, access_permissions);
    printf("Number of hard link to file '%s': %lu\n", file_path, st.st_nlink);
}

void change_access_permissions(const char *access_permissions, char *file_path) {
    mode_t mode = 0;

    (access_permissions[0] == 'r') ? (mode |= S_IRUSR) : (mode &= ~S_IRUSR);
    (access_permissions[1] == 'w') ? (mode |= S_IWUSR) : (mode &= ~S_IWUSR);
    (access_permissions[2] == 'x') ? (mode |= S_IXUSR) : (mode &= ~S_IXUSR);
    (access_permissions[3] == 'r') ? (mode |= S_IRGRP) : (mode &= ~S_IRGRP);
    (access_permissions[4] == 'w') ? (mode |= S_IWGRP) : (mode &= ~S_IWGRP);
    (access_permissions[5] == 'x') ? (mode |= S_IXGRP) : (mode &= ~S_IXGRP);
    (access_permissions[6] == 'r') ? (mode |= S_IROTH) : (mode &= ~S_IROTH);
    (access_permissions[7] == 'w') ? (mode |= S_IWOTH) : (mode &= ~S_IWOTH);
    (access_permissions[8] == 'x') ? (mode |= S_IXOTH) : (mode &= ~S_IXOTH);

    chmod(file_path, mode);
}

int main(int argc, char *argv[]) {
    if (strcmp(argv[0], "./mk_dir") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        mk_dir(argv[1]);
    } else if (strcmp(argv[0], "./show_files") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        show_files(argv[1]);
    } else if (strcmp(argv[0], "./rm_dir") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        rm_dir(argv[1]);
    } else if (strcmp(argv[0], "./create_file") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        create_file(argv[1]);
    } else if (strcmp(argv[0], "./show_file_content") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        show_file_content(argv[1]);
    } else if (strcmp(argv[0], "./rm_file") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        rm_file(argv[1]);
    } else if (strcmp(argv[0], "./create_sym_link") == 0) {
        if (argc != 3) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        create_sym_link(argv[1], argv[2]);
    } else if (strcmp(argv[0], "./show_sym_link_content") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        show_sym_link_content(argv[1]);
    } else if (strcmp(argv[0], "./show_sym_linked_file_content") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        show_sym_linked_file_content(argv[1]);
    } else if (strcmp(argv[0], "./rm_sym_link") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        rm_sym_link(argv[1]);
    } else if (strcmp(argv[0], "./create_hard_link") == 0) {
        if (argc != 3) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        create_hard_link(argv[1], argv[2]);
    } else if (strcmp(argv[0], "./rm_hard_link") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        rm_hard_link(argv[1]);
    } else if (strcmp(argv[0], "./show_mod_and_nlink") == 0) {
        if (argc != 2) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        show_mod_and_nlink(argv[1]);
    } else if (strcmp(argv[0], "./change_access_permissions") == 0) {
        if (argc != 3) {
            fprintf(stderr, "%s: invalid count of arguments\n", argv[0]);
            return -1;
        }
        change_access_permissions(argv[1], argv[2]);
    } else {
        printf("Command '%s' not found\n", argv[0]);
    }
    return 0;
}
