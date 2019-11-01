#!/bin/bash
mount /home/darptolus/CAPSL/DEMAC/nfs

for i in {2..24}
do 
    if [ $i -lt 10 ]
    then
        echo "Mounting NOPA0$i"
        ssh parallella@NOPA0$i 'mount /home/parallella/DEMAC_nfs'
    else
        echo "Mounting NOPA$i"
        ssh parallella@NOPA$i 'mount /home/parallella/DEMAC_nfs'
    fi
done
