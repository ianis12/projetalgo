#main
#glosariesdir = glosaries/
#hashtabledir = hashtable/
#holdalldir = holdall/
listdir = list/
optiondir = option/
#worddir = word/
CC = gcc
#CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(hashtabledir) -I$(holdalldir) -I$(listdir) -I$(optiondir) -I$(worddir) -O2
#CFLAGS = -std=c11 -Wall -Wconversion -Werror -Wextra -Wpedantic -I$(listdir) -I$(optiondir) -O2
CFLAGS = -std=c11 -I$(listdir) -I$(optiondir)
LDFLAGS =
#VPATH = $(glosariesdir):$(hashtabledir):$(holdalldir):$(listdir):$(optiondir):$(worddir)
VPATH = $(listdir):$(optiondir)
#objects = $(glosariesdir)glosaries.o $(hashtabledir)hashtable.o $(holdalldir)holdall.o $(listdir)list.o $(optiondir)option.o $(worddir)word.o main.o
objects = $(listdir)list.o $(optiondir)option.o main.o
executable = lidx

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

#$(glosariesdir)glosaries.o: $(glosariesdir)glosaries.c $(glosariesdir)glosaries.h
#$(hashtabledir)hashtable.o: $(hashtabledir)hashtable.c $(hashtabledir)hashtable.h
#$(holdalldir)holdall.o: $(holdalldir)holdall.c $(holdalldir)holdall.h
$(listdir)list.o: $(listdir)list.c $(listdir)list.h
$(optiondir)option.o: $(optiondir)option.c $(optiondir)option.h
#$(worddir)word.o: $(worddir)word.c $(worddir)word.h
#main.o: main.c glosaries.h hashtable.h holdall.h list.h option.h word.h
main.o: main.c list.h option.h
