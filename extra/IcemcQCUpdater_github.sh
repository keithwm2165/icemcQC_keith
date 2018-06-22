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
curr_dir=$(pwd)
path_dir=$curr_dir/$rundir
echo 'current directory: ' $curr_dir


#commands to make icemc once it is installed

#source environment variables
source $HOME/env_anita_test2.sh

if [ $findstat -eq 1 ]; then
     echo " "
     echo "found icemc"
     echo " "
     cd $path_dir/icemc
     git pull &>> $curr_dir/log_icemc_changes.txt
     make clean
     make &>> log_make_out.txt
     git log &> log_commit_history.txt
     echo " "
     echo "made icemc"
     echo " "
     cd $curr_dir
elif [ $findstat -eq 0 ]; then
     echo " "
     echo "did not find icemc"
     echo " "
     cd $path_dir
     git clone https://github.com/anitaNeutrino/icemc.git
     cd $path_dir/icemc
     make &> log_make_out.txt
     git log &> log_commit_history.txt
     echo "made icemc"
     cd $curr_dir
else
     echo "error! abort!"
fi

echo "  "
echo "  "
echo 'ending IcemcQC Updater'
echo "  "
echo "  "

. IcemcQCSetup.sh
