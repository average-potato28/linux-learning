#include "uart.h"
#include "io.h"

/**
 * @brief Initialize the UART (COM1).
 *
 * Configures the UART with 115200 baud, 8 data bits, no parity, and 1 stop bit (8N1).
 * Enables and clears FIFOs, and asserts RTS/DTR.
 */
void uart_init(void) {
    outb(UART_IER, 0x00);                  // Disable interrupts during initialization
    outb(UART_LCR, 0x80);                  // Enable DLAB (set baud rate divisor)
    outb(UART_DATA, 0x01);                 // Set divisor low byte to 1 (115200 baud)
    outb(UART_IER, 0x00);                  // Set divisor high byte to 0
    outb(UART_LCR, 0x03);                  // 8 bits, no parity, 1 stop bit (8N1), DLAB cleared
    outb(UART_FCR, 0xC7);                  // Enable FIFO, clear TX/RX queues, 14-byte threshold
    outb(UART_MCR, 0x0B);                  // OUT2 (IRQs enabled), RTS/DTR set
}

/**
 * @brief Check if transmit holding register is empty.
 */
static inline int uart_is_transmit_empty(void) {
    return inb(UART_LSR) & 0x20;
}

/**
 * @brief Write a single character to the UART.
 * @param c The character to send.
 */
void uart_putc(char c) {
    while (!uart_is_transmit_empty()) {
        __asm__ volatile("pause");
    }
    outb(UART_DATA, (uint8_t)c);
}

/**
 * @brief Write a null-terminated string to the UART.
 * @param string The null-terminated string to write.
 */
void uart_write(const char *string) {
    if (!string) {
        string = "(null)";
    }
    for (size_t i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\n' && (i == 0 || string[i - 1] != '\r')) {
            uart_putc('\r');
        }
        uart_putc(string[i]);
    }
}

/**
 * @brief Check if data is available to read from UART.
 */
int uart_has_received(void) {
    return inb(UART_LSR) & 0x01;
}

/**
 * @brief Read a single character from the UART (blocking).
 */
char uart_getc(void) {
    while (!uart_has_received()) {
        __asm__ volatile("pause");
    }
    return (char)inb(UART_DATA);
}
