#!/bin/bash
make clean; make; for i in `seq 1 15`; do make N_NODES=$i run; wait; done;
#make clean; make;  make N_NODES=1 run; 