#!/bin/bash
fileName=main.cpp
nodeList=nodeList
makeFile=Makefile
nodeNum=1
copyHost=demac
execHost=nopa01
#copyHost=nopa01

NFS_DIR=/home/parallella/DEMAC_nfs

usage()
{
  echo "./run_D.sh [-c [copyHost] -f [fileName] -l [nodeList] -cl -cf -mk -n [nodeNum] -r [execHost]]"
}

# COPY (file, makefile) >> RUN
#./run_D.sh -f 00_nodeCheck -n 24 -m Makefile -c demac -r nopa01

# RUN
#./run_D.sh -f 00_nodeCheck -n 24 -r nopa01

while [ "$1" != "" ]; do
    case $1 in
        -c | --copyHost )       shift
                                copyHost=$1
                                ;;
        -f | --file )           shift
                                fileName=$1
                                ;;
        -l | --nodeList )       shift
                                nodeList=$1
                                echo ">>> COPY $nodeList to $copyHost:$NFS_DIR <<<"
                                scp $nodeList $copyHost:$NFS_DIR
                                wait
                                ;;
        -cl | --clean )         
                                echo ">>> CLEAN $fileName from $copyHost:$NFS_DIR <<<"
                                #ssh $copyHost "cd $NFS_DIR; make PROGRAM=$fileName clean"
                                make HEADNODE=$copyHost PROGRAM=$fileName clean
                                wait
                                ;;
        -cf | --copyfile )      
                                echo ">>> COPY $fileName to $copyHost:$NFS_DIR <<<"
                                scp $fileName.cpp $copyHost:$NFS_DIR
                                scp e$fileName.c $copyHost:$NFS_DIR
                                scp -r assets/ $copyHost:$NFS_DIR
                                wait
                                ;;
        -mk | --make )          
                                echo ">>> MAKE $copyHost:$NFS_DIR <<<"
                                #ssh $copyHost "cd $NFS_DIR; make PROGRAM=$fileName"
                                make HEADNODE=$copyHost PROGRAM=$fileName
                                wait
                                ;;
        -n | --nodeNum )        shift
                                nodeNum=$1
                                ;;
        -r | --execHost )       shift
                                execHost=$1
                                echo ">>> RUN $fileName in $execHost <<<"
                                #ssh $execHost "cd $NFS_DIR; make PROGRAM=$fileName N_NODES=$nodeNum run"
                                make HEADNODE=$execHost PROGRAM=$fileName N_NODES=$nodeNum run
                                wait
                                ;;
        -h | --help )           echo ">>> Help"
                                usage
                                exit
                                ;;
        * )                     echo ">>> Type -h for help"
                                usage
                                exit 1
    esac
    shift
done


#make clean; make; make N_NODE=3 run

#for i in 10 20 30 40 50 60 70 80 90 100; do make clean; make N_SIZE=$i N_OPR=480 N_ITER=1; make run; wait; done;
#for i in 1 2 3 4 5 6 7 8 9 10; do make clean; make N_SIZE=100 N_OPR=48*$i N_ITER=1; make run; wait; done;
#for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do make clean; make N_SIZE=50 N_OPR=48 N_ITER=1 N_NODES=$i*2; make run; wait; done;
#for i in 1 2 3 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32 34 36 38 40 42 44 46 48; do make N_NODES=$i run; wait; done;
#for i in 1 3; do make N_NODES=$i run; wait; done;

#make clean; make N_SIZE=100 N_OPR=480 N_ITER=1 N_NODES=48; make run 
