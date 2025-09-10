CC := gcc
CFLAGS := -std=c90 -Wall -Wextra
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)
BIN := main

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP)

-include $(DEP)

.PHONY: all run clean