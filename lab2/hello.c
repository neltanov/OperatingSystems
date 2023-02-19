#include <unistd.h>
#include <sys/syscall.h>

int main(void) {
    syscall(SYS_write, 1, "Hello, world!\n", 14);
    return 0;
}
