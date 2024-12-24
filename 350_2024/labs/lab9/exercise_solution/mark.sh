#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling ttt2.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -O -g -std=c++17 ttt2.cpp -o ttt2 &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their ttt2" &>> $OUT_FILE
timeout 15  ./ttt2 &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo "Expected Results: " &>> $OUT_FILE
cat expected_results.txt &>> $OUT_FILE

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling our runner" &>> $OUT_FILE
g++ -g -std=c++17 runner.cpp -o runner &>> $OUT_FILE
timeout 15  ./runner &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f ttt2 runner *.o
