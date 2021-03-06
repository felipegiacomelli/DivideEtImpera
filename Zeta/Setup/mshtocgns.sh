#!/bin/bash

LIBRARY="mshtocgns-$MSHTOCGNS_VERSION"
BUILD_TYPE=`echo ${BUILD_TYPE,,}`
FLAGS="-DENABLE_MSH=OFF -DBUILD_SHARED_LIBS=TRUE -DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ ! -d "$LIBRARY" ]; then
    git clone -b master "https://github.com/felipegiacomelli/MSHtoCGNS.git"
    mv MSHtoCGNS $LIBRARY
fi

cd $LIBRARY
git checkout "v$MSHTOCGNS_VERSION"

cd Zeta/Setup/
./setup.sh
cd ../..

mkdir build
cd build

cmake .. $FLAGS -DCMAKE_INSTALL_PREFIX=$LIBRARY_INSTALL_DIRECTORY/$LIBRARY/$BUILD_TYPE
make -j $NUMBER_OF_CORES
make test
make install
