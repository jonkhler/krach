CC=gcc
EXTERNAL=$(shell pkg-config --cflags --libs libpipewire-0.3) -lm
CFLAGS = -Wall -Werror -g -I./include
DEPS = include/audio.h
SRC = first.c audio.c
OBJ = $(SRC:.c=.o)
EXEC = run

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(EXTERNAL)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(EXTERNAL)

.PHONY: clean

clean:
	rm -f *.o $(EXEC)

.PHONY: log

log: $(EXEC)
	@make > build.log 2>&1 && less build.log || cat build.log

.PHONY: all

all: $(EXEC)


