#pragma once
#ifndef TERMINAL_H
#define TERMINAL_H 1

#include <stdint.h> // For standard integer types
#include <stddef.h> // For size_t
#include "video.h"

// Structure representing a GDT entry

void terminal_initialize(void);
#endif // TERMINAL_H