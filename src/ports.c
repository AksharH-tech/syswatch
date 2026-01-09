#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syswatch.h"

#define MAX_PORTS 4096

typedef struct {
    int port;
    char proto[4];
} port_entry_t;

static port_entry_t prev[MAX_PORTS];
static int prev_count = 0;

static int port_exists(port_entry_t *list, int count, int port, const char *proto) {
    for (int i = 0; i < count; i++) {
        if (list[i].port == port && strcmp(list[i].proto, proto) == 0)
            return 1;
    }
    return 0;
}

static void parse_file(const char *path, const char *proto,
                       port_entry_t *curr, int *curr_count) {
    FILE *f = fopen(path, "r");
    if (!f) return;

    char line[256];
    fgets(line, sizeof(line), f);

    while (fgets(line, sizeof(line), f)) {
        unsigned int port, state;
        char local[64];

        if (sscanf(line, "%*d: %63[^:]:%X %*s %X",
                   local, &port, &state) != 3)
            continue;

        if (strcmp(proto, "tcp") == 0 && state != 0x0A)
            continue;

        if (*curr_count < MAX_PORTS) {
            curr[*curr_count].port = port;
            strncpy(curr[*curr_count].proto, proto, 4);
            (*curr_count)++;
        }
    }

    fclose(f);
}

void check_ports(void) {
    port_entry_t curr[MAX_PORTS];
    int curr_count = 0;

    parse_file("/proc/net/tcp",  "tcp", curr, &curr_count);
    parse_file("/proc/net/tcp6", "tcp", curr, &curr_count);
    parse_file("/proc/net/udp",  "udp", curr, &curr_count);
    parse_file("/proc/net/udp6", "udp", curr, &curr_count);

    for (int i = 0; i < curr_count; i++) {
        if (!port_exists(prev, prev_count, curr[i].port, curr[i].proto)) {
            printf("New %s port listening: %d\n",
                   curr[i].proto, curr[i].port);
        }
    }

    memcpy(prev, curr, curr_count * sizeof(port_entry_t));
    prev_count = curr_count;
}

