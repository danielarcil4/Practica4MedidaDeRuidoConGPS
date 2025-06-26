#include "terminal.h"
#include <string.h>

// Declare uart_read_line prototype
char* uart_read_line(void);

void init_terminal() {
    // UART USB CDC
}

void check_terminal() {
    if (strcmp(uart_read_line(), "DUMP") == 0) {
        //dump_all_data();
    }
}
