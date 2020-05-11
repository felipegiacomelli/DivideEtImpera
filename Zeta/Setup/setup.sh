#!/bin/bash

export SETUP_DIRECTORY=$PWD
export LIBRARY_INSTALL_DIRECTORY=$PWD/../Libraries
export BUILD_TYPE="release"
export NUMBER_OF_CORES=`nproc`

export MSHTOCGNS_VERSION="4.0.0"
export METIS_VERSION="5.1.0"

cd ..

if [ ! -d Libraries ]
then
    mkdir Libraries
fi

cd $SETUP_DIRECTORY

. mshtocgns.sh

cd $SETUP_DIRECTORY

. metis.sh
