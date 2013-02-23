#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./runMultible.sh <# of instances> <client>"
    exit 1
fi
for (( i=0; i < $1; i++ ));
do
    xterm -T "Terminal $i" -e ./$2 $i &
done

