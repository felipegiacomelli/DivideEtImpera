#!/bin/bash

rm -rf *.tar.gz

if [ ! -d "mshtocgns-3.0.0" ]; then
    rm -rf mshtocgns-3.0.0
fi

if [ ! -d "metis-5.1.0" ]; then
    rm -rf metis-5.1.0
fi

cd ..
rm -rf Libraries
mkdir Libraries

cd Setup
