#!/bin/bash
ssh parallella@192.168.10.101 'mkdir DEMAC_config'
sudo scp ~/CAPSL/DEMAC/DEMAC_set_ip.c parallella@192.168.10.101:~/DEMAC_config/
ssh parallella@192.168.10.101 'cd ~/DEMAC_config/; g++ -o DEMAC_set_ip.o DEMAC_set_ip.c'
ssh parallella@192.168.10.101 'cd ~/DEMAC_config/; sudo ./DEMAC_set_ip.o 01'
ssh parallella@192.168.10.101 'sudo ifdown eth0; ifup eth0'
