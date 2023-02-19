#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "libs.h"

int main() {
	void *handle;
	int (*func)(int num);
	handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6", RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "dlopen() error: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}
    char *lib_name = "abs";	
	func = dlsym(handle, lib_name);
    if (!func) {
        printf("%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
	printf("%d\n", (*func)(-4));

	dlclose(handle);
	return 0;
}
