#include "keyboard.h"
#include "io.h"

#define KEYBOARD_DATA_PORT 0x60

// This is a more complete scan code to ASCII conversion table.
// It handles unshifted alphanumeric keys, a few other keys, and the arrow keys.
// It does not handle shifted keys or special keys like Ctrl or Alt.
static const char SCAN_CODE_TO_ASCII[] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', // 0-9
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', // 10-19
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, // 20-29
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 30-39
    '\'', '`', 0,   '\\','z', 'x', 'c', 'v', 'b', 'n', // 40-49
    'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   // 50-59
    0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', // 60-69
    '+', '1', '2', '3', '0', '.', 0,   0,   0,   0,   // 70-79
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 80-89
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 90-99
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 100-109
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 110-119
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 120-129
};

char keyboard_read_char() {
    unsigned char scan_code;
    char ascii_char;

    while (!(inb(0x64) & 0x1)) {}

    scan_code = inb(KEYBOARD_DATA_PORT);

    if (scan_code == 0x0E) {
        return '\b'; // Backspace/delete key
    }

    if (scan_code > 128) {
        return 0;
    }

    ascii_char = SCAN_CODE_TO_ASCII[scan_code];

    return ascii_char;
}


// input detection for nano

char read_char() {
    unsigned char scan_code;
    char ascii_char;

    while (!(inb(0x64) & 0x1)) {}

    scan_code = inb(KEYBOARD_DATA_PORT);

    if (scan_code == 0x0E) {
        return '\b'; // Backspace/delete key
    }

    if (scan_code > 128) {
        return 0;
    }

    ascii_char = SCAN_CODE_TO_ASCII[scan_code];

    return ascii_char;
}