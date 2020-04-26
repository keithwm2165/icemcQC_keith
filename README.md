# icemcQC_keith
Quality Control for icemc. This is integrated from Hughes (which was for svn). This repo is for testing and editing only.

The code should work as follows: 

	1)executing the command "source IcemcQC.sh" executes IcemcQC.sh
          2)IcemcQCfind.sh runs next inside of IcemcQC.sh. this code then runs the next script which is
            3) IcemcQCUpdater.sh. This will git clone icemc or git pull for already installed icemc. then makes icemc as well
              4) IcemcQCSetup.sh runs next and cps files and gets the icemc runs ready with correct variables and files in the correct paths. 
                5)


In order to run icemcQC, a number of changes need to be made before the main command "source IcemcQC.sh" is executed.

1. edit the file "env_.sh" to include the correct paths of the environmental variables 
     - namely ROOTSYS needs to be correctly defined

How to use icemcQC_V_N:

1. upzip this directory after download icemcQC_keith
2. put bash_profile and bash_anita.sh into your home directory where you are ready to run QC (OSC cluster required)
3. change the path of bash_profile and bash_anita.sh to your own path on OSC
4. download icemc and put rest files in icemcQC_V_N into icemc directory
5. source QCSubmit.sh "energy" to run and submit icemc jobs
6. source QCPlotter.sh "branch" "energy" to make plots
