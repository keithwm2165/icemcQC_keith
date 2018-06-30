#!/bin/bash

##Run by typing . IcemcQCPlotter.sh

####################################################################
##                                                                ##
##  Author: Kaeli Hughes      July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##                                                                ##
##                                                                ##
####################################################################

echo "  "
echo 'Starting IcemcQCPlotter'
echo "  "

##setup log files from which to read output files
#Keith edits
curr_dir=`pwd`
cd $curr_dir/icemc
echo 'current dir : '$curr_dir
#Keith edits
#date > outputs/outputlog.txt
#date > outputs/templog.txt 
#date  --date="yesterday" >> outputs/outputlog.txt

outlog=output.txt
#output=output.txt
#SetupNo=$(gawk '/.txt/' IcemcQCParameters.txt | wc -l)

#echo "Setup files used:" >> outputs/$outlog
#gawk '/.txt/{print $1, $2}' IcemcQCParameters.txt >> outputs/$outlog

#Remove organization files/directories from previous run of Plotter (Do not remove icefinal.root since it will rewritten next time running QC)
cd outputs
rm -rf current_root
#rm icefinal.root
mkdir current_root

#today=$(gawk 'NR==1{print $3}' $outlog)
#echo $today
#yesterday=$(gawk 'NR==2{print $3}' $outlog)
#echo $yesterday

##grab the icefinal*.root output files
ls -l | gawk '/icefinal/{print $7, $9}' >> $outlog

##check the date to make sure they were created (modified) 
##within the last 24-48 hours
outNo=$(gawk '/icefinal/' $outlog | wc -l)
echo '# of output (*.root) files found: '$outNo
echo "  "
echo "Checking dates on output files..."
echo "  "
#echo 'Output files used:'>>templog.txt

#for (( i = 1; i <= $outNo; i++ )) 
#	do
#	outputdate[$i]=$(gawk 'NR=='$i+2'{print $1}' $output)
#	echo 'run date: '$today
#	echo 'output date: '${outputdate[$i]}
#
#if [ "${outputdate[$i]}" == "$today" ]; then
#		echo 'record #'$i' matches today'
#		outputfile=$(gawk 'NR=='$i+2'{print $2}' $outlog)
#		echo $outputfile >> templog.txt
#		cp icefinal$i.root current_root
#	elif [ "${outputdate[$i]}" == "$yesterday" ]; then
#		echo 'record #'$i' matches yesterday'
#		outputfile=$(gawk 'NR=='$i+2'{print $2}' $outlog)
#		echo $outputfile >> templog.txt
#		cp icefinal$i.root current_root
#	else
#		echo 'record #'$i' is too old...not included'
#	fi
# done

#mv templog.txt outputlog.txt

outNo=$(gawk '/.root/' $outlog | wc -l)
echo $outNo
#At this point we could have icefinal.root files that were created from different input scripts.  To deal with this, I create Setup files for each set of input files and add the appropriate icefinal* files to each one.
##grab setup file names and number of runs per setup file
testdir=`pwd`
echo $testdir
cd $curr_dir
cd ..
echo "Setup files used:" >> $curr_dir/icemc/outputs/$outlog
echo `pwd`
SetupNo=$(gawk '/.txt/' setups/IcemcQCParameters.txt | wc -l)
QC_path=`pwd`
echo `pwd`
gawk '/.txt/{print $1, $2}' $QC_path/setups/IcemcQCParameters.txt >> $curr_dir/icemc/outputs/$outlog
echo "   "
echo $SetupNo
echo `pwd`
totruns=0
var=0
for ((j = 1;j <= $SetupNo; j++ ))
	do 
	let line=$j+3+$outNo
	setup[$j]=$(gawk -F'[ .]' 'NR=='$line'{print $2}' $curr_dir/icemc/outputs/$outlog)
	echo 'setup['$j'] = '${setup[$j]}
	numruns[$j]=$(gawk 'NR=='$line'{print $1}' $curr_dir/icemc/outputs/$outlog)
	echo 'numruns['$j'] = '${numruns[$j]}
	let totruns=$totruns+${numruns[$j]}
done
echo ${setup[2]}
newvar[1]=${numruns[1]}
echo 'newvar 1 is'${newvar[1]}
cd $curr_dir/icemc/outputs/current_root
mkdir Setup1
#newvar[] is an array that holds the total amount of runs for each input file. For example, if numruns[]=[2 4 3] then newvar[]=[2 2+4 2+4+3]=[2 6 9].
#This for loop fills newvar and makes a directory depending on the input file
for((i = 2;i <= $SetupNo; i++ ))
	do
        echo 'numrun i '${numruns[$i]}
	echo 'newvar -1'${newvar[$i-1]}
	let newvar[$i]=${numruns[$i]}+${newvar[$i-1]}
	echo ${newvar[$i]}
	mkdir Setup$i
done

for((k = 1;k <= ${newvar[1]}; k++ ))
        do
        mv icefinal$k.root Setup1
done

#now go into each setup file and add icefinal* files together for plotting purposes.
cd Setup1
echo `pwd` " below setup1 " 
hadd icefinal_1.root icefinal*
cd ..

for((j = 2;j <= $SetupNo; j++ ))
        do
        echo 'newvar -1 is '${newvar[$j-1]}
        echo 'newvar is '${newvar[$j]}
	for((k = ${newvar[$j-1]};k < ${newvar[$j]}; k++ ))
	    do
	    let m=$k+1
	    echo $m
	    mv icefinal$m.root Setup$j
	done
	cd Setup$j
	hadd icefinal_$j.root icefinal*
	cd ..
done

echo `pwd` " before total numbers of runs check"
echo $outNo
echo $totruns
cd ..
cd ..
if [ "$outNo" == "$totruns" ]; then
	echo "  "
	echo 'Total numbers of runs is correct'
	echo "  "
else
	echo 'ERROR: Total numbers of runs is NOT correct'
	echo '   '
	echo 'exiting...'
	echo "   "
	echo "   "
	return		
fi

#cd outputs/current_root

#addfiles=`hadd icefinal.root icefinal*`
#echo $addfiles
#cd ..
#cd ..
##grab plotting scripts to be run
echo `pwd`
PlotNo=$(gawk '/script/' $QC_path/setups/IcemcQCParameters.txt | wc -l)
echo "Plot files used:" >> $curr_dir/icemc/outputs/$outlog
gawk '/script/{print $2}' $QC_path/setups/IcemcQCParameters.txt >> $curr_dir/icemc/outputs/$outlog

for ((j = 1;j <= $PlotNo; j++ ))
	do 
	let line=$j+4+$SetupNo+$outNo
	plot2[$j]=$(gawk -F'[ ]' 'NR=='$line'{print $1}' $curr_dir/icemc/outputs/$outlog)
	echo 'plot2['$j'] = '${plot2[$j]}
	something='hello'
done
	
echo "  "
echo "Submitting plots..."
echo "  "
echo "Plot number is "$PlotNo
pwd

version=$(awk /revision/'{print $4}' thisrun.txt)
echo 'version is' $version

cd $curr_dir
cd ../plots
make -f M.readPrimaries
mkdir Primaryplots
for ((i = 1;i <= $SetupNo; i++ ))
        do
	./readPrimaries $curr_dir/icemc/outputs/current_root/Setup$i/icefinal_$i.root
	cd $curr_dir/icemc/outputs/current_root
	mkdir rev${version}${setup[$i]}
	mv Setup$i rev${version}${setup[$i]}
	cd ..
	cd ..
	cp Primaryplots/. outputs/current_root/rev${version}${setup[$i]}/ -R
	cp outputs/current_root/. /n/home00/hughes.525/public_html/closed_pages/plots/testplots/ -R	
done
echo 'what is this value?' ${setup[1]}

#cd outputs/current_root
#for ((j = 1; j<= $SetupNo; j++ ))
#    do
#    mkdir rev${version}${setup[$j]}
#    mv Setup$j rev${version}${setup[$j]}
#       
#done


#now put plots into website directory
#cd primaryplots
#cp *.pdf /n/home00/hughes.525/public_html/closed_pages/plots/primaryplots

#cd
#cd Practice
