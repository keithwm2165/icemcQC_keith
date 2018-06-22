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
showq >> queue.txt

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

date > submit.txt
echo 'Submitted queue listing:' >> submit.txt
showq >> submit.txt

echo -e "\n" >> $thislog

grep -q "$USER" queue.txt
prevstat=$?
echo 'prevstat: '$prevstat

prevNo=$(gawk "/$USER/{print $1}" queue.txt | wc -l)
echo 'prevNo: '$prevNo
currNo=$(gawk "/$USER/{print $1}" submit.txt | wc -l)
echo 'currNo: '$currNo

high=-1

if [ $prevstat -eq 0 ]; then
    for (( i = 1; i <= $prevNo; i++ )) 
		do
		jobno=$(gawk 'NR=='$i+5'{print $1}' queue.txt)
		if [ $jobno -ge $high ]; then
			high=$jobno
			let atleast=$high+$totruns
		fi
	done
elif [ $prevstat -eq 1 ]; then
	jobno=$(gawk 'NR==6{print $1}' submit.txt)
	echo 'first job no.: '$jobno
	let atleast=$jobno+$currNo-1
else
	echo 'ERROR!!'
	exit 33
fi

echo 'final high prev: '$high
echo 'atleast: '$atleast
check='not yet'
for (( i = 1; i <= $currNo; i++ )) 
	do
	jobno=$(gawk 'NR=='$i+5'{print $1}' submit.txt)
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
		jobno=$(gawk 'NR=='$i+5'{print $1}' submit.txt)
		if [ $jobno -gt $high ]; then
			phrase='jobno_'$jobno'_complete'
			grep -q "$phrase" $thislog
			done1=$?
			if [ $done1 -eq 1 ]; then
				#echo 'job no. '$jobno' is not added to '$thislog
				#echo 'checking showq for job no. '$jobno
				showq > templog.txt
				grep -q "$jobno" templog.txt
				done2=$?
				if [ $done2 -eq 1 ]; then
					echo $phrase >> $thislog
					echo 'job no. '$jobno' has been added to '$thislog
				fi
			fi
		fi
	done
	let iteration=$iteration+1
	complete=$(gawk "/_complete/{print $1}" $thislog | wc -l)
	echo 'Runs completed: '$complete
	echo "   "
done

rm templog.txt

echo "   "
echo "All jobs complete!"
echo "   "

. IcemcQCPlotter.sh

## End program
