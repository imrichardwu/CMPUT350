#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling max.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 max.cpp -o max &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their max" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./max &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling max.cpp with our test runner" &>> $OUT_FILE
g++ -O -g -std=c++17 runner_max.cpp -o runner_max &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running max runner" &>> $OUT_FILE
timeout 15 ./runner_max &>> $OUT_FILE
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
echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling point.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 point.cpp -o point &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their point" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./point &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling point.cpp with our test runner" &>> $OUT_FILE
g++ -O -g -std=c++17 runner_point.cpp -o runner_point &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running point runner" &>> $OUT_FILE
timeout 15 ./runner_point &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f max runner_max point runner_point *.o
