CC = g++
CFLAGS = -g -Wall
INCLUDES = -I ./src/ -I /usr/local/include
srcdir = ./src/
bindir = ./bin/
base.o:
	$(CC) $(CFLAGS) -c $(srcdir)base.cpp -o $(bindir)base.o
letter.o:
	$(CC) $(CFLAGS) -c $(srcdir)letter.cpp -o $(bindir)letter.o
word.o:
	$(CC) $(CFLAGS) -c $(srcdir)word.cpp -o $(bindir)word.o

test: base.o letter.o word.o
	$(CC) $(CFLAGS) $(srcdir)test_letter.cpp -o $(bindir)test

all: test

clean:
