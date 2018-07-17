#!/bin/bash

##Run by typing . IcemcQCPlotter.sh

####################################################################
##                                                                ##
##  Author: Victoria Niu      July 2018                           ##
##  niu.172@osu.edu                                               ##
##                                                                ##
##  Revised by Keith McBride                                      ##
##                                                                ##
##  QCPlotter runs the plot code put in plots directory, makes    ##
##  plots from icemc outputs, and submits the plots to QC website ##
##                                                                ##
####################################################################

#In the ICEMCQC_rundir directory when QCSubmit finished
echo "  "
echo 'Starting IcemcQCPlotter'
echo "  "

cd .. #go to icemcQC_keith
current_dir=$(pwd) #current_dir is pwd
echo $current_dir

cd ICEMCQC_rundir #go back to ICEMCQC_rundir

RUN_NO=$(gawk 'NR=='11'{print $1}' IcemcQCParameters.txt) #read run number from IcemcQCParameters.txt

#Go to icemc directory
cd icemc
echo 'current directory:'
pwd

#In ICEMCQC_rundir/icemc directory
#delete the past directory and create a new one
rm -rf icefinal_root
mkdir icefinal_root

#name new commit of icemc
commit=`git rev-parse --short HEAD`
echo $commit

#back to icemcQC_keith directory
cd ../../
mkdir $(eval echo $commit)

cd ICEMCQC_rundir/icemc

#In ICEMCQC_rundir/icemc directory
for (( i = 1; i <= $RUN_NO; i++ ))
    do
    echo 'icefinal'$i'.root'
    cp $current_dir/ICEMCQC_rundir/icemc/icemc_outputs$i/icefinal$i.root $current_dir/ICEMCQC_rundir/icemc/icefinal_root
  done

echo "  "

echo "Submitting plots..."
echo "  "

cd ../../
#In icemcQC_keith directory
cp $current_dir/plots/M.read_Primaries $current_dir/ICEMCQC_rundir/icemc/
cp $current_dir/plots/read_Primaries.cc $current_dir/ICEMCQC_rundir/icemc/

#Go back to ICEMCQC_rundir/icemc/
cd ICEMCQC_rundir/icemc/ 

#Create a new directory to store plots, remove the previous one
rm -rf Primaries
mkdir Primaries

#compile read_Primaries.cc file to make plots
make -f M.read_Primaries

for ((i = 1;i <= $RUN_NO; i++ ))
do

#back to icemcQC_keith directory
cd ../../
#In commit directory
cd $commit
#In plots directory
mkdir plots$i
#back to icemcQC_keith directory
cd ../

#Back to ICEMCQC_rundir/icemc/ directory
cd ICEMCQC_rundir/icemc/

#current_date=$(date "+%m-%d-%Y")
#directory_name=plots.$current_date.$i

#echo "directory where plots are stored is: " $current_dir/$directory_name

echo "directory where plots are stored is: "$current_dir/$commit/plots$i

./read_Primaries icefinal_root/icefinal$i.root
cd Primaries
cp *.pdf $current_dir/$commit/plots$i
cp *.png $current_dir/$commit/plots$i
cp *.root $current_dir/$commit/plots$i

cd ../
done

echo "current directory: "
pwd

cd ../../ #back to icemcQC_keith directory
cd setups
energy=$(gawk 'NR=='37'{print $3}' inputs_anita3.txt) #read energy level from inputs_anita3.txt
cd ..

#Here is the rename of the plots
cd $commit
for (( i=1;i <= $RUN_NO; i++))
do
cd plots$i
mv altitude_int.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_altitude_int.pdf
mv altitude_int.png Primaries_"$commit"_input_anita3_energy_"$energy"_altitude_int.png
mv altitude_int.root Primaries_"$commit"_input_anita3_energy_"$energy"_altitude_int.root

mv posnu_xy.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_posnu_xy.pdf
mv posnu_xy.png Primaries_"$commit"_input_anita3_energy_"$energy"_posnu_xy.png
mv posnu_xy.root Primaries_"$commit"_input_anita3_energy_"$energy"_posnu_xy.root

mv r_fromballoon.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon.pdf
mv r_fromballoon.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon.png
mv r_fromballoon.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon.root

mv nuflavorint.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_nuflavorint.pdf
mv nuflavorint.png Primaries_"$commit"_input_anita3_energy_"$energy"_nuflavorint.png
mv nuflavorint.root Primaries_"$commit"_input_anita3_energy_"$energy"_nuflavorint.root

mv r_fromballoon_sq.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq.pdf
mv r_fromballoon_sq.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq.png
mv r_fromballoon_sq.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq.root

mv r_fromballoon_vs_altitude_int.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_vs_altitude_int.pdf
mv r_fromballoon_vs_altitude_int.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_vs_altitude_int.png
mv r_fromballoon_vs_altitude_int.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_vs_altitude_int.root

mv r_fromballoon_sq_vs_altitude_int.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq_vs_altitude_int.pdf
mv r_fromballoon_sq_vs_altitude_int.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq_vs_altitude_int.png
mv r_fromballoon_sq_vs_altitude_int.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_fromballoon_sq_vs_altitude_int.root

mv fresnel1.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel1.pdf
mv fresnel1.png Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel1.png
mv fresnel1.root Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel1.root

mv fresnel2.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel2.pdf
mv fresnel2.png Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel2.png
mv fresnel2.root Primaries_"$commit"_input_anita3_energy_"$energy"_fresnel2.root

mv currentint.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_currentint.pdf
mv currentint.png Primaries_"$commit"_input_anita3_energy_"$energy"_currentint.png
mv currentint.root Primaries_"$commit"_input_anita3_energy_"$energy"_currentint.root

mv dtryingdirection.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_dtryingdirection.pdf
mv dtryingdirection.png Primaries_"$commit"_input_anita3_energy_"$energy"_dtryingdirection.png
mv dtryingdirection.root Primaries_"$commit"_input_anita3_energy_"$energy"_dtryingdirection.root

mv chord_kgm2_bestcase.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_chord_kgm2_bestcase.pdf
mv chord_kgm2_bestcase.png Primaries_"$commit"_input_anita3_energy_"$energy"_chord_kgm2_bestcase.png
mv chord_kgm2_bestcase.root Primaries_"$commit"_input_anita3_energy_"$energy"_chord_kgm2_bestcase.root

mv weight_bestcase.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_weight_bestcase.pdf
mv weight_bestcase.png Primaries_"$commit"_input_anita3_energy_"$energy"_weight_bestcase.png
mv weight_bestcase.root Primaries_"$commit"_input_anita3_energy_"$energy"_weight_bestcase.root

mv logchord.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_logchord.pdf
mv logchord.png Primaries_"$commit"_input_anita3_energy_"$energy"_logchord.png
mv logchord.root Primaries_"$commit"_input_anita3_energy_"$energy"_logchord.root

mv r_exit2bn.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn.pdf
mv r_exit2bn.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn.png
mv r_exit2bn.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn.root

mv r_exit2bn_measured.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn_measured.pdf
mv r_exit2bn_measured.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn_measured.png
mv r_exit2bn_measured.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_exit2bn_measured.root

mv nuexit.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_nuexit.pdf
mv nuexit.png Primaries_"$commit"_input_anita3_energy_"$energy"_nuexit.png
mv nuexit.root Primaries_"$commit"_input_anita3_energy_"$energy"_nuexit.root

mv nuexitice.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_nuexitice.pdf
mv nuexitice.png Primaries_"$commit"_input_anita3_energy_"$energy"_nuexitice.png
mv nuexitice.root Primaries_"$commit"_input_anita3_energy_"$energy"_nuexitice.root

mv costheta_nutraject.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_costheta_nutraject.pdf
mv costheta_nutraject.png Primaries_"$commit"_input_anita3_energy_"$energy"_costheta_nutraject.png
mv costheta_nutraject.root Primaries_"$commit"_input_anita3_energy_"$energy"_costheta_nutraject.root

mv d1.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_d1.pdf
mv d1.png Primaries_"$commit"_input_anita3_energy_"$energy"_d1.png
mv d1.root Primaries_"$commit"_input_anita3_energy_"$energy"_d1.root

mv d2.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_d2.pdf
mv d2.png Primaries_"$commit"_input_anita3_energy_"$energy"_d2.png
mv d2.root Primaries_"$commit"_input_anita3_energy_"$energy"_d2.root

mv d1+d2.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_d1+d2.pdf
mv d1+d2.png Primaries_"$commit"_input_anita3_energy_"$energy"_d1+d2.png
mv d1+d2.root Primaries_"$commit"_input_anita3_energy_"$energy"_d1+d2.root

mv nnu.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_nnu.pdf
mv nnu.png Primaries_"$commit"_input_anita3_energy_"$energy"_nnu.png
mv nnu.root Primaries_"$commit"_input_anita3_energy_"$energy"_nnu.root

mv theta.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_theta.pdf
mv theta.png Primaries_"$commit"_input_anita3_energy_"$energy"_theta.png
mv theta.root Primaries_"$commit"_input_anita3_energy_"$energy"_theta.root

mv r_in.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_in.pdf
mv r_in.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_in.png
mv r_in.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_in.root

mv r_enterice.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_r_enterice.pdf
mv r_enterice.png Primaries_"$commit"_input_anita3_energy_"$energy"_r_enterice.png
mv r_enterice.root Primaries_"$commit"_input_anita3_energy_"$energy"_r_enterice.root

mv chord.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_chord.pdf
mv chord.png Primaries_"$commit"_input_anita3_energy_"$energy"_chord.png
mv chord.root Primaries_"$commit"_input_anita3_energy_"$energy"_chord.root

mv deviewangle_deg.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_deviewangle_deg.pdf
mv deviewangle_deg.png Primaries_"$commit"_input_anita3_energy_"$energy"_deviewangle_deg.png
mv deviewangle_deg.root Primaries_"$commit"_input_anita3_energy_"$energy"_deviewangle_deg.root

mv n_exit_phi.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_n_exit_phi.pdf
mv n_exit_phi.png Primaries_"$commit"_input_anita3_energy_"$energy"_n_exit_phi.png
mv n_exit_phi.root Primaries_"$commit"_input_anita3_energy_"$energy"_n_exit_phi.root

mv mytheta.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_mytheta.pdf
mv mytheta.png Primaries_"$commit"_input_anita3_energy_"$energy"_mytheta.png
mv mytheta.root Primaries_"$commit"_input_anita3_energy_"$energy"_mytheta.root

mv mybeta.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_mybeta.pdf
mv mybeta.png Primaries_"$commit"_input_anita3_energy_"$energy"_mybeta.png
mv mybeta.root Primaries_"$commit"_input_anita3_energy_"$energy"_mybeta.root

mv theta_rf_atbn.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_theta_rf_atbn.pdf
mv theta_rf_atbn.png Primaries_"$commit"_input_anita3_energy_"$energy"_theta_rf_atbn.png
mv theta_rf_atbn.root Primaries_"$commit"_input_anita3_energy_"$energy"_theta_rf_atbn.root

mv nnu_costheta_cosphi.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_nnu_costheta_cosphi.pdf
mv nnu_costheta_cosphi.png Primaries_"$commit"_input_anita3_energy_"$energy"_nnu_costheta_cosphi.png
mv nnu_costheta_cosphi.root Primaries_"$commit"_input_anita3_energy_"$energy"_nnu_costheta_cosphi.root

mv rin_costheta_phi.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_rin_costheta_phi.pdf
mv rin_costheta_phi.png Primaries_"$commit"_input_anita3_energy_"$energy"_rin_costheta_phi.png
mv rin_costheta_phi.root Primaries_"$commit"_input_anita3_energy_"$energy"_rin_costheta_phi.root

mv theta_vs_phi.pdf Primaries_"$commit"_input_anita3_energy_"$energy"_theta_vs_phi.pdf
mv theta_vs_phi.png Primaries_"$commit"_input_anita3_energy_"$energy"_theta_vs_phi.png
mv theta_vs_phi.root Primaries_"$commit"_input_anita3_energy_"$energy"_theta_vs_phi.root

done

#now put plots into website directory
#cd plots 
#cp *.pdf ./.
#cd  Primariesplots
#cp *.pdf /n/home00/hughes.525/public_html/closed_pages/plots/primaryplots


echo 'QCjobs complete! Good job!!!'
cd $current_dir #back to icemcQC_keith
return
