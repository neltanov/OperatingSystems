#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int global = 2;

int main() {
    int local = 1;
    printf("Local variable address: %p. Value: %d\n", &local, local);
    printf("Global variable address: %p. Value: %d\n", &global, global);
    printf("Pid: %d\n", getpid());

    int pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("Child pid: %d\n", getpid());
        printf("Parent pid: %d\n", getppid());
        printf("Local variable address in child process: %p. Value: %d\n", &local, local);
        printf("Global variable address in child process: %p. Value: %d\n", &global, global);
        global = 22;
        local = 11;
        printf("Local variable address in child process after reassigning: %p. Value: %d\n", &local, local);
        printf("Global variable address in child process after reassigning: %p. Value: %d\n", &global, global);

//        int grandson_pid = fork();
//        if (grandson_pid == 0) {
//            printf("Grandson pid: %d\n", getpid());
//            printf("Grandson's parent pid: %d\n", getppid());
//            sleep(10);
//            exit(10);
//        }

        exit(5);
    }

    if (pid > 0) {
        sleep(5);
        printf("Local variable address in parent process: %p. Value: %d\n", &local, local);
        printf("Global variable address in parent process: %p. Value: %d\n", &global, global);
        sleep(30);

//        int wstatus, child_pid;
//        child_pid = wait(&wstatus);
//        if (child_pid == -1) {
//            perror("waitpid");
//            exit(EXIT_FAILURE);
//        }
//
//        if (WIFEXITED(wstatus)) {
//            printf("Child process exited with status = %d\n", WEXITSTATUS(wstatus));
//        } else if (WIFSIGNALED(wstatus)) {
//            printf("Child process killed by signal %d\n", WTERMSIG(wstatus));
//        } else if (WIFSTOPPED(wstatus)) {
//            printf("Child process stopped by signal %d\n", WSTOPSIG(wstatus));
//        } else if (WIFCONTINUED(wstatus)) {
//            printf("Child process continued\n");
//        }

    }

    exit(EXIT_SUCCESS);
}