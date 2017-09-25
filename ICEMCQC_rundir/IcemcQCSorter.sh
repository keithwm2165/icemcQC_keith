#!/bin/bash

##Run by typing . IcemcQCSorter.sh

####################################################################
##                                                                ##
##  Author: Khalida Hendricks July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##                                                                ##
##  IcemcQCSorter organizes all IcemcQC products into           ##
##  appropriate folders by release/#.# or trunk/version#, then    ##
##  by setupfile (or logs).  It then runs IcemcQCPlotter to      ##
##  make required plots.                                          ##
##                                                                ##
####################################################################

echo "  "
echo 'starting IcemcQCSorter'
echo "  "

tgtdir=$(gawk '/tgtdir/{print $2}' IcemcQCParameters.txt)
localdir=`pwd`
echo 'current dir : '$localdir
echo 'target dir : '$localdir
echo " "

mkdir -p logs

##NEED TO DO:
##1)add plots
##2)organize output

##NEED TO DO:
##1)gather/organize/prep IcemcQC files
##2)count number of trunk directories and delete 11th directory

##NEED TO DO:
##1)website?


##NEED TO DO:
##1)run plotting programs, sort everything into appropriate files
##     ie, trunk/469
##     ie, release/1.0
##  I think it would be best to have one shell that qsubs the plots 
##  individually; that way plots are modular and can be easily 
##  edited/added
