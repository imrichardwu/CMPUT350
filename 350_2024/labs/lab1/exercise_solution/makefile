#  make        refreshes executable
#  make clean  removes all created files and executable
#  make dep    lists file dependencies that can be pasted below

WARN := -Wall -Wextra -Wconversion -Wsign-conversion

# debug mode
CCOPTS := $(WARN) -O -g -std=c++17

# compiler and linker
CC := g++

BINS := p1 p2

all: $(BINS) 

# how to create executable from single object file name
$(BINS) : % : %.o
	$(CC) -o $@ $^

# how to create .o files from .c files
%.o : %.cpp makefile
	$(CC) $(CCOPTS) -c -o $@ $<

# other file dependencies
# (paste output of make dep here)

# remove generated files
clean:	
	rm -f *.o $(BINS)
