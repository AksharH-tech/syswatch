# syswatch

A lightweight Linux system monitoring tool written in C.

`syswatch` periodically polls the Linux `/proc` filesystem to detect
system activity and anomalies, including CPU usage, memory usage,
process lifecycle changes, and newly opened listening ports.

It is designed to be:
- Simple
- Efficient
- Dependency-free
- Easy to extend

---

## Features

### CPU Monitoring
- Calculates CPU usage using changes from `/proc/stat`
- Accurate across polling intervals

### Memory Monitoring
- Reads `/proc/meminfo`
- Uses `MemAvailable` for realistic memory usage
- Reports percentage of memory used

### Process Monitoring
- Detects newly started processes
- Detects terminated processes
- Reports PID and process name

### Port Monitoring
- Detects new listening TCP and UDP ports
- Supports IPv4 and IPv6
- Parses `/proc/net/tcp`, `/proc/net/tcp6`, `/proc/net/udp`, `/proc/net/udp6`
- Reports protocol and port number

---

## Build

Requirements:
- Linux
- GCC
- Make

Clone and build:

```sh
git clone https://github.com/AksharH-tech/syswatch.git
cd syswatch
make
```

---

## Run

```sh
./syswatch
```

Example output:

```
syswatch starting...
CPU usage: 8.34%
Memory Usage: 41.27%
Process started: PID 8421 (firefox)
New tcp port listening: 8080
```

The program runs continuously and polls the system at a fixed interval.

---

## Project Structure

```
.
├── include/
│   └── syswatch.h
├── src/
│   ├── main.c
│   ├── cpu.c
│   ├── mem.c
│   ├── process.c
│   └── ports.c
├── Makefile
└── README.md
```

---

## Design Notes

- Uses `/proc` directly — no shell commands or external tools
- All monitoring logic is snapshot-based and diffed
- Low CPU and memory overhead
- Modular layout makes it easy to add features

---

## Limitations

- Linux-only
- No persistence or logging (stdout only)
- No alert thresholds or notifications
- Designed for interactive or background terminal use

---

## Possible Future Extensions

- File logging
- Config file support
- CPU / memory alert thresholds
- ncurses-based TUI
- JSON output mode
- GUI frontend using a separate process

---

## License

MIT License

