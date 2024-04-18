#include "print.h"
#include "shell.h"
#include "filesystem.h"
#include "mem.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_CYAN, PRINT_COLOR_BLACK);
    print_str("Welcome to NEX operating system!");
    print_newline();
    shell_loop();
    mem_init();
    fs_init();
}