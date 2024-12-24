#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling their test code" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 Matrix.cpp matrixTest.cpp -o matrix &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their matrix" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./matrix &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling our test code" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 Matrix.cpp matrixTestRunner.cpp -o matrix &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their matrix" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./matrix &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f matrix *.o
