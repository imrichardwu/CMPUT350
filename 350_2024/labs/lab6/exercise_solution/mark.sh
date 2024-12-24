#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling mainStack.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 mainStack.cpp -o mainStack &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their mainStack" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./mainStack &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Compiling mainStack.cpp with our test runner" &>> $OUT_FILE
g++ -O -g -std=c++17 runner_stack.cpp -o runner_stack &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running runner_stack" &>> $OUT_FILE
timeout 15 ./runner_stack &>> $OUT_FILE
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
echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling mainSum.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 mainSum.cpp -o mainSum &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their mainSum" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./mainSum &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling mainSum.cpp with our test runner" &>> $OUT_FILE
g++ -O -g -std=c++17 runner_sum.cpp -o runner_sum &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running runner_sum" &>> $OUT_FILE
timeout 15 ./runner_sum &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE


rm -f mainStack runner_stack mainSum runner_sum *.o
