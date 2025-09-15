CC := gcc
INCLUDE := -I/mingw64/include
LIB := -L/mingw64/lib
CFLAGS := -Wall -Wextra $(INCLUDE)
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
BIN := build/main

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) -lcurl -lcjson

build/%.o: src/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -MF $(basename $@).d

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP)

-include $(DEP)

.PHONY: all run clean