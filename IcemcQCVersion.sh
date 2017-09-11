#!/bin/bash

## Run by typing . IcemcVersion.sh

####################################################################
##                                                                ##
##  Author: Khalida Hendricks July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##                                                                ##
##  IcemcVersion parses the update log to find the latest        ##
##  version of Icemc and runs IcemcQC on that version.          ##                          ##
##                                                                ##
####################################################################

echo "  "
echo 'starting IcemcQCVersion.sh'
echo "  "

rundir=$(gawk '/rundir/{print $2}' setups/IcemcQCParameters.txt)
var_path=`pwd`
echo 'current directory: ' $var_path
echo 'run directory:     ' $HOME'/'$rundir

mkdir -p logs
cd logs

##Get JUST the updates for /releases/
gawk '/releases/{print NR, $2}' updatelog.txt > latest.txt

count1=$(wc -l latest.txt)
echo 'Number of updates to releases: ' $count1 >> latest.txt

count1=$(gawk '/latest/{print $6}' latest.txt)
echo 'count1 = ' $count1
echo 'Number of updates to releases: ' $count1 >> thisrun.txt

##Extract a list of any versions that were updated
echo " " > versions.txt

for (( i = 1; i <= $count1; i++ )) 
	do

	version=$(gawk -F'[/]' 'NR=='$i'{print $3}' latest.txt)
	grep "$version" versions.txt
	there=$?
#echo 'version : ' $version
#	echo 'check '$i: $there
	
	if [ $there -eq 1 ]; then
		echo $version >> versions.txt
	fi
	
done

##versions.txt is a file that lists any updated releases
##There should only be one, but in case there are more 
##versions.txt will have them listed

##Now count the versions, and determine which one is the
##latest.  This was harder than I thought and is a loooong
##batch of code.
##echos can be taken out once I am happy that it does what I want.

count2=$(wc -l versions.txt)
echo 'Number of versions updated (plus 1): ' $count2 >> latest.txt
count2=$(gawk '/versions/{print $7}' latest.txt)
let count3=count2-1
let count4=count2+1
echo 'Number of versions updated: ' $count3 >> thisrun.txt
echo '# actual versions (count3): ' $count3
echo '# version lines(count4): ' $count4
##count4 should be the number of versions plus 2

field=$(gawk -F'[.]' '{print NF}' versions.txt)
#echo "Fields: " $field
echo "Fields: "$field >> latest.txt
echo "Fields: "$field

max=0

for (( i = 2; i <= $count4; i++ )) 
	do
	
	numfield=$(gawk '/Fields:/{print $'$i'}' latest.txt)
	echo 'numfield '$i': '$numfield
	if [ $numfield -gt $max ]; then
		max=$numfield
	fi
	echo 'temp max: '$max
done

echo 'max = '$max

for (( i = 1; i <= $max; i++ )) 
	do
	
	M[$i]=0
	
done

latest=0.0
echo ' '
echo ' '
echo 'for loop starts at loop #2'
#echo $latest
for (( i = 2; i <= $count2; i++ )) 
	do
	
	echo " "
	echo " "
	echo 'loop #'$i
	version=$(gawk 'NR=='$i'{print $1}' versions.txt)
	echo 'latest version: ' $latest
	echo 'current version: '$version
	gotcha=1
	dec=1
		
	while [ $gotcha -eq 1 ] 
	do
	
		echo " "
		echo 'dec = '$dec
		digit=$(gawk -F'[.]' 'NR=='$i'{print $'$dec'}' versions.txt)
		echo 'digit = '$digit
		echo 'M['$dec'] = '${M[$dec]}
	
		if [ $digit -gt ${M[$dec]} ]; then
			latest=$version
			M[$dec]=$digit
			gotcha=0
			echo 'M['$dec'] changed to '${M[$dec]}
			let dec=dec+1

			for (( j = $dec; j <= $max; j++ )) 
			do
				echo " "
				echo 'dec = '$j
				digit=$(gawk -F'[.]' 'NR=='$i'{print $'$j'}' versions.txt)
				echo 'digit: ' $digit
				if [ $digit -ge 0 ]; then
					M[$j]=$digit
					echo 'M['$j'] changed to '${M[$j]}
				else 
					echo 'no more digits! unary operator expected'
					M[$j]=0
					echo 'M['$j'] reset to '${M[$j]}
				fi
			done
			
			echo " "
			echo 'Latest version updated.'
		
		elif [ $digit -lt ${M[$dec]} ]; then
			gotcha=0
			echo 'This is not the latest version; no update made.'
		elif [ $digit -eq ${M[$dec]} ]; then
			let dec=dec+1
			gotcha=1
			echo 'It is a tie...trying the next digit.'
		else
			echo 'ERROR HERE'
			gotcha=0
		fi
	
	##This closes the gotcha while
	done

	

##This closes the for while
done

echo 'Latest release version: ' $latest >> thisrun.txt

echo " "
echo " "
echo 'Final answer - the latest version is: ' $latest
echo " "
echo " "

##OK so that was longer/harder that I thought it would be.
##Finally, run IcemcQCSetup on the latest version of Icemc.

cd ..
pwd

. IcemcQCSetup.sh releases/$latest

##The End
