#This is Keith McBride editing the code for integrating to Github. date:08/18/2017

#!/bin/bash

##Run by typing . IcemcQC.sh

####################################################################
##                                                                ##
##  Author: Khalida Hendricks July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##  IcemcQC.sh starts running IcemcQC program        		  ##
##								  ##
####################################################################

echo "  "
echo "  "
echo "  "
echo "  "
echo 'Starting IcemcQC'
echo "  "
echo "  "
echo 'Author: Khalida Hendricks'
echo 'hendricks.189@osu.edu'
echo 'July 2014'
echo "  "
echo 'Edits:Keith McBride'
echo 'August 2017'
echo "  "
echo 'Edits:Victoria Niu'
echo 'Jun 2018'
echo "  "
echo "  "
echo "  "

##prep log files, run directory

mkdir -p logs
date >> logs/QClog.txt
date > logs/thisrun.txt

QCdir=`pwd`
echo 'current directory: '$QCdir
echo 'QC run directory: '$QCdir' (QCdir)' >> logs/QClog.txt
echo 'QC run directory: '$QCdir' (QCdir)' >> logs/thisrun.txt
echo 'QC run directory has been set to: '$QCdir
echo -e "\n" >> logs/QClog.txt
echo -e "\n" >> logs/thisrun.txt

echo "  "
echo "Checking to ensure all required files are present."
echo "Disclaimer: this check is not comprehensive...."

##Make sure the setups folder is there and contains the 
##IcemcQCParameters.txt file
ls | egrep -q 'setups'
there=$?

if [ $there -eq 1 ]; then
	echo "  "
	echo 'setups folder not present'
	echo 'Setups folder not present; run aborted.' >> logs/QClog.txt
	echo 'IcemcQC cannot run without setup information'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	return
elif [ $there -eq 0 ]; then
	echo "  "
	echo 'setups folder is present' 
	cd setups
	echo 'Temp Log' > templog.txt
	temp=templog.txt
	ls | egrep -q 'IcemcQCParameters.txt'
	there1=$?
	if [ $there1 -eq 1 ]; then
		echo "  "
		echo 'IcemcQCParameters.txt is not present in setups folder.'
		echo 'IcemcQC cannot run without setup information'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		echo 'IcemcQCParameters.txt is not present; run aborted.'  >> logs/QClog.txt
		return
	elif [ $there1 -eq 0 ]; then
		echo 'IcemcQCParameters.txt is present.'
		mode=$(gawk '/mode/{print $2}' IcemcQCParameters.txt)
		echo "  "
		echo 'Setup Parameters:'
		echo "  "
		echo 'mode: '$mode	
		rundir=$(gawk '/rundir/{print $2}' IcemcQCParameters.txt)
		echo 'run directory: '$rundir
		tgtdir=$(gawk '/tgtdir/{print $2}' IcemcQCParameters.txt)
		echo 'target directory: '$tgtdir
		dwell=$(gawk '/dwell/{print $2}' IcemcQCParameters.txt)
		echo 'dwell time: '$dwell
		echo "  "
		echo 'If these parameters are not correct Icemc may not run properly.'
		grep -q ".txt" IcemcQCParameters.txt
		there2=$?
		if [ $there2 -eq 1 ]; then
			echo 'No setup files specified!'
			echo 'IcemcQC cannot run without setup information.'
			echo "  "
			echo "exiting..."
			echo "  "
			echo "  "
			cd ..
			rm logs/thisrun.txt
			echo 'No setup files specified; run aborted.' >> logs/QClog.txt
			return
		elif [ $there2 -eq 0 ]; then
			echo "  "
			echo "Setup files specified:" 
			gawk '/.txt/{print $2}' IcemcQCParameters.txt
			echo "   "
			SetupNo=$(gawk '/.txt/' IcemcQCParameters.txt | wc -l)
			gawk '/.txt/{print $2}' IcemcQCParameters.txt >> $temp
			for ((j = 1;j <= $SetupNo; j++ ))
				do 
				setup=$(gawk -F'[.]' 'NR=='$j+1'{print $1}' $temp)
				ls | egrep -q $setup
				there3=$?
				if [ $there3 -eq 1 ]; then
					echo $setup' is not present in setups folder.'
					echo $setup' is not present in setups folder; run aborted.' >> logs/QClog.txt
					echo 'Please copy '$setup' to the setups folder or delete from IcemcQCParameters.txt Setup files list and try again!'
					echo "  "
					echo "exiting..."
					echo "  "
					echo "  "
					cd ..
					rm logs/thisrun.txt
					return
				elif [ $there3 -eq 0 ]; then
					echo $setup' is present in setups folder.'
				fi
			done
		fi
	fi
	rm templog.txt
	cd ..
fi

##Make sure the plots folder is there and contains all 
##specified plot files and their make files.

ls | egrep -q 'plots'
there4=$?

if [ $there4 -eq 1 ]; then
	echo "  "
	echo 'plots folder not present'
	echo 'Plots folder not present; run aborted.' >> logs/QClog.txt
	echo 'IcemcQC cannot make any plots without a plots folder!'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	return
elif [ $there4 -eq 0 ]; then
	echo "  "
	echo 'plots folder is present'
	echo "  "
	echo 'Temp Log' > templog.txt
	temp=templog.txt
	grep -q "script" setups/IcemcQCParameters.txt
	there4b=$?
	if [ $there4b -eq 1 ]; then
		echo 'No plot files specified; IcemcQC cannot run without plot files.'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		rm logs/thisrun.txt
		echo 'No plot files specified or plot files specified incorrectly; run aborted.' >> logs/QClog.txt
		return
	elif [ $there4b -eq 0 ]; then
		echo "Plot files specified:" 
		gawk '/script/{print $2}' setups/IcemcQCParameters.txt
		echo "   "
		PlotNo=$(gawk '/script/' setups/IcemcQCParameters.txt | wc -l)
		#echo 'PlotNo: '$PlotNo
		gawk '/script/{print $2}' setups/IcemcQCParameters.txt >> $temp
		for ((j = 1;j <= $PlotNo; j++ ))
			do 
			script=$(gawk -F'[.]' 'NR=='$j+1'{print $1}' $temp)
			cd plots
			fullscript=$script'.*'
			ls | egrep -q "$fullscript"
			there5=$?
			if [ $there5 -eq 1 ]; then
				echo $fullscript' is not present in plots folder.'
				cd ..
				echo $fullscript' is not present in plots folder; run aborted.' >> logs/QClog.txt
				echo 'Please copy '$fullscript' to the plots folder or delete from IcemcQCParameters.txt Plot files list and try again!'
				echo "  "
				echo "exiting..."
				echo "  "
				echo "  "
				rm logs/thisrun.txt
				return
			elif [ $there5 -eq 0 ]; then
				echo $fullscript' is present in plots folder.'
				cd ..
			fi
			maker=$(gawk -F'[.]' 'NR=='$j+1'{print $1}' $temp)
			maker='M.'$maker
			cd plots
			ls | egrep -q $maker
			there6=$?
			if [ $there6 -eq 1 ]; then
				cd ..
				echo 'make file '$maker' is not present in plots folder.'
				echo 'make file '$maker' is not present in plots folder; run aborted.' >> logs/QClog.txt
				echo 'Please copy '$maker' to the plots folder or delete from IcemcQCParameters.txt Plot files list and try again!'
				echo "  "
				echo "exiting..."
				echo "  "
				echo "  "
				rm logs/thisrun.txt
				return
			elif [ $there6 -eq 0 ]; then
				echo $maker' is present in plots folder.'
				echo "  "
				cd ..
			fi
		done
	fi
	rm templog.txt
fi
	
##Make sure the IcemcQC executables are all there

if [ $mode == automatic ]; then
	ls | egrep -q 'IcemcQCUpdater.sh'
	there7=$?
	if [ $there7 -eq 1 ]; then
		echo "  "
		echo 'IcemcQCUpdater.sh is not present.  Cannot run IcemcQC in automatic mode.'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		rm logs/thisrun.txt
		echo 'IcemcQCUpdater.sh is not present; run aborted.' >> logs/QClog.txt
		return
	fi

	ls | egrep -q 'IcemcQCVersion.sh'
	there8=$?
	if [ $there8 -eq 1 ]; then
		echo "  "
		echo 'IcemcQCVersion.sh is not present.  Cannot run IcemcQC in automatic mode.'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		rm logs/thisrun.txt
		echo 'IcemcQCVersion.sh is not present; run aborted.' >> logs/QClog.txt
		return
	fi
	
	ls | egrep -q 'IcemcQCSetup.sh'
	there9=$?
	if [ $there9 -eq 1 ]; then
		echo "  "
		echo 'IcemcQCSetup.sh is not present.  Cannot run IcemcQC in automatic mode.'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		rm logs/thisrun.txt
		echo 'IcemcQCSetup.sh is not present; run aborted.' >> logs/QClog.txt
		return
	fi
fi

ls | egrep -q 'IcemcQCSubmit.sh'
there10=$?
if [ $there10 -eq 1 ]; then
	echo "  "
	echo 'IcemcQCSubmit.sh is not present.  Cannot run IcemcQC.'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	echo 'IcemcQCSubmit.sh is not present; run aborted.' >> logs/QClog.txt
	return
fi

ls | egrep -q 'IcemcQCPlotter.sh'
there11=$?
if [ $there11 -eq 1 ]; then
	echo "  "
	echo 'IcemcQCPlotter.sh is not present.  Cannot run IcemcQC.'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	echo 'IcemcQCPlotter.sh is not present; run aborted.' >> logs/QClog.txt
	return
fi	

ls | egrep -q 'IcemcQCSorter.sh'
there11=$?
if [ $there11 -eq 1 ]; then
	echo "  "
	echo 'IcemcQCSorter.sh is not present.  Cannot run IcemcQC.'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	echo 'IcemcQCsorter.sh is not present; run aborted.' >> logs/QClog.txt
	return
fi

ls | egrep -q 'IcemcQCCloseout.sh'
there12=$?
if [ $there12 -eq 1 ]; then
	echo "  "
	echo 'IcemcQCCloseout.sh is not present.  No status report will be sent and IcemcQC website will not be updated.'
	echo 'IcemcQCsorter.sh is not present.  No status report will be sent and IcemcQC website will not be updated.' >> logs/QClog.txt
fi
	
echo "  "
echo "  "

egrep -q 'manual' setups/IcemcQCParameters.txt
there13=$?
if [ $there13 -eq 0 ]; then
	. IcemcQCSubmit.sh
elif [ $there13 -eq 1 ]; then
	egrep -q 'automatic' setups/IcemcQCParameters.txt
	there14=$?
	if [ $there14 -eq 0 ]; then

                #keith edits
                . IcemcQCfind.sh
		 #. IcemcQCUpdater.sh


	else
		echo 'mode entered incorrectly in IcemcQCParameters.txt'
		echo 'mode must be manual or automatic'
		echo 'exiting IcemcQC'
		echo "  "
		echo "exiting..."
		echo "  "
		echo "  "
		rm logs/thisrun.txt
		echo 'mode entered incorrectly in IcemcQCParameters.txt; run aborted.' >> logs/QClog.txt
		return
	fi
else 
	echo 'mode entered incorrectly in IcemcQCParameters.txt'
	echo 'mode must be manual or automatic'
	echo 'exiting IcemcQC'
	echo "  "
	echo "exiting..."
	echo "  "
	echo "  "
	rm logs/thisrun.txt
	echo 'mode entered incorrectly in IcemcQCParameters.txt; run aborted.' >> logs/QClog.txt
	return
fi
