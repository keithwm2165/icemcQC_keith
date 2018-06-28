#!/bin/bash

##Run by typing . IcemcQCSubmit.sh

####################################################################
##                                                                ##
##  Author: Khalida Hendricks July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##                                                                ##
##  IcemcQCSubmit runs multiple iterations of Icemc given     ##  
##  various setup file parameters, then makes plots and takes     ##
##  statistics on the results in order to compare Icemc          ## 
##  versions.                                                     ##
##  IcemcQC is run automatically by IcemcQCTimer (or            ##
##  IcemcVersion) but also can be run manually from any          ##
##  Icemc directory on any version of Icemc.                    ##                                 ##
##                                                                ##
####################################################################

echo "  "
echo 'starting IcemcQCSubmit.sh'
echo "  "
pwd

echo 'username: '$USER

tgtdir=$(gawk '/tgtdir/{print $2}' IcemcQCParameters.txt)
echo $tgtdir
thislog=thisQClog.txt
dwell=$(gawk '/dwell/{print $2}' IcemcQCParameters.txt)

echo 'input log file: ' $thislog
echo "  "
echo "  "
#echo "RUNNING MAKE..."
echo "  "
echo "  "
	
rundir=$(gawk '/rundir/{print $2}' IcemcQCParameters.txt)
#make clean
#make

echo "   "
echo "   "
echo "   "

localdir=`pwd`
echo 'current dir : '$localdir
echo " "
echo 'IcemcQC run from '$localdir > $thislog
date >> $thislog
echo -e "\n" >> $thislog

ls | egrep -q 'Icemc$'
there=$?

if [ $there -eq 0 ]; then
    echo 'Icemc executable present? : YES'
	    echo 'Icemc executable present? : YES' >> $thislog
elif [ $there -eq 1 ]; then
    echo 'Icemc executable present? : NO - ERROR'
	echo 'Icemc executable present? : NO - ERROR' >> $thislog
else 
    echo 'Icemc executable present? : ERROR'
	echo 'Icemc executable present? : ERROR' >> $thislog
fi

echo -e "\n" >> $thislog
echo "  "
echo "  "

SetupNo=$(gawk '/.txt/' IcemcQCParameters.txt | wc -l)
echo 'Number of setup files: '$SetupNo
echo 'Number of setup files: '$SetupNo >> $thislog
echo "Setup files used:" >> $thislog
gawk '/.txt/{print $1, $2}' IcemcQCParameters.txt >> $thislog

echo "   "

#Find the number of runs total from runs for each input 
totruns=0
for ((j = 1;j <= $SetupNo; j++ ))
	do 
	setup[$j]=$(gawk 'NR=='$j+9'{print $2}' $thislog)
	echo 'setup['$j'] = '${setup[$j]}
	numruns[$j]=$(gawk 'NR=='$j+9'{print $1}' $thislog)
	echo 'numruns['$j'] = '${numruns[$j]}
	let totruns=$totruns+${numruns[$j]}
done

echo 'total # of runs: '$totruns
echo "   "
echo "   "
echo "   "

date > queue.txt
echo 'Initial queue listing:' >> queue.txt
qstat -u $USER >> queue.txt

#Submit all the jobs
totrun=1
for (( i = 1; i <= $SetupNo; i++ ))
    do
#Keith Edits
	for (( j = 1; j <= ${numruns[$i]}; j++ ))
		do
		echo 'totrun value:'$totrun
		qsub runIcemc.sh -v SRC_DIR=$updater_dir,INPUTFILE=${setup[$i]},RUN_NO=$totrun,OUTPUTFILE=$tgtdir,LOCALDIR=$localdir
		#qsub runIcemc.sh -v INPUTFILE=inputsworking.txt,RUN_NO=$totrun,OUTPUTFILE=outputs,LOCALDIR=$localdir
	let totrun=$totrun+1
	done
done

echo "   "
echo 'SHOW QUEUE:'
echo "   "
qstat -u $USER
echo "   "
echo "   "
echo "   "
########################################################################################################################################
#Stuff below needs to change to be useful on OSC cluster. This was written for another cluster and does not work correctly on OSC. #
########################################################################################################################################
## comments below are useful for debugging with Victoria's account on OSC with 2 runs submitted only. 


date > submit.txt
echo 'Submitted queue listing:' >> submit.txt
qstat -u $USER >> submit.txt
#return #see below for the +5 in NR== for gawk commands. need to change the 5 to get the record returned to correspond to the first job (maybe bigger or smaller than 5).
# uncomment the above line of "return" and check the submit.txt file to see what it contains.Should be 2 jobs listed.  If the jobs arent showing up then they werent submitted properly.  

echo -e "\n" >> $thislog

grep -q "$USER" queue.txt
prevstat=$?
echo 'prevstat: '$prevstat
#return
# uncomment the above line of return to kill the code here and make sure prevstat is 0 since it should have been successful? prestat should be 0 or 1. 0 means found $USER in queue.txt. 1 means did not find $USER in queue.txt
#$? gives the "exit status" of the most recent command, in this case the grep -q command. -q is to surpress the output of grep .

prevNo=$(gawk "/$USER/{print $1}" queue.txt | wc -l)
echo 'prevNo: '$prevNo
#Here, gawk will return only those records or rows of queue.txt that contain the thin in between "/ /". So, "/$USER/{print $1}" prints the first stuff before white space (print $1) and then only shows the (records) ones that have $USER in them. 
currNo=$(gawk "/$USER/{print $1}" submit.txt | wc -l)
echo 'currNo: '$currNo

#prevno and currno will be returning, effectively, how many times $USER showed up in the queue before submitting jobs (prevno) and after submitting jobs (currno).

high=-1

if [ $prevstat -eq 0 ]; then # (if it has found $USER in the queue.txt file) (if Victoria is only running QC and no other codes that will submit jobs, then we dont need to worry about this). 
    for (( i = 1; i <= $prevNo; i++ )) 
		do
		jobno=$(gawk 'NR=='$i+5'{print $1}' queue.txt) #gawk ('NR==6' '{print $1}') file.txt will  do the following. 1) print $1 returns the 1st column of file.txt 2) NR==6 returns only the record that corresponds to the 6th thing returned by print $1
		if [ $jobno -ge $high ]; then
			high=$jobno
			let atleast=$high+$totruns
		fi
	done
elif [ $prevstat -eq 1 ]; then # (if it has not found $USER in the queue.txt file). (this should be whats happening to Victoria) However, the stuff here could be a problem since there might not be 6 records if we have changed the number of runs to 2!. So we need to determine how to get jobno to be the actual first job.
	jobno=$(gawk 'BEGIN{ FS="." }NR==8{print $1}' submit.txt)
	echo 'first job no.: '$jobno
	let atleast=$jobno+$currNo-1
else #really shouldnt happen! (any other return value that not 1 or 0 is bad and means something else went wrong). 
	echo 'ERROR!!'
	exit 33
fi

echo 'final high prev: '$high
echo 'atleast: '$atleast
check='not yet'
for (( i = 1; i <= $currNo; i++ ))  #might need to change the 5 here. This a comprehensive check of whether the queue.txt jobs and the submit jobs match up (the rest of the code is accessing the right jobs).
	do
	jobno=$(gawk 'NR=='$i+5'{print $1}' submit.txt)
	echo $jobno
	let jobno
	if [ $jobno -ge $atleast ]; then
		check='OK'
	fi
done

echo 'At least '$totruns' jobs submitted? : '$check
echo 'Total runs submitted: '$totruns

##
## here comes the hard part
##

echo "   "
echo "   "
echo "   "
echo "Checking for job completion..."
echo "   "
echo "   "
echo "   "

iteration=1
complete=0
echo 'Temp Log' > templog.txt

while (($complete < $totruns))
	do
	sleep $dwell
	date
	echo 'Check no. '$iteration
	for (( i = 1; i <= $currNo; i++ )) 
		do
#this line needs to change
		jobno=$(gawk 'NR=='$i+5'{print $1}' submit.txt) #the 5 is here because they knew that submit.txt (being from qstat on the other cluster) would give the first job submitted identification.
		if [ $jobno -gt $high ]; then
			phrase='jobno_'$jobno'_complete'
                        echo $phrase
			grep -q "$phrase" $thislog
			done1=$? #1 or 0
			if [ $done1 -eq 1 ]; then
				echo 'job no. '$jobno' is not added to '$thislog
				echo 'checking qstat for job no. '$jobno
					qstat -u $USER > templog.txt
				grep -q "$jobno" templog.txt
				done2=$? #1 or 0
				if [ $done2 -eq 1 ]; then
					echo $phrase >> $thislog
					echo 'job no. '$jobno' has been added to '$thislog
				fi
			fi
		fi
	done
	let iteration=$iteration+1

#this line needs to change
	complete=$(gawk "/_complete/{print $1}" $thislog | wc -l) #thislog is thisQClog.txt


	echo 'Runs completed: '$complete
	echo "   "
done

rm templog.txt

echo "   "
echo "All jobs complete!"
echo "   "

. IcemcQCPlotter.sh

## End program
