# Makefile DEMAC
HEADNODE? = parallella@NOPA01
PROGRAM? = main
PRG_EXE = ${PROGRAM}.o
PRG = ${PROGRAM}.cpp
GCC = mpic++
CRSC = arm-linux-gnueabi-gcc
NFS_DIR = /home/parallella/DEMAC_nfs
N_NODES?= 24

${PRG_EXE}:
	ssh ${HEADNODE} 'cd ${NFS_DIR}; ${GCC} -std=c++11 -fopenmp -o ${PRG_EXE} ${PRG}'
	
run: 
#	ssh ${HEADNODE} 'cd ${NFS_DIR}; mpirun --mca btl self,tcp --mca btl_base_verbose 30 -machinefile nodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'
	ssh ${HEADNODE} 'cd ${NFS_DIR}; mpirun -machinefile nodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'

clean:
	ssh ${HEADNODE} 'cd ${NFS_DIR}; rm -f ${PRG_EXE} ${PRG}'
