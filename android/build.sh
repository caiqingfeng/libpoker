# how to setup ndk standlone tool chains on mac
# $download android-ndk-r10c.bin
# $chmod +x android-ndk-r10c.bin
# $./android-ndk-r10c.bin
# $export ANDROID_NDK_ROOT=`pwd`
# $./build/tools/make-standalone-toolchain.sh --platform=android-8 --install-dir=$HOME/Desktop/installed-tools/android-ndk-toolchains-r10c

# export ANDROID_NDK_ROOT=/Users/caiqingfeng/Desktop/tools/android-ndk-r10c
# export ANDROID_TOOLCHAINS_BIN=/Users/caiqingfeng/Desktop/installed-tools/android-ndk-toolchains-r10c/bin
[[ -n $ANDROID_NDK_ROOT ]]&&echo exist ndk_root||export ANDROID_NDK_ROOT=/home/luyunstaff001/software/android-ndk-r10c
[[ -n $ANDROID_TOOLCHAINS_HOME ]]&&echo exist toolchains_home||export ANDROID_TOOLCHAINS_HOME=/home/luyunstaff001/software/android-toolchain-r10c
WORK_PATH=`cd $(dirname $0) && pwd`
SYSROOT=$ANDROID_NDK_ROOT/platforms/android-8/arch-arm
export SYSROOT
#export PATH=$NDK_PATH/toolchains:$PATH
#export CXX=/opt/android-toolchain/bin/arm-linux-androideabi-g++
export CXX=$ANDROID_TOOLCHAINS_HOME/bin/arm-linux-androideabi-g++
#export CXXFLAGS="-DNDEBUG -g -O2 -march=armv5te -pipe -DANDROID -D_MOBILE_ -I/usr/local/include -I${WORK_PATH} -I${WORK_PATH}/vendor -I${WORK_PATH}/vendor/zmq -I${WORK_PATH}/vendor/protobuf -I${WORK_PATH}/vendor/cryptopp"
export CXXFLAGS="-DNDEBUG -g -O2 -std=c++11 -march=armv5te -pipe -DANDROID -D_MOBILE_ -I/usr/local/include -I${WORK_PATH} -I${WORK_PATH}/zmq -I${WORK_PATH}/protobuf -I${WORK_PATH}/cryptopp -I${WORK_PATH}/../.."
export AR=$ANDROID_TOOLCHAINS_HOME/bin/arm-linux-androideabi-ar
export ARFLAGS=-cr
export RANLIB=$ANDROID_TOOLCHAINS_HOME/bin/arm-linux-androideabi-ranlib
make init
make
make install
