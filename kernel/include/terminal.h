#ifndef TERMINAL_H
#define TERMINAL_H 1

#include <stdint.h> // For standard integer types
#include <stddef.h> // For size_t

// Structure representing a GDT entry
typedef struct {
    size_t x;
    size_t y;
	bool is_blinking;
} Cursor;

void update_cursor(Cursor * cursor, size_t x, size_t y);
void write_char(char c, uint8_t color, Cursor * cursor);
void write_string(const char * str, uint8_t color, Cursor * cursor);
void terminal_initialize(void);
void terminal_clear(Cursor* cursor);
Cursor * get_terminal_cursor();

#endif // TERMINAL_H