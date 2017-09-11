#This is a test script to find if icemc is called.
#and then making it.
# Keith McBride
# 08/18/2017

echo "  "
echo "  "
echo 'starting IcemcQC finder'
echo "  "
echo "  "

rundir=$(gawk '/rundir/{print $2}' setups/IcemcQCParameters.txt)
curr_dir=$(pwd)
find_dir=$curr_dir/$rundir

#find if icemc is installed
find $HOME -type d -name "icemc" | $find_dir/icemc &> log_find_icemc.txt
grep 'No' log_find_icemc.txt
findstat=$?
echo 'the find icemc value is '$findstat

echo ' if 1, then found icemc ' 
echo " "
echo " "
echo ' if 0, then did not find icemc '

. IcemcQCUpdater.sh
