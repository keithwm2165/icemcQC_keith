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

2. 
