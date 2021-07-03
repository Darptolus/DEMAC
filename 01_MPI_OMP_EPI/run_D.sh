#!/bin/bash
operation=0; # n = none; c = copy; r = run 
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
#make clean; make; make N_NODES=24 run

#make clean; make; for i in `seq 1 15`; do make N_NODES=$i run; wait; done;
#make clean; make;  make N_NODES=1 run; 

# mpic++ -std=c++11 -fopenmp -o 00_nodeCheck 00_nodeCheck.cpp -I /opt/adapteva/esdk/tools/host/include -L /opt/adapteva/esdk/tools/host/lib -le-hal -le-loader -lpthread

# e-gcc -T /opt/adapteva/esdk/bsps/current/internal.ldf -O0 e00_nodeCheck.c -o e00_nodeCheck.elf -le-lib

# mpirun -machinefile nodeList -N 1 -np 3 ./00_nodeCheck