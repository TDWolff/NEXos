#include "io.h"
#include <stddef.h>
#include <stdint.h>

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void* memset(void* dest, int ch, size_t count) {
    char* dst8 = (char*)dest;

    while (count--) {
        *dst8++ = ch;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t count) {
    char* dst8 = (char*)dest;
    const char* src8 = (const char*)src;

    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void outw(uint16_t port, uint16_t val) {
    asm volatile ( "outw %0, %1" : : "a"(val), "Nd"(port) );
}

uint16_t inw(uint16_t port) {
    uint16_t result;
    asm volatile ( "inw %1, %0" : "=a"(result) : "Nd"(port) );
    return result;
}

void *strncpy(char *dest, const char *src, size_t n) {
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for ( ; i < n; i++)
        dest[i] = '\0';

    return dest;
}

typedef char* va_list;

void sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
}
