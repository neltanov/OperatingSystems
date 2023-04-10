#include <stdio.h>
#include <unistd.h>

int global_init = 0;
int global_not_init;

void init_local() {
    int local;
    printf("Local in function not initialized: %p\n", &local);
    local = 2;
    printf("Local in function initialized: %p\n", &local);
}

void print_variables() {
    int local;
    static int static_local;
    const int const_local;
    printf("Local: %p\n"
           "Static local: %p\n"
           "Const local: %p\n"
           "Global initialized: %p\n"
           "Global not initialized: %p\n"
           "Function: %p\n",
           &local, &static_local, &const_local, &global_init, &global_not_init, &print_variables);
    init_local();
}

int main() {
    printf("Pid: %d\n", getpid());
    print_variables();
    sleep(30);
    return 0;
}