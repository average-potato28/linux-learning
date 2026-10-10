#pragma once
#include <stdint.h>
#include <stddef.h>

#define DEBUGCON_PORT 0xE9

void debugcon_putc(char c);
void debugcon_puts(const char *str);
void debugcon_printf(const char *fmt, ...);

// Convenience alias so you can use kprintf(...) directly
#define kprintf debugcon_printf
