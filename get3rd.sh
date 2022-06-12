#!/bin/bash 
# Author: Roman Dobias
# Purpose: Clone all necessary 3rd party libraries to '3rd' directory
#          compile & install into '3rd/install' and generate CMake conf
#          in build 

mkdir -p 3rd
mkdir -p 3rd/install
mkdir -p build

INSTALL_DIR=`realpath 3rd/install`
echo "Install dir: ${INSTALL_DIR}"

function prepareRepo {
    REPO_NAME=$1
    REPO_URL=$2
    REPO_ADDITIONAL_CMAKE_DEFINITIONS=$3
    START_PATH=`pwd`

    echo "${REPO_NAME} : ${REPO_URL}"
    cd 3rd
    git clone ${REPO_URL} ${REPO_NAME}
    cd ${REPO_NAME}
    mkdir -p build && cd build
    cmake ../ -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} ${REPO_ADDITIONAL_CMAKE_DEFINITIONS} 
    make -j4
    make install
    cd ${START_PATH}
}

###############################################################################
# I. Clone deps
###############################################################################
#prepareRepo "imgui" "https://github.com/ocornut/imgui" 
prepareRepo "geGL" "https://github.com/dormon/geGL/" 

##############################################################################
# III. Generate configuration
###############################################################################

cd build
CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${INSTALL_DIR} cmake ../


