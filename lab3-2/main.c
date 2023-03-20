#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define OUTPUT_MODE 00700

void mk_dir(char *dir_name) {
    DIR *dir;
    if (!(dir = opendir(dir_name))) {
        mkdir(dir_name, OUTPUT_MODE);
    } else {
        closedir(dir);
        printf("mkdir: cannot create directory '%s'", dir_name);
    }
}

void dir_files(char *dir_name) {
    DIR *dir;
    if (!(dir = opendir(dir_name))) {
        printf("%s: No such file or directory", dir_name);
        exit(2);
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        printf("%s\n", entry->d_name);
    }
}

//void rm_dir(char *dir_name) {
//    DIR *dir;
//    if (!(dir = opendir(dir_name))) {
//        mkdir(dir_name, OUTPUT_MODE);
//    } else {
//        remove(dir_name);
//        closedir(dir);
//        printf("mkdir: cannot create directory '%s'", dir_name);
//    }
//}

int main(int argc, char *argv[]) {

    return 0;
}
