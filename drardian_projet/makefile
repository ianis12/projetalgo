#main:

glosariesdir = glosaries/
hashtabledir = hashtable/
holdalldir = holdall/
listdir = list/
optiondir = option/
worddir = word/

CC = gcc

CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic \
	-I$(glosariesdir) -I$(hashtabledir) -I$(holdalldir) -I$(listdir) \
	-I$(optiondir) -I$(worddir) -O2

LDFLAGS =

VPATH = $(glosariesdir):$(hashtabledir):$(holdalldir):$(listdir):$(optiondir)\
:$(worddir)

objects = $(glosariesdir)glosaries.o $(hashtabledir)hashtable.o \
	$(holdalldir)holdall.o $(listdir)list.o $(optiondir)option.o $(worddir)word.o main.o

executable = lidx

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

$(glosariesdir)glosaries.o: glosaries.c glosaries.h
$(hashtabledir)hashtable.o: hashtable.c hashtable.h
$(holdalldir)holdall.o: holdall.c holdall.h
$(listdir)list.o: list.c list.h
$(optiondir)option.o: option.c option.h
$(worddir)word.o: word.c word.h
main.o: main.c glosaries.h list.h option.h
