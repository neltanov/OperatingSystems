#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PAGE_SIZE 0x1000 // 4096 байтов
#define SOFT_DIRTY_BIT 55
#define FILE_SHARED_BIT 61
#define PAGE_SWAPPES_BIT 62
#define PAGE_PRESENT_BIT 63
#define PFN_MASK 0x7fffffffffffff // 55 единиц

static void print_page(uint64_t address, uint64_t data) {
    printf("0x%-16lx : pfn %-16lx soft-dirty %ld file/shared %ld "
           "swapped %ld present %ld\n",
           address,
           data & PFN_MASK,
           (data >> SOFT_DIRTY_BIT) & 1,
           (data >> FILE_SHARED_BIT) & 1,
           (data >> PAGE_SWAPPES_BIT) & 1,
           (data >> PAGE_PRESENT_BIT) & 1);
}

int main(int argc, char *argv[]) {
    char filename[BUFSIZ];
    if (argc != 4) {
        printf("Usage: %s pid start_address end_address\n", argv[0]);
        return 1;
    }

    errno = 0;
    int pid = (int) strtol(argv[1], NULL, 0);
    if (errno) {
        perror("strtol");
        return 1;
    }
    snprintf(filename, sizeof filename, "/proc/%d/pagemap", pid);

    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    uint64_t start_address = strtoul(argv[2], NULL, 0);
    uint64_t end_address = strtoul(argv[3], NULL, 0);
    printf("%lu\n", start_address);
    printf("%lu\n", end_address);

    for (uint64_t i = start_address; i < end_address; i += 0x1000) {
        uint64_t data;
        uint64_t index = (i / PAGE_SIZE) * sizeof(data);
        printf("%lu", index);
        if (pread(fd, &data, sizeof(data), index) != sizeof(data)) {
            perror("pread");
            break;
        }
        print_page(i, data);
    }

    close(fd);
    return 0;
}