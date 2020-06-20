# Makefile DEMAC
HEADNODE? = parallella@NOPA01
PROGRAM? = main
EPROGRAM = e${PROGRAM}
PRG = ${PROGRAM}.cpp
EPRG = ${EPROGRAM}.c
PRG_EXE = ${PROGRAM}.elf
EPRG_EXE = ${EPROGRAM}.elf
GCC = mpic++
EGCC = e-gcc
OPT = 0
CRSC = arm-linux-gnueabi-gcc
NFS_DIR = /home/parallella/DEMAC_nfs
N_NODES?= 24

ESDK=/opt/adapteva/esdk
ELIBS=-L ${ESDK}/tools/host/lib
EINCS=-I ${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/internal.ldf

${PRG_EXE}:  
	ssh ${HEADNODE} 'cd ${NFS_DIR}; ${GCC} -std=c++11 -fopenmp -o ${PRG_EXE} ${PRG} ${EINCS} ${ELIBS} -le-hal -le-loader -lpthread'
	ssh ${HEADNODE} 'cd ${NFS_DIR}; ${EGCC} -T ${ELDF} -O${OPT} ${EPRG} -o ${EPRG_EXE} -le-lib'
	
run:
	ssh ${HEADNODE} 'cd ${NFS_DIR}; mpirun -machinefile nodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'

clean:
	ssh ${HEADNODE} 'cd ${NFS_DIR}; rm -f ${PRG_EXE} ${PRG} ${EPRG_EXE} ${EPRG}'
