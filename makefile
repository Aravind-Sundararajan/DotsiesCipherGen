CC = g++
CFLAGS = -g -Wall
INCLUDES = -I ./src/ -I /usr/local/include
srcdir = ./src/
bindir = ./bin/
base.o: $(srcdir)base.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)base.cpp -o $(bindir)base.o
letter.o: $(srcdir)letter.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)letter.cpp -o $(bindir)letter.o
word.o: $(srcdir)word.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)word.cpp -o $(bindir)word.o
phrase.o: $(srcdir)phrase.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)phrase.cpp -o $(bindir)phrase.o
kvp.o: $(srcdir)kvp.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)kvp.cpp -o $(bindir)kvp.o	
table.o: $(srcdir)table.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)table.cpp -o $(bindir)table.o		
mat.o: $(srcdir)mat.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)mat.cpp -o $(bindir)mat.o
parser.o: $(srcdir)parser.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)parser.cpp -o $(bindir)parser.o
test.o: $(srcdir)test_letter.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(srcdir)test_letter.cpp -o $(bindir)test.o
test: base.o letter.o word.o phrase.o kvp.o table.o mat.o parser.o test.o 
	$(CC) $(CFLAGS) $(INCLUDES) -o test $(bindir)base.o $(bindir)letter.o $(bindir)word.o $(bindir)phrase.o $(bindir)kvp.o $(bindir)table.o $(bindir)mat.o $(bindir)parser.o $(bindir)test.o
all: test

clean:
