#include "terminal.h"

void init_terminal() {
    // UART USB CDC
}

void check_terminal() {
    if (strcmp(uart_read_line(), "DUMP") == 0) {
        dump_all_data();
    }
}
