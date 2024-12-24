#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling solve" &>> $OUT_FILE
g++ -Wall -Wextra -O -std=c++17 solve_main.cpp Solve.cpp -o solve &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo "Running their solve on test2x4.mgc" &>> $OUT_FILE
timeout 15 ./solve -c < test2x4.mgc &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Expected Results: " &>> $OUT_FILE
cat expected_output1.txt &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their solve on test2x4.mgs" &>> $OUT_FILE
timeout 15 ./solve -s < test2x4.mgs &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Expected Results: " &>> $OUT_FILE
cat expected_output2.txt &>> $OUT_FILE


echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE


rm -f solve *.o
