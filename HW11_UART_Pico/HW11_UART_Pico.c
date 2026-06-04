#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN 4
#define UART_RX_PIN 5

int main()
{
    // USB serial (PC monitor)
    stdio_usb_init();

    // UART to STM32
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    while (true)
    {
        // ===== PC (USB serial) -> STM32 UART =====
        int usb_ch = getchar_timeout_us(0);
        if (usb_ch != PICO_ERROR_TIMEOUT)
        {
            uart_putc(UART_ID, (char)usb_ch);
        }

        // ===== STM32 UART -> PC (USB serial) =====
        if (uart_is_readable(UART_ID))
        {
            char c = uart_getc(UART_ID);
            printf("%c", c);
        }
    }
}