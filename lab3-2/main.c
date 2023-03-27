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

void mk_dir(char *dir_path) {
     if (mkdir(dir_path, OUTPUT_MODE) == -1){
        printf("mkdir: cannot create directory '%s'", dir_path);
         return;
    }
}

void show_files(char *dir_path) {
    DIR *dir;
    if (!(dir = opendir(dir_path))) {
        printf("%s: No such file or directory\n", dir_path);
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        printf("%s\n", entry->d_name);
    }
}

void rm_dir(char *dir_path) {
    if (remove(dir_path) == -1) {
        printf("%s: No such directory\n", dir_path);
        return;
    }
}

void create_file(char *file_path) {
    if (creat(file_path, OUTPUT_MODE) == -1) {
        printf("%s: Can't create file\n", file_path);
        return;
    }
}

void show_file_content(char *file_path) {
    char buffer[BUFFER_SIZE];
    int in_fd = open(file_path, O_RDONLY);
    while(TRUE) {
        size_t rd_count = read(in_fd, buffer, BUFFER_SIZE);
        if (rd_count <= 0) {
            break;
        }
        write(1, buffer, rd_count);
    }
}

void rm_file(char *file_path) {
    if (remove(file_path) == -1) {
        printf("%s: No such file\n", file_path);
        return;
    }
}

void create_sym_link(char *file_path) {
    if (symlink(strcat(file_path, "_symlink"), file_path)) {
        printf("%s: Symbol link didn't created\n", file_path);
        return;
    }
}

void show_sym_link_content(char *sym_link_path) {
    char buffer[BUFFER_SIZE];
    while(TRUE) {
        size_t rd_count = readlink(sym_link_path, buffer, BUFFER_SIZE);
        if (rd_count <= 0) {
            break;
        }
        write(1, buffer, rd_count);
    }
}

void show_sym_linked_file(char *sym_link_path) {
    char buffer[BUFFER_SIZE];
    size_t rd_count = readlink(sym_link_path, buffer, BUFFER_SIZE);
    write(1, buffer, rd_count);
}

void rm_sym_link(char *sym_link_path) {
    if (remove(sym_link_path) == -1) {
        printf("%s: No such symbol link\n", sym_link_path);
        return;
    }
}

void create_hard_link(char *file_path, char *hard_link_path) {
    if (link(hard_link_path, file_path)) {
        printf("%s: Hard link didn't created\n", file_path);
        return;
    }
}

void rm_hard_link(char *hard_link_path) {
    if (remove(hard_link_path) == -1) {
        printf("%s: No such hard link\n", hard_link_path);
        return;
    }
}

void show_mod_and_nlink(char *file_path) {
    struct stat st;
    if (stat(file_path, &st) == -1) {
        printf("%s: Can't get file stats\n", file_path);
        return;
    }
    char mod[3];
    int readable = access(file_path, R_OK);
    int writable = access(file_path, W_OK);
    int executable = access(file_path, X_OK);

    if (readable == 0) {
        mod[0] = 'r';
    } else {
        mod[0] = '-';
    }

    if (writable == 0) {
        mod[1] = 'w';
    } else {
        mod[1] = '-';
    }

    if (executable == 0) {
        mod[2] = 'x';
    } else {
        mod[2] = '-';
    }

    printf("Access mode of file %s: %s\n", file_path, mod);
    printf("Number of hard link to file %s: %lu\n", file_path, st.st_nlink);
}

void change_access_mode(char *file_path) {
//    chmod(file_path,)
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Invalid count of arguments!\n");
        return -1;
    }
    if (strcmp(argv[0], "./mk_dir") == 0) {
        mk_dir(argv[1]);
    } else if (strcmp(argv[0], "./show_files") == 0) {
        show_files(argv[1]);
    } else if (strcmp(argv[0], "./rm_dir") == 0) {
        rm_dir(argv[1]);
    } else if (strcmp(argv[0], "./create_file") == 0) {
        create_file(argv[1]);
    } else if (strcmp(argv[0], "./show_file_content") == 0) {
        show_file_content(argv[1]);
    } else if (strcmp(argv[0], "./rm_file") == 0) {
        rm_file(argv[1]);
    } else if (strcmp(argv[0], "./create_sym_link") == 0) {
        create_sym_link(argv[1]);
    } else if (strcmp(argv[0], "./show_sym_link_content") == 0) {
        show_sym_link_content(argv[1]);
    } else if (strcmp(argv[0], "./show_sym_linked_file") == 0) {
        show_sym_linked_file(argv[1]);
    } else if (strcmp(argv[0], "./rm_sym_link") == 0) {
        rm_sym_link(argv[1]);
    } else if (strcmp(argv[0], "./create_hard_link") == 0) {
        create_hard_link(argv[1], argv[2]);
    } else if (strcmp(argv[0], "./rm_hard_link") == 0) {
        rm_hard_link(argv[1]);
    } else if (strcmp(argv[0], "./show_mod_and_nlink") == 0) {
        show_mod_and_nlink(argv[1]);
    } else if (strcmp(argv[0], "./change_access_mode") == 0) {
//        change_access_mode(argv[1]);
    } else {
        printf("This command doesn't exist\n");
    }
    return 0;
}
