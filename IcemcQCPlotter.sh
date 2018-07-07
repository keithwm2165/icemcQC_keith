#!/bin/bash

##Run by typing . IcemcQCPlotter.sh

####################################################################
##                                                                ##
##  Author: Victoria Niu      July 2018                           ##
##  niu.172@osu.edu                                               ##
##                                                                ##
##  Revised by Keith McBride                                      ##
##                                                                ##
####################################################################

#In the ICEMCQC_rundir directory when QCSubmit finished
echo "  "
echo 'Starting IcemcQCPlotter'
echo "  "

cd .. #go to icemcQC_keith
current_dir=$(pwd) #current_dir is pwd
echo $current_dir
cd ICEMCQC_rundir #go back to ICEMCQC_rundir

RUN_NO=$(gawk 'NR=='11'{print $1}' IcemcQCParameters.txt) #read run number from IcemcQCParameters.txt

#Go to icemc directory
cd icemc
echo 'current directory:'
pwd

#In ICEMCQC_rundir/icemc directory
#delete the past directory and create a new one
rm -rf icefinal_root
mkdir icefinal_root

#In ICEMCQC_rundir/icemc directory
for (( i = 1; i <= $RUN_NO; i++ ))
    do
    echo 'icefinal'$i'.root'
    cp $current_dir/ICEMCQC_rundir/icemc/icemc_outputs$i/icefinal$i.root $current_dir/ICEMCQC_rundir/icemc/icefinal_root
  done

echo "  "

echo "Submitting plots..."
echo "  "

cd ../../
#In icemcQC_keith directory
cp $current_dir/plots/M.read_Primaries $current_dir/ICEMCQC_rundir/icemc/
cp $current_dir/plots/read_Primaries.cc $current_dir/ICEMCQC_rundir/icemc/

#Go back to ICEMCQC_rundir/icemc/
cd ICEMCQC_rundir/icemc/ 

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

echo "current directory: "
pwd

#now put plots into website directory
#cd plots 
#cp *.pdf ./.
#cd  Primariesplots
#cp *.pdf /n/home00/hughes.525/public_html/closed_pages/plots/primaryplots


echo 'QCjobs complete! Good job!!!'
cd $current_dir #back to icemcQC_keith
return
