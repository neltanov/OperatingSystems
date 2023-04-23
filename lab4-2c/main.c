#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void f1() {
    char array[4096];
    sleep(1);
    f1();
}

void f2() {
    printf("Function f2 has been invoked\n");
    char *array[10];
    for(int i = 0; i < 10; i++) {
        array[i] = malloc(400000096 * sizeof(char));
        sleep(1);
    }
    for (int i = 0; i < 10; i++) {
        free(array[i]);
    }
}

int main() {
    printf("Pid: %d\n", getpid());
    sleep(10);
//    f1();
    f2();
    return 0;
}
