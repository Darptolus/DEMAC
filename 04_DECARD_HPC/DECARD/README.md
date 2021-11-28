# DEMAC & DECARD

## DECARD using Caviness


```
vpkg_require cmake/3.11.3 libfabric/1.9.0 openmpi/4.0.2 git/2.29.3 gcc/9.2.0 intel/2020u2
cd 04_DECARD_HPC/build
```

Check packages installed
```
vpkg_history
```
Join workgroup
```
workgroup -g ececis_research
```
```
cmake ../DECARD
cmake --build .
./decard_main.out
```

Debug mode
```
cmake ../DECARD -DDEBUG=0 -DVERBOSE_MODE=1
N_NODES=2; sbatch -N $N_NODES -n $N_NODES decard.qs
```

## DARTS

How to run with darts:

In order to run with DARTS make sure you have DARTS built and install in a given location
To build DARTS do:

```
cmake .. -DHWLOC_ROOT={location_to_hwloc} -DTBB_ROOT={location_to_intel_tbb} -DCMAKE_INSTALL_PREFIX={installation_location} 
```


## Runing on DEMAC

Debug mode
```
cmake ../DECARD -DDEBUG=0 -DVERBOSE_MODE=1 -DEPIPH=1
```

```
mpic++ -I./ -I./assets -std=c++11 -fopenmp -o main.elf main.cpp -I /opt/adapteva/esdk/tools/host/include -L /opt/adapteva/esdk/tools/host/lib -le-hal -le-loader -lpthread
e-gcc -T /opt/adapteva/esdk/bsps/current/internal.ldf -O0 e00_nodeCheck.c -o e00_nodeCheck.elf -le-lib
mpirun -machinefile nodeList -N 1 -H nopa08,nopa10 ./main.elf
mpirun -machinefile nodeList -N 1 -np 2 ./decard_main.out 
```