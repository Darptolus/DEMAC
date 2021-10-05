##
## openmpi.sh
##
## Bash script to be sourced by job scripts to do default setup of
## the Open MPI environment variables.
##

if [ -z "$UD_JOB_ENV_OPENMPI_DONE" ]; then
    #
    # Include OpenMP (threads) setup if necessary; also chains-in
    # the common setup:
    #
    . /opt/shared/slurm/templates/libexec/openmp.sh

    #
    # Location the command we'll use to launch the workers:
    #
    OPENMPI_VERSION="<unknown>"
    if [ -n "$UD_USE_SRUN_LAUNCHER" ]; then
        UD_MPIRUN="srun --mpi=pmix --cpu-bind=Cores --export=ALL"
    else
        UD_MPIRUN="$(which mpirun 2>/dev/null)"
        if [ $? -ne 0 ]; then
            UD_MPIRUN="$(which mpiexec 2>/dev/null)"
            if [ $? -ne 0 ]; then
                echo "No mpirun/mpiexec found in ${PATH}"
                exit 1
            fi
        fi
        OPENMPI_VERSION="$($UD_MPIRUN --version | grep '^mpirun ' | sed 's/^[^0-9]+//')"
        if [[ $OPENMPI_VERSION =~ ([0-9]+)\.([0-9]+)\.([0-9]+) ]]; then
            OPENMPI_VERSION_MAJOR=${BASH_REMATCH[1]}
            OPENMPI_VERSION_MINOR=${BASH_REMATCH[2]}
            OPENMPI_VERSION_PATCH=${BASH_REMATCH[3]}
        fi
    fi

    #
    # If a signal handler was registered...
    #
    if [ -n "$UD_HAVE_SIGNAL_HANDLER" ]; then
        # ...arrange for mpirun/orte to wait 15 minutes before forwarding SIGTERM to the
        # worker process(es).  Since our Slurm only waits 5 minutes between SIGTERM and
        # the fatal SIGKILL, Open MPI will never forward SIGTERM to the workers before
        # Slurm has already SIGKILL'ed them:
        export OMPI_MCA_odls_base_sigkill_timeout=3000
    fi

    #
    # Always display the resource mappings:
    #
    export OMPI_MCA_rmaps_base_display_map=true

    #
    # We may eventually need this for nodes with different core counts...
    #
    export OMPI_MCA_orte_hetero_nodes=true

    if [ -z "$UD_DISABLE_CPU_AFFINITY" ]; then
        if [ "$SLURM_JOB_PARTITION" = 'devel' ]; then
            export OMPI_MCA_hwloc_base_binding_policy=hwthread
        else
            export OMPI_MCA_hwloc_base_binding_policy=core
        fi

        #
        # Default distribution is by-core:
        #
        export OMPI_MCA_rmaps_base_mapping_policy=core
        if [ -n "$UD_MPI_RANK_DISTRIB_BY" ]; then
            case "$UD_MPI_RANK_DISTRIB_BY" in
                n|node|N|NODE)
                    export OMPI_MCA_rmaps_base_mapping_policy=node
                    ;;
                s|socket|S|SOCKET)
                    export OMPI_MCA_rmaps_base_mapping_policy=socket
                    ;;
                c|core|C|CORE)
                    export OMPI_MCA_rmaps_base_mapping_policy=core
                    ;;
                *)
                    echo "WARNING:  invalid UD_MPI_RANK_DISTRIB_BY, defaulting to by-core: $UD_MPI_RANK_DISTRIB_BY"
                    ;;
            esac
        fi

        if [ -z "$UD_USE_SRUN_LAUNCHER" -a -n "$OMP_THREAD_LIMIT" ]; then
            if [ $OMP_THREAD_LIMIT -gt 1 ]; then
                if [ $OPENMPI_VERSION_MAJOR -lt 3 ]; then
                    #
                    # The SLURM_TASKS_PER_NODE needs to be multiplied by the
                    # OMP_THREAD_LIMIT for this to work properly:
                    #
                    if [ -z "$UD_QUIET_JOB_SETUP" ]; then
                        echo "-- Altering SLURM_TASKS_PER_NODE to account for multithreading:"
                        echo "--  starting value:   $SLURM_TASKS_PER_NODE"
                    fi
                    export SLURM_TASKS_PER_NODE=$(echo $SLURM_TASKS_PER_NODE | tr ',' "\n" | awk -F\( '{printf("%s%d",comma,$1*'${OMP_THREAD_LIMIT}');comma=",";if($2){printf("(%s",$2)}}')
                    if [ -z "$UD_QUIET_JOB_SETUP" ]; then
                        echo "--  adjusted value:   $SLURM_TASKS_PER_NODE"
                        echo
                    fi
                fi
                case "$OMPI_MCA_rmaps_base_mapping_policy" in
                    node|socket)
                        export OMPI_MCA_rmaps_base_mapping_policy="${OMPI_MCA_rmaps_base_mapping_policy}:pe=$OMP_THREAD_LIMIT"
                        ;;
                    *)
                        export OMPI_MCA_rmaps_base_mapping_policy="slot:pe=$OMP_THREAD_LIMIT"
                        ;;
                esac
                if [ -z "$UD_QUIET_JOB_SETUP" ]; then
                    echo "-- Setting Open MPI hybrid resource mapping policy: $OMPI_MCA_rmaps_base_mapping_policy"
                    echo
                fi
            fi
        fi
    fi

    # Wants IB disabled?
    if [ -n "$UD_DISABLE_IB_INTERFACES" ]; then
        export OMPI_MCA_btl_base_exclude="openib"
        export OMPI_MCA_oob_tcp_if_exclude="ib0"
        export OMPI_MCA_btl_tcp_if_exclude="ib0"
    else
        export OMPI_MCA_btl_base_exclude="tcp"
    fi

    if [ "$SLURM_JOB_PARTITION" = 'devel' ]; then
        export OMPI_MCA_hwloc_base_use_hwthreads_as_cpus=yes
    fi

    if [ -n "$UD_SHOW_MPI_DEBUGGING" ]; then
        export OMPI_MCA_mca_base_verbose=level:100
        export OMPI_MCA_coll_base_verbose=100
        export OMPI_MCA_btl_base_verbose=100
        export OMPI_MCA_mtl_base_verbose=100
        export OMPI_MCA_plm_base_verbose=100
        export OMPI_MCA_ras_base_verbose=100
        if [ -z "$UD_DISABLE_CPU_AFFINITY" ]; then
            export OMPI_MCA_hwloc_base_verbose=100
            export OMPI_MCA_hwloc_base_report_bindings=yes
        fi
    fi

    #
    # Print a summary:
    #
    if [ -z "$UD_QUIET_JOB_SETUP" ]; then
        echo "-- Open MPI job setup complete (on $(hostname -s)):"
        echo "--  mpi job startup      = ${UD_MPIRUN}"
        echo "--  nhosts               = ${SLURM_NNODES}"
        echo "--  nproc                = ${SLURM_NTASKS}"
        echo "--  nproc-per-node       = ${SLURM_TASKS_PER_NODE}"
        echo "--  cpus-per-proc        = ${OMP_THREAD_LIMIT}"
        echo
        echo '-- Open MPI environment flags:'
        printenv | grep ^OMPI_MCA_ | sed 's/^/--  /'
        echo
    fi
    UD_JOB_ENV_OPENMPI_DONE=1
fi
