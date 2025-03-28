CC = g++
CFLAGS = -g -Wall -O3 -march=native -std=c++11
DEBUG_FLAGS = -DNDEBUG
INCLUDES = -I ./src/ -I /usr/local/include
srcdir := ./src/
bindir := ./bin/

# Create bin directory if it doesn't exist
$(shell mkdir -p $(bindir))

base.o: $(srcdir)base.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)base.cpp -o $(bindir)base.o

letter.o: $(srcdir)letter.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)letter.cpp -o $(bindir)letter.o

kvp.o: $(srcdir)kvp.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)kvp.cpp -o $(bindir)kvp.o

table.o: $(srcdir)table.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)table.cpp -o $(bindir)table.o

mat.o: $(srcdir)mat.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)mat.cpp -o $(bindir)mat.o

ciphertext.o: $(srcdir)ciphertext.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)ciphertext.cpp -o $(bindir)ciphertext.o

parser.o: $(srcdir)parser.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)parser.cpp -o $(bindir)parser.o

decrypter.o: $(srcdir)decrypter.h
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)decrypter.cpp -o $(bindir)decrypter.o

# Compile test_letter.cpp with NO_NCURSES flag for text-only version
test.o: $(srcdir)test_letter.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -DNO_NCURSES $(INCLUDES) -c $(srcdir)test_letter.cpp -o $(bindir)test.o

# Compile test_letter.cpp without NO_NCURSES flag for ncurses version
test_ncurses.o: $(srcdir)test_letter.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)test_letter.cpp -o $(bindir)test_ncurses.o

# Compile decrypter_demo.cpp
decrypter_demo.o: $(srcdir)decrypter_demo.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)decrypter_demo.cpp -o $(bindir)decrypter_demo.o

# Build the ncurses version
test: base.o letter.o kvp.o table.o mat.o ciphertext.o parser.o test_ncurses.o
	$(CC) $(CFLAGS) $(INCLUDES) -o test $(bindir)base.o $(bindir)letter.o $(bindir)kvp.o $(bindir)table.o $(bindir)mat.o $(bindir)ciphertext.o $(bindir)parser.o $(bindir)test_ncurses.o -lncurses -lpanel

# Build the text-only version
nocurses: base.o letter.o kvp.o table.o mat.o ciphertext.o parser.o test.o
	$(CC) $(CFLAGS) $(INCLUDES) -o nocurses $(bindir)base.o $(bindir)letter.o $(bindir)kvp.o $(bindir)table.o $(bindir)mat.o $(bindir)ciphertext.o $(bindir)parser.o $(bindir)test.o

# Build the decrypter demo program
decrypter_demo: base.o letter.o kvp.o table.o mat.o ciphertext.o parser.o decrypter.o decrypter_demo.o
	$(CC) $(CFLAGS) $(INCLUDES) -o decrypter_demo $(bindir)base.o $(bindir)letter.o $(bindir)kvp.o $(bindir)table.o $(bindir)mat.o $(bindir)ciphertext.o $(bindir)parser.o $(bindir)decrypter.o $(bindir)decrypter_demo.o

mat_example.o: $(srcdir)mat_example.cpp
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $(srcdir)mat_example.cpp -o $(bindir)mat_example.o

mat_example: base.o mat.o mat_example.o
	$(CC) $(CFLAGS) $(INCLUDES) -o mat_example $(bindir)base.o $(bindir)mat.o $(bindir)mat_example.o

clean:
	rm -f *.exe test nocurses mat_example decrypter_demo && cd $(bindir) && rm -f *.o

all: test mat_example nocurses decrypter_demo
