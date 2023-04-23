#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Pid: %d\n", getpid());
    sleep(1);
    execve(argv[0], argv, __environ);
    printf("Hello, world!\n");

    sleep(60);
    return 0;
}
