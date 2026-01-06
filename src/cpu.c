#include <stdio.h>
#include <time.h>
#include "syswatch.h"

static unsigned long long prev_total = 0;
static unsigned long long prev_idle = 0;

void check_cpu(void) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return;

    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle,
           &iowait, &irq, &softirq, &steal);
    fclose(fp);

    unsigned long long idle_time = idle + iowait;
    unsigned long long total_time =
        user + nice + system + idle + iowait + irq + softirq + steal;

    if (prev_total == 0) {
        prev_total = total_time;
        prev_idle = idle_time;
        return;
    }

    unsigned long long total_delta = total_time - prev_total;
    unsigned long long idle_delta = idle_time - prev_idle;

    double usage = 100.0 * (1.0 - (double)idle_delta / total_delta);

    printf("CPU usage: %.2f%%\n", usage);

    prev_total = total_time;
    prev_idle = idle_time;
}

