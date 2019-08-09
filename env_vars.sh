#.bashrc

# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi

module load cmake/3.11.4
module load gnu/7.3.0
export CC=`which gcc`
export CXX=`which g++`

export ROOTSYS=/fs/project/PAS0654/shared_software/anita/owens_pitzer/build/root
eval 'source /fs/project/PAS0654/shared_software/anita/owens_pitzer/build/root/bin/thisroot.sh'

export ICEMC_SRC_DIR=/users/PAS0654/osu10204/icemcQC_keith/ICEMCQC_rundir/icemc
export ICEMC_BUILD_DIR=/users/PAS0654/osu10204/icemcQC_keith/ICEMCQC_rundir/icemc
export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}
