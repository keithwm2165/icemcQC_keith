#!/bin/bash

## Run by typing . IcemcQCSetup.sh

####################################################################
##                                                                ##
##  Author: Khalida Hendricks July 2014                           ##
##  hendricks.189@osu.edu                                         ##
##                                                                ##
##  IcemcQCSetup sets up the updated Icemc directories (trunk     ##
##  or latest release).  Copies required setup files and plot     ##
##  scripts, creates required txt files, etc.                     ##                          
##                                                                ##
####################################################################

## $1 is the command line argument that gives the target directory

echo "  "
echo 'starting IcemcQCSetup'
echo "  "

rundir=$(gawk '/rundir/{print $2}' setups/IcemcQCParameters.txt)

script_path=$(pwd)

cp env_vars_icemcQC.sh $script_path/$rundir/env_vars_icemcQC.sh
cp IcemcQCSubmit.sh $script_path/$rundir/IcemcQCSubmit.sh
cp IcemcQCPlotter.sh $script_path/$rundir/IcemcQCPlotter.sh
cp IcemcQCSorter.sh $script_path/$rundir/IcemcQCSorter.sh
cp runIcemc.sh $script_path/$rundir/runIcemc.sh

cp setups/* $script_path/$rundir/.
cp plots/* $script_path/$rundir/.
cp logs/thisrun.txt $script_path/$rundir/thisrun.txt
cd $script_path/$rundir/

pwd
var_path=`pwd`
chmod 777 -R $var_path
ls *.txt

. IcemcQCSubmit.sh
