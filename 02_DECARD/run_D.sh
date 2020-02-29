#!/bin/bash
make clean; make; make N_NODE=3 run

#for i in 10 20 30 40 50 60 70 80 90 100; do make clean; make N_SIZE=$i N_OPR=480 N_ITER=1; make run; wait; done;
#for i in 1 2 3 4 5 6 7 8 9 10; do make clean; make N_SIZE=100 N_OPR=48*$i N_ITER=1; make run; wait; done;
#for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do make clean; make N_SIZE=50 N_OPR=48 N_ITER=1 N_NODES=$i*2; make run; wait; done;
#for i in 1 2 3 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48; do make N_NODES=$i run; wait; done;
#for i in 1 3; do make N_NODES=$i run; wait; done;

#make clean; make N_SIZE=100 N_OPR=480 N_ITER=1 N_NODES=48; make run 
