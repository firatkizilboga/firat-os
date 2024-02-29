#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);
__attribute__((__noreturn__))
void kerror(const char *msg);
void* malloc(size_t);
void free(void*);

#ifdef __cplusplus
}
#endif

#endif
