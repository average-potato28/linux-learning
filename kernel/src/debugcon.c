#include "debugcon.h"
#include "io.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

void debugcon_putc(char c) {
    outb(DEBUGCON_PORT, (uint8_t)c);
}

void debugcon_puts(const char *str) {
    if (!str) {
        str = "(null)";
    }
    while (*str) {
        if (*str == '\n') {
            debugcon_putc('\r');
        }
        debugcon_putc(*str++);
    }
}

static void print_uint(uint64_t val, int base, bool uppercase) {
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    char buf[65];
    int idx = 0;

    if (val == 0) {
        debugcon_putc('0');
        return;
    }

    while (val > 0) {
        buf[idx++] = digits[val % base];
        val /= base;
    }

    for (int i = idx - 1; i >= 0; i--) {
        debugcon_putc(buf[i]);
    }
}

static void print_int(int64_t val) {
    if (val < 0) {
        debugcon_putc('-');
        val = -val;
    }
    print_uint((uint64_t)val, 10, false);
}

void debugcon_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    for (const char *p = fmt; *p != '\0'; p++) {
        if (*p != '%') {
            if (*p == '\n') {
                debugcon_putc('\r');
            }
            debugcon_putc(*p);
            continue;
        }

        p++;
        switch (*p) {
            case 's': {
                const char *s = va_arg(args, const char *);
                debugcon_puts(s);
                break;
            }
            case 'c':
                debugcon_putc((char)va_arg(args, int));
                break;
            case 'd':
            case 'i':
                print_int(va_arg(args, int));
                break;
            case 'u':
                print_uint(va_arg(args, unsigned int), 10, false);
                break;
            case 'x':
                print_uint(va_arg(args, unsigned int), 16, false);
                break;
            case 'X':
                print_uint(va_arg(args, unsigned int), 16, true);
                break;
            case 'p':
                debugcon_puts("0x");
                print_uint((uintptr_t)va_arg(args, void *), 16, false);
                break;
            case '%':
                debugcon_putc('%');
                break;
            default:
                debugcon_putc('%');
                debugcon_putc(*p);
                break;
        }
    }

    va_end(args);
}
