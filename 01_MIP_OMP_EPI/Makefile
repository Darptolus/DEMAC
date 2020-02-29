# Makefile DEMAC GeMM
PROGRAM = main.cpp
EPROGRAM = emain.c
PRG_EXE = main.elf
EPRG_EXE = emain.elf
HEADNODE = parallella@NOPA10
GCC = mpic++
EGCC = e-gcc
OPT = 0
CRSC = arm-linux-gnueabi-gcc
NFS_DIR = /home/parallella/DEMAC_nfs
LOCAL_NFS = /home/darptolus/CAPSL/DEMAC/nfs
BUILD = build.sh
N_NODES?= 24

ESDK=/opt/adapteva/esdk
ELIBS=-L ${ESDK}/tools/host/lib
EINCS=-I ${ESDK}/tools/host/include
ELDF=${ESDK}/bsps/current/internal.ldf

${PRG_EXE}: copy_file 
	ssh ${HEADNODE} 'cd ${NFS_DIR}; ${GCC} -std=c++11 -fopenmp -o ${PRG_EXE} ${PROGRAM} ${EINCS} ${ELIBS} -le-hal -le-loader -lpthread'
	ssh ${HEADNODE} 'cd ${NFS_DIR}; ${EGCC} -T ${ELDF} -O${OPT} ${EPROGRAM} -o ${EPRG_EXE} -le-lib'
	
copy_file: ${PROGRAM}
	cp ${PROGRAM} ${LOCAL_NFS}
	cp ${EPROGRAM} ${LOCAL_NFS}
	
run: ${LOCAL_NFS}/${PRG_EXE}
	ssh ${HEADNODE} 'cd ${NFS_DIR}; mpirun -machinefile mpiNodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'
	
init:
	./DEMAC_sshfsEnable.sh

clean:
	rm -f ${LOCAL_NFS}/${PRG_EXE} ${LOCAL_NFS}/${PROGRAM} ${LOCAL_NFS}/${EPROGRAM} ${LOCAL_NFS}/${BUILD} 
