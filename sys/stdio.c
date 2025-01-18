#include "sys/include/stdio.h"
#include "sys/include/stddef.h" // For size_t
#include "sys/include/stdbool.h"

// Low-level output function (e.g., writing to framebuffer, serial port, etc.)
static void low_level_putchar(char c) {
    volatile char* framebuffer = (volatile char*)0xB8000; // Example address for VGA framebuffer
    static size_t cursor_pos = 0;
    
    if (c == '\n') {
        cursor_pos += 80 - cursor_pos % 80; // Newline, move to next line
    } else {
        framebuffer[cursor_pos++] = c;
        framebuffer[cursor_pos++] = 0x07; // White on black text
    }
}

// Print a single character
void putchar(char c) {
    low_level_putchar(c);
}

// Print a string followed by a newline
void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
    putchar('\n'); // Append newline at the end
}

// Print an integer in different bases (decimal, hexadecimal, etc.)
void print_int(int num, int base) {
    if (num == 0) {
        putchar('0');
        return;
    }

    char buffer[32]; // Large enough buffer for integer storage
    int i = 0;
    bool is_negative = num < 0;

    if (is_negative) {
        num = -num;
    }

    // Convert integer to string based on base
    while (num > 0) {
        int digit = num % base;
        buffer[i++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'A');
        num /= base;
    }

    if (is_negative && base == 10) {
        buffer[i++] = '-';
    }

    // Print the number from the buffer in reverse
    for (int j = i - 1; j >= 0; j--) {
        putchar(buffer[j]);
    }
}

// Print a hexadecimal number (used for memory addresses, etc.)
void print_hex(uint32_t num) {
    putchar('0');
    putchar('x'); // Prefix for hexadecimal representation

    print_int(num, 16); // Print using base 16
}

// Print a floating-point number
void print_float(float num) {
    if (num < 0) {
        putchar('-');
        num = -num;
    }

    int integer_part = (int)num;
    float fractional_part = num - integer_part;

    // Print integer part
    print_int(integer_part, 10);
    putchar('.');

    // Print fractional part with a fixed precision (e.g., 3 decimal places)
    for (int i = 0; i < 3; i++) {
        fractional_part *= 10;
        int frac_digit = (int)fractional_part;
        putchar(frac_digit + '0');
        fractional_part -= frac_digit;
    }
}

// Print a formatted string
void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'c': { // Character
                    char c = va_arg(args, int);
                    putchar(c);
                    break;
                }
                case 's': { // String
                    const char* str = va_arg(args, const char*);
                    puts(str);
                    break;
                }
                case 'd': { // Decimal integer
                    int num = va_arg(args, int);
                    print_int(num, 10);
                    break;
                }
                case 'x': { // Hexadecimal integer
                    uint32_t num = va_arg(args, uint32_t);
                    print_hex(num);
                    break;
                }
                case 'f': { // Float
                    float num = va_arg(args, double); // `double` is promoted from float
                    print_float(num);
                    break;
                }
                case '%': { // Literal '%'
                    putchar('%');
                    break;
                }
                default:
                    putchar('%');
                    putchar(*fmt);
                    break;
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}
