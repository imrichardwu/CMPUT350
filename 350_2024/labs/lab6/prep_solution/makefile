#  make        refreshes executable
#  make clean  removes all created files and executable
#  make dep    lists file dependencies that can be pasted below

WARN := -Wall -Wextra -Wconversion -Wsign-conversion

# debug mode
CCOPTS := $(WARN) -O -g -std=c++17

# compiler and linker
CC := g++

all: pp1 pp2

# how to create executable from object files
pp1 : pp1.o
	$(CC) -o $@ $^
pp2 : pp2.o
	$(CC) -o $@ $^

# how to create .o files from .c files
%.o : %.cpp makefile
	$(CC) $(CCOPTS) -c -o $@ $<

# other file dependencies
# (paste output of make dep here)

dep:
	$(CC) -MM *.cpp

# remove generated files
clean:	
	rm -f *.o pp1 pp2
