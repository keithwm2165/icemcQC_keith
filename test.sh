echo 'export ANITA_UTIL_INSTALL_DIR=/home/mcbride.342/anitaBuildTool/Utils' > env_vars_icemcQC.sh
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ANITA_UTIL_INSTALL_DIR/lib' >> env_vars_icemcQC.sh
echo 'export ICEMC_SRC_DIR=$path_dir/icemc' >> env_vars_icemcQC.sh
echo 'export ICEMC_BUILD_DIR=$path_dir/icemc' >> env_vars_icemcQC.sh
echo 'export LD_LIBRARY_PATH=${ICEMC_BUILD_DIR}:${LD_LIBRARY_PATH}' >> env_vars_icemcQC.sh

