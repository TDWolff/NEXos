#include "print.h"
#include "keyboard.h"
#include "string.h"

char line[256];

void read_line() {
    int pos = 0;
    while (1) {
        char ch = keyboard_read_char();
        if (ch == '\n') {
            line[pos] = 0;
            return;
        } else if (ch == '\b') { // Backspace
            if (pos > 0) {
                pos--;
                print_char('\b');
            }
        } else if (ch >= ' ' && ch <= '~') {
            line[pos] = ch;
            pos++;
            print_char(ch);
        }
    }
}

void shell_loop() {
    while (1) {
        print_str("> ");
        read_line();
        print_char('\n');
        if (strcmp(line, "ls") == 0) {
            print_str("ls command is not implemented yet\n");
        } else if (strncmp(line, "echo ", 5) == 0) {
            print_str(line + 5);
            print_char('\n');
        } else {
            print_str("Unknown command: ");
            print_str(line);
            print_char('\n');
        }
    }
}