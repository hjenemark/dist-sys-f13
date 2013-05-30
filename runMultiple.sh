#!/bin/bash

if [ $# -eq 0 ]
then
    echo "Usage: ./runMultible.sh <client> <ip-start> <ip-end>"
    exit 1
fi
for (( i=$2; i < $3; i++ )) ;
do
    sudo ifconfig eth0:$i 10.10.10.$i netmask 255.255.255.0
done

for (( i=$2; i < $3; i++ ));
do
    screen -d -m terminator -T "Terminal $i" -x ./$1 v4 10.10.10.$i
done

