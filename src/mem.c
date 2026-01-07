#include <stdio.h>
#include "syswatch.h"
#include <string.h>

void check_mem(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return;

    unsigned long long total = 0, available = 0;
    char key[64];
    unsigned long long value;
    char unit[32];

    while (fscanf(f, "%63s %llu %31s", key, &value, unit) == 3) {
        if (strcmp(key, "MemTotal:") == 0)
            total = value;
        else if (strcmp(key, "MemAvailable:") == 0)
            available = value;
    }

    fclose(f);

    if (total > 0) {
        double used = 100.0 * (1.0 - (double)available / total);
        printf("Memory Usage: %.2f%%\n", used);
    }
}

