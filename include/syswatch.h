#ifndef SYSWATCH_H
#define SYSWATCH_H

#define DEFAULT_POLL_INTERVAL 5
#define CPU_THRESHOLD 85.0

void check_cpu(void);
void check_mem(void);
void check_processes(void);

#endif

