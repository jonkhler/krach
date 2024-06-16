CC=clang
CFLAGS=-Wall -Werror -g -I./include
LDFLAGS=$(shell pkg-config --cflags --libs libpipewire-0.3) -lm
DEPS = include/audio.h include/graph.h include/mix.h
SRC = src/audio.c src/graph.c src/mix.c src/first.c
OBJ = $(SRC:src/%.c=build/%.o)
EXEC = build/app

$(shell mkdir -p build)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f build/* 

.PHONY: log
log: $(EXEC)
	@make > build.log 2>&1 && less build.log || cat build.log
