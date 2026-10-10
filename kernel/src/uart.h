#pragma once

#include <stdint.h>
#include <stddef.h>

#define UART_COM1_BASE 0x3F8

// Register offsets from COM1 base
#define UART_DATA (UART_COM1_BASE + 0) // Receive buffer / Transmitter holding register / DLL
#define UART_IER  (UART_COM1_BASE + 1) // Interrupt Enable Register / DLM
#define UART_FCR  (UART_COM1_BASE + 2) // FIFO Control Register (write) / IIR (read)
#define UART_LCR  (UART_COM1_BASE + 3) // Line Control Register
#define UART_MCR  (UART_COM1_BASE + 4) // Modem Control Register
#define UART_LSR  (UART_COM1_BASE + 5) // Line Status Register
#define UART_MSR  (UART_COM1_BASE + 6) // Modem Status Register
#define UART_SCR  (UART_COM1_BASE + 7) // Scratch Register

// Backward-compatibility aliases
#define UART_BASE UART_COM1_BASE
#define UART_INTR UART_IER

void uart_init(void);
void uart_putc(char c);
void uart_write(const char *string);
int uart_has_received(void);
char uart_getc(void);
