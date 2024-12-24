#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


make clean
echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling testg" &>> $OUT_FILE
make testg &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling testGrid" &>> $OUT_FILE
make testGrid &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "Runnin testg" &>> $OUT_FILE
timeout 10 valgrind --leak-check=full ./testg < test.input &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "Timeout ..."  &>> $OUT_FILE
fi
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
