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
RUN_NO=$(gawk 'NR=='11'{print $1}' IcemcQCParameters.txt) #read run number from IcemcQCParameters.txt

cd ..
current_dir=$(pwd) #current_dir=path/icemcQC_keith/
echo $current_dir


cd ICEMCQC_rundir/icemc/
echo 'current directory:'
pwd

#In ICEMCQC_rundir/icemc directory
rm -rf icefinal_root
mkdir icefinal_root
cd icefinal_root

#In ICEMCQC_rundir/icemc/icefinal_root directory
for (( i = 1; i <= $RUN_NO; i++ ))
    do
    echo 'icefinal'$i'.root'
    cp $current_dir/ICEMCQC_rundir/icemc/icemc_outputs$i/icefinal$i.root $current_dir/icefinal_root
  done

pwd

echo "  "

echo "Submitting plots..."
#echo "  "
#echo "Plot number is "$PlotNo
#pwd

cd ..
#Back to ICEMCQC_rundir/icemc/
cp $current_dir/plots/M.read_Primaries $current_dir/ICEMCQC_rundir/icemc/
cp $current_dir/plots/read_Primaries.cc $current_dir/ICEMCQC_rundir/icemc/

#delete the past directory and create a new one
#rm -rf Primariesplots
#make a new directory for storing the plots made at the end of a QC run
#compile read_Primaries.cc file to make plots
make -f M.read_Primaries

for ((i = 1;i <= $RUN_NO; i++ ))
do
current_date=$(date "+%m-%d-%Y")
directory_name=plots.$current_date.$i
echo "directory where plots are stored is: " $current_dir/$directory_name
mkdir $current_dir/$directory_name
./read_Primaries icefinal_root/icefinal$i.root
cp *.pdf $current_dir/$directory_name/.
done

pwd
#now put plots into website directory
#cd plots 
#cp *.pdf ./.
#cd  Primariesplots
#cp *.pdf /n/home00/hughes.525/public_html/closed_pages/plots/primaryplots

echo 'QCjobs complete! Good job!!!'
return
