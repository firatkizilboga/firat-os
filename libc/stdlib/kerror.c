#include <stdio.h>
#include <stdlib.h>

void kerror(const char *msg) {
	// TODO: Abnormally terminate the process as if by SIGABRT.
    printf("Error: %s\n", msg);
	printf("abort()\n");
	abort();
	while (1) { }
}
