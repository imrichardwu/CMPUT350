#!/bin/bash

N=1000
R=2  # rounds each policy plays

# global policy score array
declare -ga PS=(0 0 0)

# run one experiment and update policy scores
function exp()
{
  N=$1
  C=$2
  RP=$3
  BP=$4

  echo "exp::: " $N $C $RP $BP

  s=`runexp.sh $N $C $RP $BP`

  echo "FOO " $s

  RS=`echo $s | gawk -e '{ print $19; }'`
  RS=`echo "scale=4; ${PS[$RP]} + $RS/$R/2" | bc`

  echo "RS " $RS
  PS[$RP]=$RS

  BS=`echo $s | gawk -e '{ print $22; }'`
  BS=`echo "scale=4; ${PS[$BP]} + $BS/$R/2" | bc`
  echo "BS " $BS

  PS[$BP]=$BS
}

function expset()
{
  N=$1
  C=$2

  echo "expset::: " $N $C

  PS=(0 0 0)

  echo ${PS[0]} " " ${PS[1]} " " ${PS[2]}

  exp $N $C 0 1
  exp $N $C 0 2
  exp $N $C 1 0
  exp $N $C 1 2
  exp $N $C 2 0
  exp $N $C 2 1
 
  echo ${PS[0]} " " ${PS[1]} " " ${PS[2]}
}

echo "FOOOO"

for i in 20 50 100 300
do
  echo "============ " $i
  expset $N $i
done
