#!/bin/bash -l

# SBATCH --nodes=2
# SBATCH --ntasks=2
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=1

# SBATCH --mem=8G
# SBATCH --mem-per-cpu=1024M

#SBATCH --job-name=GameOfWorlds

# SBATCH --partition=standard

#SBATCH --partition=ececis_research
# SBATCH --time=0-02:00:00
#SBATCH --time=0-00:10:00

# SBATCH --time-min=0-01:00:00

# SBATCH --output=%x-%j.out
# SBATCH --error=%x-%j.out

# SBATCH --mail-user='my_address@udel.edu'
# SBATCH --mail-type=END,FAIL,TIME_LIMIT_90

#SBATCH --export=NONE

vpkg_require libfabric/1.9.0
vpkg_require gcc/9.2.0
vpkg_require openmpi/4.0.2
vpkg_require intel/2020u2

#UD_QUIET_JOB_SETUP=YES

#UD_USE_SRUN_LAUNCHER=YES

#UD_DISABLE_CPU_AFFINITY=YES

#UD_MPI_RANK_DISTRIB_BY=CORE

#UD_DISABLE_IB_INTERFACES=YES

#UD_SHOW_MPI_DEBUGGING=YES


#job_exit_handler() {
#  # Copy all our output files back to the original job directory:
#  cp * "$SLURM_SUBMIT_DIR"
#
#  # Don't call again on EXIT signal, please:
#  trap - EXIT
#  exit 0
#}
#export UD_JOB_EXIT_FN=job_exit_handler

#export UD_JOB_EXIT_FN_SIGNALS="SIGTERM EXIT"

#
# Do standard Open MPI environment setup (networks, etc.)
#
#. /opt/shared/slurm/templates/libexec/openmpi.sh
. /home/2088/DEMAC/04_DECARD_HPC/caviness_scripts/openmpi_decard.sh
#
# [EDIT] Execute your MPI program
#
#${UD_MPIRUN} ./my_mpi_program arg1 "arg2 has spaces" arg3
#mpi_rc=$?

${UD_MPIRUN} ./../build/apps/GameOfWorlds/GameOfWorlds
mpi_rc=$?

#
# [EDIT] Do any cleanup work here...
#

#
# Be sure to return the mpirun's result code:
#
exit $mpi_rc

