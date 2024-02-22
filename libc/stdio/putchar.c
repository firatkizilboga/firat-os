#include <stdio.h>
#include <stdbool.h>
#include <../../kernel/include/terminal.h>
#include <../../kernel/include/video.h>
#if defined(__is_libk)
#include <kernel/tt.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
#else
	write_char(ic, 
		vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK)
	, get_terminal_cursor());
#endif
	return ic;
}
