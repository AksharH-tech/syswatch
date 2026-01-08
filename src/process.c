#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "syswatch.h"

#define MAX_PIDS 65536

typedef struct {
    int pid;
    char name[32];
} proc_entry_t;

static proc_entry_t prev[MAX_PIDS];
static int prev_count = 0;

static void get_process_name(int pid, char *buf, size_t size) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);

    FILE *f = fopen(path, "r");
    if (!f) {
        snprintf(buf, size, "unknown");
        return;
    }

    if (fgets(buf, size, f)) {
        /* strip newline */
        char *nl = strchr(buf, '\n');
        if (nl) *nl = '\0';
    } else {
        snprintf(buf, size, "unknown");
    }

    fclose(f);
}

static int is_number(const char *s) {
    while (*s) {
        if (!isdigit(*s))
            return 0;
        s++;
    }
    return 1;
}

void check_processes(void) {
    DIR *dir = opendir("/proc");
    if (!dir) return;

    proc_entry_t curr[MAX_PIDS];
    int curr_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && is_number(entry->d_name)) {
            if (curr_count < MAX_PIDS){
                curr[curr_count].pid = atoi(entry->d_name);
                curr_count++;
            }
        }
    }

    closedir(dir);

    /* detect new processes */
    for (int i = 0; i < curr_count; i++) {
        int found = 0;
        for (int j = 0; j < prev_count; j++) {
            if (curr[i].pid == prev[j].pid) {
                found = 1;
                break;
            }
        }
        if (!found) {
            get_process_name(curr[i].pid, curr[i].name, sizeof(curr[i].name));
            printf("Process started: PID %d (%s)\n",
                curr[i].pid, curr[i].name);
        }
    }

    /* detect terminated processes */
    for (int i = 0; i < prev_count; i++) {
        int found = 0;
        for (int j = 0; j < curr_count; j++) {
            if (prev[i].pid == curr[j].pid) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Process terminated: PID %d (%s)\n",
                prev[i].pid, prev[i].name);
        }
    }

    /* update snapshot */
    memcpy(prev, curr, curr_count * sizeof(proc_entry_t));
    prev_count = curr_count;
}

