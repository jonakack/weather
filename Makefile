CC := gcc
INCLUDE := -I/mingw64/include
LIB := -L/mingw64/lib -Linclude/libs
CFLAGS := -std=c11 -Wall -Wextra $(INCLUDE)
SRC_FILES := $(wildcard src/*.c)
LIB_FILES := $(wildcard include/libs/*.c)
SRC := $(SRC_FILES) $(LIB_FILES)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC_FILES)) $(patsubst include/libs/%.c,build/%.o,$(LIB_FILES))
DEP := $(OBJ:.o=.d)
BIN := build/main

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB) -lcurl -lcjson

build/%.o: src/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -MF $(basename $@).d

build/%.o: include/libs/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@ -MF $(basename $@).d

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJ) $(DEP)

-include $(DEP)

.PHONY: all run clean