CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC = src/main.c src/cpu.c src/mem.c src/process.c src/ports.c
OBJ = $(SRC:.c=.o)
BIN = syswatch

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

clean:
	rm -f $(OBJ) $(BIN)
