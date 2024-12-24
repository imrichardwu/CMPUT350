#!/bin/bash

OUT_FILE=out.txt

# Remove 
if [ -f $OUT_FILE ]; then
    rm -r $OUT_FILE
fi


echo "----------------------------------------" &>> $OUT_FILE
echo "Compiling" &>> $OUT_FILE
make &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo " " &>> $OUT_FILE
echo "----------------------------------------" &>> $OUT_FILE
