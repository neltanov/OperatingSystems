#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

void f1() {
    char array[4096];
    
    usleep(100000);
    f1();
}

void f2() {
    printf("Function f2 has been invoked\n");
    long size = 0;
    const int block = 1024 * 100;

    printf("pid: %d; size %ld\n", getpid(), size);

    while(1) {
        malloc(block);
        size += block;

        printf("pid: %d; size %ld\n", getpid(), size);

        usleep(100000);
        if (size > block * 50) {
            mmap(NULL, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        }
    }
}

//void create_new_mapping() {
//    mmap(0, 4096, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//}

int main() {
    printf("Pid: %d\n", getpid());
    sleep(10);
    f1();
//    f2();
//    create_new_mapping();
    sleep(15);
    return 0;
}
