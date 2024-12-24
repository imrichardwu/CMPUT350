#!/bin/bash

OUT_FILE=out.txt

if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


# CPoint
# --------------------
echo "Compiling CPoint" &>> $OUT_FILE
gcc -Wall -Wextra -O CPoint.c -o CPoint &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo " " &>> $OUT_FILE


# String
# --------------------
echo "Compiling String" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -std=c++17 TestString.cpp String.cpp -g -o mainstring &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Results of string" &>> $OUT_FILE
valgrind --leak-check=full ./mainstring &>> $OUT_FILE


echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo " " &>> $OUT_FILE


# Set
# --------------------
echo "Compiling Set" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -O -std=c++17 mainSet.cpp Set.cpp -g -o mainset &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Results of set" &>> $OUT_FILE
valgrind --leak-check=full ./mainset &>> $OUT_FILE


echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo " " &>> $OUT_FILE



# Hexagons
# --------------------
echo "Compiling Hexagons" &>> $OUT_FILE
g++ -Wall -Wextra -Wconversion -Wsign-conversion -std=c++17 testdriver.cpp -g -o mainhexagons &>> $OUT_FILE
echo " " &>> $OUT_FILE

echo "Results of Hexagons" &>> $OUT_FILE
./mainhexagons &>> $OUT_FILE


echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
echo " " &>> $OUT_FILE

rm -f CPoint mainset mainstring mainhexagons