#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling histoMain.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 histoMain.cpp -o histoMain &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their histoMain" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./histoMain < histo_input.txt &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo "Expected Results: " &>> $OUT_FILE
cat histo_output.txt &>> $OUT_FILE

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

echo "Compiling sorterMain.cpp" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -g -std=c++17 sorterMain.cpp -o sorterMain &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Running their sorterMain" &>> $OUT_FILE
timeout 15 valgrind --leak-check=full ./sorterMain < sorter_input.txt &>> $OUT_FILE
EXIT_STATUS=$?
if [ $EXIT_STATUS -eq 124 ]
then
    echo " " &>> $OUT_FILE
    echo "ERROR!! Timeout occured during program execution"  &>> $OUT_FILE
fi
echo "Expected Results: " &>> $OUT_FILE
cat sorter_output.txt &>> $OUT_FILE

echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE

rm -f histoMain sorterMain *.o
