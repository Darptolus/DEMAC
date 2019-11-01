#!/bin/bash
#DEMAC Node IP configuration 
#arguments = current_IP 
ssh parallella@$1 'mkdir DEMAC_config'
scp ~/DEMAC/Node_Config/DEMAC_set_ip.c parallella@$1:~/DEMAC_config/
ssh parallella@$1 'cd ~/DEMAC_config/; g++ -o DEMAC_set_ip.o DEMAC_set_ip.c'
ssh parallella@$1 'cd ~/DEMAC_config/; sudo ./DEMAC_set_ip.o 09' ## change new IP to be input to bash file
ssh parallella@$1 'sudo ifdown eth0; ifup eth0'
