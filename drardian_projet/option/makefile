CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2
LDFLAGS =
objects = main.o option.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

tar: clean
	tar -zcf "$(CURDIR).tar.gz" *.c *.h makefile

main.o: main.c option.h
option.o: option.c option.h
