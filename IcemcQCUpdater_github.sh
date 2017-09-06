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
var_path=`pwd`
echo 'current directory: ' $var_path


#commands to make icemc once it is installed

#source environment variables
source $HOME/env_anita.sh

#find if icemc is installed
#find $HOME -type d -name "icemc" | $HOME/icemc &> log_find_icemc.txt
#grep 'No' log_find_icemc.txt
#findstat=$?
#echo 'the find icemc value is '$findstat

if [ $findstat -eq 1 ]; then
     echo " "
     echo "found icemc"
     echo " "
     cd $HOME/ICEMCQC/icemc/
     git pull &>> $var_path/log_icemc_changes.txt
     make clean
     make &>> log_make_out.txt
     git log &> log_commit_history.txt
     echo " "
     echo "made icemc"
     echo " "
elif [ $findstat -eq 0 ]; then
     echo " "
     echo "did not find icemc"
     echo " "
     cd $HOME/ICEMCQC/
     git clone https://github.com/anitaNeutrino/icemc.git
     cd $HOME/ICEMCQC/icemc/
     make &> log_make_out.txt
     git log &> log_commit_history.txt
     echo "made icemc"
else
     echo "error! abort!"
fi

echo "  "
echo "  "
echo 'ending IcemcQC Updater'
echo "  "
echo "  "

. IcemcQCSetup.sh
