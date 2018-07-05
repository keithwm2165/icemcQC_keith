#!/bin/bash

##Run by typing . IcemcQCPlotter.sh

####################################################################
##                                                                ##
##  Author: Victoria Niu      July 2018                           ##
##  niu.172@osu.edu                                                ##
##                                                                ##
##                                                                ##
####################################################################

#In the ICEMCQC_rundir directory when QCSubmit finished
echo "  "
echo 'Starting IcemcQCPlotter'
echo "  "

cd icemc
echo 'current directory:'
pwd

#In ICEMCQC_rundir/icemc directory
rm -rf icefinal_root
mkdir icefinal_root
cd icefinal_root

#In ICEMCQC_rundir/icemc/icefinal_root directory
for (( i = 1; i <= 2; i++ ))
    do
    echo 'icefinal'$i'.root'
    cp /users/PAS0654/osu10204/testingQC/icemcQC_keith/ICEMCQC_rundir/icemc/icemc_outputs$i/icefinal$i.root /users/PAS0654/osu10204/testingQC/icemcQC_keith/ICEMCQC_rundir/icemc/icefinal_root
  done

pwd

echo "  "

#echo "Submitting plots..."
#echo "  "
#echo "Plot number is "$PlotNo
#pwd

cd ..
#Back to ICEMCQC_rundir/icemc/
cp /users/PAS0654/osu10204/testingQC/icemcQC_keith/plots/M.read_Primaries /users/PAS0654/osu10204/testingQC/icemcQC_keith/ICEMCQC_rundir/icemc
cp /users/PAS0654/osu10204/testingQC/icemcQC_keith/plots/read_Primaries.cc /users/PAS0654/osu10204/testingQC/icemcQC_keith/ICEMCQC_rundir/icemc

#delete the past directory and create a new one
rm -rf Primariesplots
mkdir Primariesplots

#compile read_Primaries.cc file to make plots
make -f M.read_Primaries

for ((i = 1;i <= 2; i++ ))
do
./read_Primaries icefinal_root/icefinal$i.root
done

pwd
#now put plots into website directory
cd  Primariesplots
cp *.pdf /n/home00/hughes.525/public_html/closed_pages/plots/primaryplots

echo 'QCjobs complete! Good job!!!'
return
