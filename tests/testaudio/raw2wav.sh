#!/bin/bash -eu

n=`printf response/s%07d $1`
FILES=`ls $n/*.raw`

for f in $FILES
do
    echo "Processing $f"
    bn=${f%.*}.wav
    sox -r 48k -e signed -b 16 -c 1 $f $bn
done
