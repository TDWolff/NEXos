#include "print.h"
#include <stdint.h>

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

int cursor_row = 0;
int cursor_col = 0;

struct Char {
    uint8_t character;
    uint8_t color;
};

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void print_newline() {
    col = 0;

    if (row < NUM_ROWS - 1) {
        row++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }

    clear_row(NUM_COLS - 1);
}

void move_cursor() {
    uint16_t pos = row * NUM_COLS + col;

    outb(0x3D4, 14); // Tell the VGA controller that we're setting the high cursor byte.
    outb(0x3D5, pos >> 8); // Send the high cursor byte.
    outb(0x3D4, 15); // Tell the VGA controller that we're setting the low cursor byte.
    outb(0x3D5, pos); // Send the low cursor byte.
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
    } else if (character == '\b') { // Backspace
        if (col > 0) {
            col--;
            buffer[col + NUM_COLS * row] = (struct Char) {
                character: (uint8_t) ' ',
                color: color,
            };
            move_cursor();
        }
    } else {        
        if (col > NUM_COLS) {
            print_newline();
        }

        buffer[col + NUM_COLS * row] = (struct Char) {
            character: (uint8_t) character,
            color: color,
        };

        col++;
    }

    move_cursor(); // Update the cursor position after printing a character.
}

void print_str(char* str) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];

        if (character == '\0') {
            return;
        }

        print_char(character);
    }
}

void print_int(char* str) {
// take in a float/int/var from another function and print it to the screen
    print_str(str);
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}