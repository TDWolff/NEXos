#include "print.h"
#include "keyboard.h"
#include "string.h"
#include "filesystem.h"

char line[256];
char file_buffer[SECTOR_SIZE]; // Temporary buffer for file content

void nano_command(const char* filename, const char* location) {
    File* file = fs_create_file(0, filename, location);
    if (file == NULL) {
        print_str("Failed to create file\n");
        return;
    }

    print_str("Type your text. Press Ctrl + S to save and Ctrl + X to exit\n");
}

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
        if (strncmp(line, "nano ", 5) == 0) {
            const char* filename = line + 5;
            const char* location = "/";
            print_str("Creating file...\n");
            nano_command(filename, location);
        } else if (strcmp(line, "ls") == 0) {
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