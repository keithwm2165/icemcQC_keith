export ROOTSYS=/users/PAS0174/osu8620/root-6.08.06
export LD_INCLUDE_PATH=/users/PAS0174/osu8620/cint/libcint/build/include:$LD_INCLUDE_PATH
module load fftw3/3.3.5
module load gnu/6.3.0
module load python/3.4.2
module load cmake/3.7.2
export CC=/usr/local/gcc/6.3.0/bin/gcc
eval source /users/PAS0174/osu8620/root-6.08.06/builddir/bin/thisroot.sh
export ICEMC_SRC_DIR=/users/PAS0654/osu10204/icemcQC_keith/ICEMCQC_rundir/icemc
export ICEMC_BUILD_DIR=/users/PAS0654/osu10204/icemcQC_keith/ICEMCQC_rundir/icemc
export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}
