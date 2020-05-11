#!/bin/bash

rm -rf *.tar.gz

if [ ! -d "mshtocgns-$MSHTOCGNS_VERSION" ]; then
    rm -rf mshtocgns-$MSHTOCGNS_VERSION
fi

if [ ! -d "metis-$METIS_VERSION" ]; then
    rm -rf metis-$METIS_VERSION
fi

cd ..
rm -rf Libraries
mkdir Libraries

cd Setup
