CC=g++ -O3 -g -Wall -Wextra -std=c++17

#CC=g++ -std=c++17 -O3 -g -Wall -Wextra -fsanitize=undefined -fno-sanitize=object-size -fsanitize=address -fsanitize=leak -fsanitize=signed-integer-overflow -fsanitize=bounds-strict

all: testGrid

test: testGrid
	./testGrid

clean:
	rm -f ./testGrid

testGrid: TestGrid.cpp Grid.cpp Grid.h
	$(CC) TestGrid.cpp Grid.cpp -lglut -lGLEW -lGL -lGLU -lX11 -L/usr/lib/nvidia-375 -Wno-write-strings -o ./testGrid

testg: Test.cpp Grid.cpp Grid.h
	$(CC) Test.cpp Grid.cpp -lglut -lGLEW -lGL -lGLU -lX11 -L/usr/lib/nvidia-375 -Wno-write-strings -o ./testg

tar:
	rm -f *~ a3.tar
	tar cf a3.tar *

submit:
	echo "submit README.txt GridPriv.h GridInclude.h Grid.cpp via eClass"
