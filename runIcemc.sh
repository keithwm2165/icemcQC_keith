#!/bin/bash
#PBS -l walltime=0:59:00
#PBS -l nodes=1:ppn=1
#PBS -l mem=1GB
#PBS -N IcemcQC
#PBS -j oe 
#PBS -A PAS0654

cd $SRC_DIR

source env_vars.sh
echo $ICEMC_SRC_DIR
echo ' '
cd $LOCALDIR/icemc

date
echo 
pwd
echo "  "
echo 'setup file: '$INPUTFILE
echo 'run number: '$RUN_NO
echo 'output file: '$OUTPUTFILE
echo "  "

./icemc -i $LOCALDIR/$INPUTFILE -o $OUTPUTFILE -r $RUN_NO
pbsdcp /tmp/icefinal$RUN_NO/'*' $OUTPUTFILE
