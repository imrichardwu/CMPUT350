#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Question 1:" &>> $OUT_FILE
echo "Compiling and checking for warnings:" &>> $OUT_FILE
gcc -g -Wall -Wextra -Wconversion -Wsign-conversion p1.c p1_marker.c -o p1_marker &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 1: Zero sized array, expecting assertion" &>> $OUT_FILE
(./p1_marker 0 || false) &>> $OUT_FILE || true
echo " " &>> $OUT_FILE
echo "Test 2: Non-zero sized array" &>> $OUT_FILE
(valgrind --leak-check=full ./p1_marker 10 || false) &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE

echo "----------------------------------------" &>> $OUT_FILE
echo "Question 2:" &>> $OUT_FILE
echo "Compiling and checking for warnings:" &>> $OUT_FILE
gcc -g -Wall -Wextra -Wconversion -Wsign-conversion p2.c p2_marker.c -o p2_marker &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "Test 1: Zero sized linked list, expecting assertion" &>> $OUT_FILE
(./p2_marker 0 || false) &>> $OUT_FILE || true
echo " " &>> $OUT_FILE
echo "Test 2: Non-zero sized linked list" &>> $OUT_FILE
(valgrind --leak-check=full ./p2_marker 10 || false) &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE

rm p1_marker p2_marker
