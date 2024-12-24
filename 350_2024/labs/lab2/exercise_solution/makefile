#  make        refreshes executable
#  make clean  removes all created files and executable
#  make dep    lists file dependencies that can be pasted below

WARN := -Wall -Wextra -Wconversion -Wsign-conversion

# debug mode
CCOPTS := $(WARN) -O -g -std=c++17

# compiler and linker
CC := g++

all: matrix

# how to create executable from object files
matrix : Matrix.o matrixTest.o
	$(CC) -o matrix $^

# how to create .o files from .c files
%.o : %.c makefile
	$(CC) $(CCOPTS) -c -o $@ $<

# other file dependencies
# (paste output of make dep here)
Matrix.o: Matrix.cpp Matrix.h
matrixTest.o: matrixTest.cpp Matrix.h

dep:
	$(CC) -MM *.cpp

# remove generated files
clean:	
	rm -f *.o matrix
