#ifndef AION_STDIO_H
#define AION_STDIO_H

#include <stdarg.h>
#include <stdint.h>

// Function prototypes
void putchar(char c);            // Print a single character
void puts(const char* str);      // Print a string with a newline
void printf(const char* fmt, ...); // Print a formatted string
void print_int(int num, int base); // Print integer in different bases (e.g., hex)
void print_hex(uint32_t num);    // Print hexadecimal number
void print_float(float num);     // Print floating-point numbers

#endif // AION_STDIO_H
