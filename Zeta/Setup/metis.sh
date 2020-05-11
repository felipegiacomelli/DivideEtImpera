#!/bin/bash

LIBRARY="metis-$METIS_VERSION"
DOWNLOAD_LINK="http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/$LIBRARY.tar.gz"
COMPRESSED_LIBRARY=$LIBRARY.tar.gz
BUILD_TYPE=`echo ${BUILD_TYPE,,}`
FLAGS="-DMETIS_USE_LONGINDEX=1 -DMETIS_USE_DOUBLEPRECISION=1 -DGKLIB_PATH=../GKlib"

if [ ! -d "$LIBRARY" ]; then
    if [ ! -f "$COMPRESSED_LIBRARY" ]; then
        wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY
        tar -x -z -f $COMPRESSED_LIBRARY
    else
        tar -x -z -f $COMPRESSED_LIBRARY
    fi
fi

cd $LIBRARY

cd build

cmake .. $FLAGS -DCMAKE_INSTALL_PREFIX=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE
make -j $NUMBER_OF_CORES
make install
