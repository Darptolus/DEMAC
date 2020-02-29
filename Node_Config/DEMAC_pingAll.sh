#!/bin/bash
date
for i in `seq 0 24`; 
do j=$(echo $i | awk '{printf "%02d\n", $0}'); 
    ping -c 1 NOPA$j > /dev/null
    if [ $? -eq 0 ]; then
        echo "node NOPA$j is up"
    else
        echo "node NOPA$j is down"
    fi
done
