#!/bin/bash
#PBS -l walltime=0:59:00
#PBS -l nodes=1:ppn=1
#PBS -N IcemcQC
#PBS -j oe 
#PBS -m abe

cd $SRC_DIR

source env_vars_icemcQC.sh
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

./icemc -i $LOCALDIR/$INPUTFILE -o outputs -r $RUN_NO &> log_QC$RUN_NO.txt

pbsdcp /tmp/iceout$RUN_NO/'*' $OUTPUTFILE
