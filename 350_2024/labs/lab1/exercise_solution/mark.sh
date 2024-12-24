#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling p1.cpp" &>> $OUT_FILE
g++ -g -Wall -Wextra -Wconversion -Wsign-conversion p1.cpp -o p1 &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their p1" &>> $OUT_FILE
timeout 15  valgrind --leak-check=full ./p1 &>> $OUT_FILE

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling p2.cpp" &>> $OUT_FILE
g++ -g -Wall -Wextra -Wconversion -Wsign-conversion p2.cpp -o p2 &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their p2" &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 1: Expecting" &>> $OUT_FILE
cat output_test1.txt &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 1: Received" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./p2 < input_test1.txt &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
if [ $EXIT_STATUS -ne 0 ]; then
    echo "Incorrect return value"
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Test 2: Expecting" &>> $OUT_FILE
cat output_test2.txt &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 2: Received" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./p2 < input_test2.txt &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
if [ $EXIT_STATUS -ne 1 ]; then
    echo "Incorrect return value"
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Test 3: Expecting" &>> $OUT_FILE
cat output_test3.txt &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 3: Received" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./p2 < input_test3.txt &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
if [ $EXIT_STATUS -ne 1 ]; then
    echo "Incorrect return value"
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f p1 p2 *.o
