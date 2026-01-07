#include <stdio.h>
#include <unistd.h>
#include "syswatch.h"

int main(void) {
    printf("syswatch starting...\n");

    while (1) {
        check_cpu();
        check_mem();
        sleep(DEFAULT_POLL_INTERVAL);
    }

    return 0;
}

