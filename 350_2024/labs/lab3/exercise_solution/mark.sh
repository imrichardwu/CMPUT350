#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling AnimalSim.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 AnimalSim.cpp -o animal &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their animal" &>> $OUT_FILE
timeout 1 valgrind --leak-check=full ./animal &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f animal *.o
