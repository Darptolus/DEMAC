#!/bin/bash
mount /home/darptolus/CAPSL/DEMAC/nfs;
ssh parallella@NOPA00 'mount /home/parallella/DEMAC_nfs';

for i in `seq 2 24`; 
do j=$(echo $i | awk '{printf "%02d\n", $0}'); 
	echo ">>>>> Mountinng nfs NOPA$j <<<<<"; 
	ssh parallella@NOPA$j 'mount /home/parallella/DEMAC_nfs';
 done
 