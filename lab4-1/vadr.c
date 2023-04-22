#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 100

int global_init = 2;
int global_not_init;

int *init_local() {
    int local;
    printf("Local in function not initialized: %p\n", &local);
    local = 2;
    printf("Local in function initialized: %p\n", &local);
    return &local;
}

void work_with_buf() {
    char *buf1 = malloc(BUF_SIZE * sizeof(char));
    strcpy(buf1, "hello, world1");
    printf("%s\n", buf1);
    free(buf1);
    printf("%s\n", buf1);

    char *buf2 = malloc(BUF_SIZE * sizeof(char));
    strcpy(buf2, "hello, world2");
    printf("%s\n", buf2);
    free(buf2 + BUF_SIZE / 2);
    printf("%s\n", buf2);
}

void print_var_adresses() {
    int local;
    static int static_local;
    const int const_local;
    printf("Local: %p\n"
           "Static local: %p\n"
           "Const local: %p\n"
           "Global initialized: %p\n"
           "Global not initialized: %p\n"
           "Function: %p\n",
           &local, &static_local, &const_local, &global_init, &global_not_init, &print_var_adresses);
    printf("Local in function: %p\n", init_local());
}

int main() {
    printf("Pid: %d\n", getpid());
    print_var_adresses();
    work_with_buf();
    sleep(30);
    return 0;
}