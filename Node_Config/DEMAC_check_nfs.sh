#!/bin/bash
for i in `seq 0 24`; 
do j=$(echo $i | awk '{printf "%02d\n", $0}'); 
	echo ">>>>> Checking nfs NOPA$j <<<<<"; 
	ssh parallella@NOPA$j 'ls -1 DEMAC_nfs | wc -l';
 done
