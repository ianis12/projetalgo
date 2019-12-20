hashtabledir = ../hashtable/
holdalldir = ../holdall/
listdir = ../list/
CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(listdir) -O2
LDFLAGS =
VPATH = $(hashtabledir):$(holdalldir):$(listdir)
objects = $(hashtabledir)hashtable.o $(holdalldir)holdall.o $(listdir)list.o main.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(holdalldir)holdall.o: holdall.c holdall.h
$(listdir)list.o: list.c list.h
main.o: main.c hashtable.h holdall.h list.h

