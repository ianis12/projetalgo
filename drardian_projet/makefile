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

$(glosariesdir)glosaries.o: $(glosariesdir)glosaries.c $(glosariesdir)glosaries.h
$(hashtabledir)hashtable.o: $(hashtabledir)hashtable.c $(hashtabledir)hashtable.h
$(holdalldir)holdall.o: $(holdalldir)holdall.c $(holdalldir)holdall.h
$(listdir)list.o: $(listdir)list.c $(listdir)list.h
$(optiondir)option.o: $(optiondir)option.c $(optiondir)option.h
$(worddir)word.o: $(worddir)word.c $(worddir)word.h
main.o: main.c glosaries.h list.h option.h
