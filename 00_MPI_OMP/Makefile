# Makefile DEMAC GeMM
PROGRAM = main.cpp
PRG_EXE = main.o
GCC = mpic++
CRSC = arm-linux-gnueabi-gcc
NFS_DIR = /home/parallella/DEMAC_nfs
LOCAL_NFS = /home/darptolus/CAPSL/DEMAC/nfs

N_NODES?= 24

${PRG_EXE}: copy_file 
	ssh parallella@NOPA01 'cd ${NFS_DIR}; ${GCC} -std=c++11 -fopenmp -o ${PRG_EXE} ${PROGRAM}'
	
copy_file: ${PROGRAM}
	#scp ${PROGRAM} parallella@NOPA01:${NFS_DIR}/${PROGRAM}
	cp ${PROGRAM} ${LOCAL_NFS}/${PROGRAM}
	
run: ${LOCAL_NFS}/${PRG_EXE}
	ssh parallella@NOPA01 'cd ${NFS_DIR}; mpirun -machinefile mpiNodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'
	
init:
	./DEMAC_sshfsEnable.sh

clean:
	rm -f ${LOCAL_NFS}/${PRG_EXE} ${LOCAL_NFS}/${PROGRAM} ${LOCAL_NFS}/M_*
