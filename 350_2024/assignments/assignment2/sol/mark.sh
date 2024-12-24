#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling their code" &>> $OUT_FILE
make &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling with our test runner" &>> $OUT_FILE
g++ -O -g Marine.cpp Tank.cpp World.cpp World2.cpp runner_world.cpp -o runner_world &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running runner_world" &>> $OUT_FILE
timeout 15 ./runner_world &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f runner_world
