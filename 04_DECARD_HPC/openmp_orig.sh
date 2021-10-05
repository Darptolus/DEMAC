##
## openmp.sh
##
## Bash script to be sourced by job scripts to do default setup of
## the OpenMP environment variables.
##

if [ -z "$UD_JOB_ENV_OPENMP_DONE" ]; then
  #
  # Do the general setup stuff:
  #
  . /opt/shared/slurm/templates/libexec/common.sh

  if [ ${SLURM_CPUS_PER_TASK:-1} -gt 1 ]; then
    #
    # Setup the OpenMP max thread count in case the --cpus-per-task option was used
    # above.
    #
    export OMP_THREAD_LIMIT=${SLURM_CPUS_PER_TASK:-1}

    #
    # Setup MKL aliases, too:
    #
    if [ -z "$MKL_NUM_THREADS" ]; then
      export MKL_NUM_THREADS=${OMP_THREAD_LIMIT}
    fi
    if [ -z "$MKL_DYNAMIC" ]; then
      export MKL_DYNAMIC=true
    fi

    #
    # Bind OMP threads to processors:
    #
    if [ -z "$OMP_PROC_BIND" ]; then
      export OMP_PROC_BIND=true
      if [ -z "$OMP_PLACES" ]; then
        #
        # If using the 'devel' partition then bind to hyperthreads:
        #
        if [ "$SLURM_JOB_PARTITION" = 'devel' ]; then
          export OMP_PLACES=threads
        else
          export OMP_PLACES=cores
        fi
      fi
      #
      # PGI OpenMP runtime requires explicit association of OpenMP threads to
      # processor indices; if we don't set MP_BIND to "true" we aren't forcing
      # the behavior, though.
      #
      export MP_BLIST="$(grep '^Cpus_allowed_list:' /proc/$$/status | perl -e '$o=""; while (<>) { $l=$_; if ( $l =~ /^\D+(.*)$/ ) { $l = $1; } while ( $l =~ /^((\d+)(-(\d+))?)(,(.*))?$/ ) { $i = int($2); $iMax = $4; $l = $6; if ( ! $iMax ) { $iMax = $i; } else { $iMax = int($iMax); } while ( $i <= $iMax ) { if ( $o ne "" ) { $o .= ","; } $o .= $i; $i++; } } } print $o;')"
    fi

    if [ -z "$UD_QUIET_JOB_SETUP" -a $OMP_THREAD_LIMIT -gt 1 ]; then
      #
      # Print a summary:
      #
      echo "-- OpenMP job setup complete:"
      [ -n "$OMP_THREAD_LIMIT" ] && echo "--  OMP_THREAD_LIMIT     = ${OMP_THREAD_LIMIT}"
      [ -n "$OMP_PROC_BIND" ] &&    echo "--  OMP_PROC_BIND        = ${OMP_PROC_BIND}"
      [ -n "$OMP_PLACES" ] &&       echo "--  OMP_PLACES           = ${OMP_PLACES}"
      [ -n "$MKL_NUM_THREADS" ] &&  echo "--  MKL_NUM_THREADS      = ${MKL_NUM_THREADS}"
      [ -n "$MKL_DYNAMIC" ] &&      echo "--  MKL_DYNAMIC          = ${MKL_DYNAMIC}"
      [ -n "$MP_BLIST" ] &&         echo "--  MP_BLIST             = ${MP_BLIST}"
      echo
    fi
  fi
  UD_JOB_ENV_OPENMP_DONE=1
fi
