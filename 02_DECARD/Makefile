# Makefile DEMAC
PROGRAM = main.cpp
PRG_EXE = main.o
GCC = mpic++
CRSC = arm-linux-gnueabi-gcc
NFS_DIR = /home/parallella/DEMAC_nfs
LOCAL_NSF = /home/darptolus/CAPSL/DEMAC/nfs

C_INCLUDES = -I./ -I./assets
# CPY_FILES = node.h opr_code.h

N_NODES?= 24
N_SIZE?= 10
N_OPR?= 48
N_ITER?= 1

${PRG_EXE}: copy_file 
	ssh parallella@NOPA01 'cd ${NFS_DIR}; ${GCC} ${C_INCLUDES} -std=c++11 -fopenmp -lpthread -o ${PRG_EXE} -DN_SIZE=${N_SIZE} -DN_OPR=${N_OPR} -DN_ITER=${N_ITER} ${PROGRAM}'
	
copy_file: ${PROGRAM}
	cp ${PROGRAM} ${LOCAL_NSF}
	### for i in ${CPY_FILES}; do cp $$i ${LOCAL_NSF}; done
	cp -r assets/ ${LOCAL_NSF}
	
run: ${LOCAL_NSF}/${PRG_EXE}
	ssh parallella@NOPA01 'cd ${NFS_DIR}; mpirun -machinefile mpiNodeList -N 1 -np ${N_NODES} ./${PRG_EXE}'
	
init:
	./DEMAC_sshfsEnable.sh

clean:
	rm -f ${LOCAL_NSF}/${PRG_EXE} ${LOCAL_NSF}/${PROGRAM} ${LOCAL_NSF}/M_*
