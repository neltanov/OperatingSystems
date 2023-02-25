#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <errno.h>

void parent(pid_t pid) {
    int status;
    waitpid(pid, &status, 0);
    ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD);

    while (!WIFEXITED(status)) {

        struct user_regs_struct state;
    
        ptrace(PTRACE_SYSCALL, pid, 0, 0);
        waitpid(pid, &status, 0);
    
        // at syscall
        if (WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
            ptrace(PTRACE_GETREGS, pid, 0, &state);
            printf("SYSCALL %lld at %08llx\n", state.orig_rax, state.rip);
      
        // skip after syscall
            ptrace(PTRACE_SYSCALL, pid, 0, 0);
            waitpid(pid, &status, 0);
        } 
    }
}

void child() {
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    execl("./hello", NULL);
    perror("execl");
}

int main() {
    pid_t pid = fork();
    printf("%d\n", pid);
    if (pid) {
        parent(pid);
    }
    else {
        child();
    }
    
    return 0;
}
