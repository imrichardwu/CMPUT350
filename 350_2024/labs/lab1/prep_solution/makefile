#  make        refreshes executable
#  make clean  removes all created files and executable
#  make dep    lists file dependencies that can be pasted below

WARN := -Wall -Wextra -Wconversion -Wsign-conversion

# debug mode
COPTS  := $(WARN) -O -g
CCOPTS := $(WARN) -O -g -std=c++17

all: e1c e2 e3

# how to create executable from single object file name
e1c : 
	gcc $(COPTS) e1c.c -o e1c

e2 : 
	g++ $(CCOPTS) e2.cpp -o e2

e3: 
	g++ $(CCOPTS) e3.cpp -o e3

# other file dependencies
# (paste output of make dep here)

# remove generated files
clean:	
	rm -f *.o e1c e2 e3
