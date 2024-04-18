#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <stdint.h>

unsigned char inb(unsigned short port);
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, int ch, size_t count);
void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint16_t inw(uint16_t port);
void *strncpy(char *dest, const char *src, size_t n);
void sprintf(char *str, const char *format, ...);

#endif