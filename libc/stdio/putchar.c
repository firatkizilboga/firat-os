#include <stdio.h>
#include <stdbool.h>
#include <../../kernel/include/terminal.h>
#if defined(__is_libk)
#include <kernel/tt.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	write_char(ic, 0x07, get_terminal_cursor());
#endif
	return ic;
}
