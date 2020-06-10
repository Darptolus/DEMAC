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
                                echo ">>> Copying $nodeList to $copyHost:$NFS_DIR <<<"
                                scp $nodeList $copyHost:$NFS_DIR
                                wait
                                ;;
        -cl | --clean )         
                                echo ">>> Clean $copyHost:$NFS_DIR <<<"
                                #ssh $copyHost "cd $NFS_DIR; make PROGRAM=$fileName clean"
                                make HEADNODE=$copyHost PROGRAM=$fileName clean
                                wait
                                ;;
        -cf | --copyfile )      
                                echo ">>> Copying $fileName to $copyHost:$NFS_DIR <<<"
                                scp $fileName.cpp $copyHost:$NFS_DIR
                                wait
                                ;;
        -mk | --make )          
                                echo ">>> Make $copyHost:$NFS_DIR <<<"
                                #ssh $copyHost "cd $NFS_DIR; make PROGRAM=$fileName"
                                make HEADNODE=$copyHost PROGRAM=$fileName
                                wait
                                ;;
        -n | --nodeNum )        shift
                                nodeNum=$1
                                ;;
        -r | --execHost )       shift
                                execHost=$1
                                echo ">>> Running $fileName in $execHost <<<"
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
