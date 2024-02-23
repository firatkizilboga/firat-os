#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "video.h"
#if defined(__is_libk)
#include <kernel/tt.h>
#endif

int putchar(int ic) {
	write_char(ic, getCurrentCursor(), getCurrentFrame());
}
