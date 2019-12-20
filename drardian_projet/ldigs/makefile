digsdir = ../digs/
CC = gcc
CFLAGS = -std=c11 -g -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings -O2 -I$(digsdir) -DDIGS_COMPAR_LEN=1
LDFLAGS =
VPATH = $(digsdir)
objects = $(digsdir)digs.o ldigs.o main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

digs.o: digs.c digs.h
ldigs.o: ldigs.c ldigs.h digs.h
main.o: main.c ldigs.h digs.h
