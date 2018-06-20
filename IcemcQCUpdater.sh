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

#source environment variables and make file for doing so as well
export ANITA_UTIL_INSTALL_DIR=/home/mcbride.342/anitaBuildTool/Utils
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ANITA_UTIL_INSTALL_DIR/lib
export ICEMC_SRC_DIR=$path_dir/icemc
export ICEMC_BUILD_DIR=$path_dir/icemc
export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}


echo 'export ANITA_UTIL_INSTALL_DIR=/home/mcbride.342/anitaBuildTool/Utils' > env_vars_icemcQC.sh
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ANITA_UTIL_INSTALL_DIR/lib' >> env_vars_icemcQC.sh
echo 'export ICEMC_SRC_DIR='$path_dir'/icemc' >> env_vars_icemcQC.sh
echo 'export ICEMC_BUILD_DIR='$path_dir'/icemc' >> env_vars_icemcQC.sh
echo 'export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}' >> env_vars_icemcQC.sh


if [ $findstat -eq 1 ]; then
     echo " "
     echo "found icemc"
     echo " "
     cd $path_dir/icemc
     git pull &>> $updater_dir/log_icemc_changes.txt
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
