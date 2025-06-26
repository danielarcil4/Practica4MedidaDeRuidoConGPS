#include "power.h"

void enter_sleep_mode() {
    // SLEEP o DORMANT
    __wfi(); // Wait for Interrupt (low power standby)
}

void wake_from_sleep() {
    // This function can be used to handle any wake-up logic if needed
    // For now, it does nothing as the system will wake up on any interrupt
}