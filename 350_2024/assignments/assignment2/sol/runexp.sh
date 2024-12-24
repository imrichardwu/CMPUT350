#!/bin/bash

N=$1    # runs
C=$2    # unit count (Marines/Tanks)
RP=$3   # red policy
BP=$4   # blue policy
D=0     # delay
#S=-432559583 # seed
S=0 # seed
G=0     # graphics

if [ "$BP" == "" ]
then
  echo "call: runexp N C RP BP"
  exit 1
fi


F=result
rm -f $F

i=0
while [ $i -lt $N ]
do
  # echo $i
  # width height delay seed marines tanks redpol bluepol bounce gfx
  simul 700 700 $D $S $C $C $RP $BP 1 $G | grep over >> $F
  let i++
done

RW=`grep RED $F | wc -l`
DR=`grep draw $F | wc -l`
BW=`grep BLUE $F | wc -l`

RS=`echo "scale=4; ($RW + $DR * 0.5)/$N*100" | bc`
BS=`echo "scale=4; ($BW + $DR * 0.5)/$N*100" | bc`

echo "N " $N " C " $C " RP " $RP " BP " $BP " RW " $RW " - DR " $DR " - BW " $BW " : RED " $RS " - BLUE " $BS
