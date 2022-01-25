Instructions:
**************************************************
*                     DEMAC                      *
**************************************************

run_D can be used to copy and execute programs 

./run_D.sh [-c [copyHost] -f [fileName] -l [nodeList] -m [Makefile] -cl -mk -n [nodeNum] -r [execHost]]

************************
*        Clean         *
************************

Clean filename.cpp & .o from node NOPA##

  ./run_D.sh -c [NOPA##] -f [filename] -cl

  MiniDemac:

    ./run_D.sh -c nopa08 -f 00_nodeCheck -cl

************************
*         Copy         *
************************

Copy filename to node NOPA##

  ./run_D.sh -c [NOPA##] -f [filename] -cf

  MiniDemac:

    ./run_D.sh -c nopa08 -f 00_nodeCheck -cf


Copy Nodelist to node NOPA##

  ./run_D.sh -c [NOPA##] -l [Nodelist]

  MiniDemac:

    ./run_D.sh -c nopa08 -l nodelist


************************
*         Make         *
************************

Make filename.o from filename.cpp in node NOPA##

  ./run_D.sh -c [NOPA##] -f [filename] -mk

  MiniDemac:

    ./run_D.sh -c nopa08 -f 00_nodeCheck -cf

************************
*         Run          *
************************

Run filename.o in node NOPA##

  ./run_D.sh -f [filename] -n [number_of_nodes] -r [NOPA##]

  MiniDemac:

    ./run_D.sh -f 00_nodeCheck -n 4 -r nopa08

************************
* Clean/Copy/Make/Run  *
************************

Copy (file, makefile) >> Clean >> Run

  ./run_D.sh -c [NOPA##] -f [filename] -cl -cf -mk -n 24  -r [NOPA##]

  MiniDemac:

    ./run_D.sh -c nopa08 -f 00_nodeCheck -cl -cf -mk -n 4  -r nopa08

**************************************************
*                    CAVINESS                    *
**************************************************

vpkg_require cmake/3.11.3 libfabric/1.9.0 openmpi/4.0.2 git/2.29.3 gcc/9.2.0 intel/2020u2

cd 00_MPI_OMP/caviness

mpic++ -std=c++11 -fopenmp -o [filename] [filename].cpp

workgroup -g ececis_research

N_NODES=3; N_CORES=2; sbatch -N $N_NODES -n $N_NODES -c $N_CORES [filename].qs

watch -n 2  "squeue | head -n 1; squeue | grep -n 'diegor'"