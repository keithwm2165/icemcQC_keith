#!/bin/bash
#PBS -l walltime=0:59:00
#PBS -l nodes=1:ppn=1
#PBS -N IcemcQC
#PBS -j oe 
#PBS -m abe
#PBS -M mcbride.342@osu.edu


source $curr_dir/env_vars_icemcQC.sh
cd $LOCALDIR/icemc
date
echo "  "
pwd
echo "  "
echo 'setup file: '$INPUTFILE
echo 'run number: '$RUN_NO
echo 'output file: '$OUTPUTFILE
echo "  "

./icemc -i$INPUTFILE -o/tmp/iceout$RUN_NO -r$RUN_NO

pbsdcp /tmp/iceout$RUN_NO/'*' $OUTPUTFILE
