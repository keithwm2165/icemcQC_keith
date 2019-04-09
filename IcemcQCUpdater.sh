#!/bin/bash
#This is a test script for using github to download or pull icemc
#and then making it. 
# Keith McBride
# 08/18/2017

echo "  "
echo "  "
echo 'starting IcemcQC Updater for pulling from github '
echo "  "
echo "  "

#code for directories
rundir=$(gawk '/rundir/{print $2}' setups/IcemcQCParameters.txt)
updater_dir=$(pwd)
path_dir=$updater_dir/$rundir
echo 'current directory: ' $updater_dir


#commands to make icemc once it is installed

#source environment variables and make file for doing so as well includes modules loaded for using on cluster without breaking. 
#the OSC cluster changed; New shell environment written by Brain; We also need new shell envrionment if run on unity cluster.
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi
module load cmake/3.11.4
module load gnu/7.3.0
export CC=`which gcc`
export CXX=`which g++`
export ROOTSYS=/fs/project/PAS0654/shared_software/anita/owens_pitzer/build/root
eval 'source /fs/project/PAS0654/shared_software/anita/owens_pitzer/build/root/bin/thisroot.sh'
export ICEMC_SRC_DIR=$path_dir/icemc
export ICEMC_BUILD_DIR=$path_dir/icemc
export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}


echo 'export ROOTSYS=/users/PAS0174/osu8620/root-6.08.06' > env_vars_icemcQC.sh
echo 'export LD_INCLUDE_PATH=/users/PAS0174/osu8620/cint/libcint/build/include:$LD_INCLUDE_PATH' >> env_vars_icemcQC.sh
echo 'module load fftw3/3.3.5' >> env_vars_icemcQC.sh
echo 'module load gnu/6.3.0' >> env_vars_icemcQC.sh
echo 'module load python/3.4.2' >> env_vars_icemcQC.sh
echo 'module load cmake/3.7.2' >> env_vars_icemcQC.sh
echo 'export CC=/usr/local/gcc/6.3.0/bin/gcc' >> env_vars_icemcQC.sh
echo 'eval source /users/PAS0174/osu8620/root-6.08.06/builddir/bin/thisroot.sh' >>env_vars_icemcQC.sh
echo 'export ICEMC_SRC_DIR='$path_dir'/icemc' >> env_vars_icemcQC.sh
echo 'export ICEMC_BUILD_DIR='$path_dir'/icemc' >> env_vars_icemcQC.sh
echo 'export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}' >> env_vars_icemcQC.sh


if [ $findstat -eq 1 ]; then
     echo " "
     echo "found icemc"
     echo " "
     cd $path_dir/icemc
     git pull origin master &>> $updater_dir/log_icemc_changes.txt
     sed -i 's/ANITA3_EVENTREADER=1/#ANITA3_EVENTREADER=1/' Makefile
     sed -i 's/ANITA3_EVENTCORRELATOR=1/#ANITA3_EVENTCORRELATOR=1/' Makefile
     make clean
     make &>> log_makeicemc_output.txt
     git log &> log_commit_history.txt
     echo " "
     echo "made icemc"
     echo " "
     cd $updater_dir
elif [ $findstat -eq 0 ]; then
     echo " "
     echo "did not find icemc"
     echo " "
     cd $path_dir
     git clone https://github.com/anitaNeutrino/icemc.git
     cd $path_dir/icemc
     mkdir outputs
     make &> log_makeicemc_output.txt
     git log &> log_commit_history.txt
     echo "made icemc"
     cd $updater_dir
else
     echo "error! abort!"
fi

echo "  "
echo "  "
echo 'ending IcemcQC Updater'
echo "  "
echo "  "

. IcemcQCSetup.sh
