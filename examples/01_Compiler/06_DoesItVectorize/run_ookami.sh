#!/bin/bash

env=$PWD/../../env

source $env/ookami/arm.bashrc  
make arm

source $env/ookami/cray.bashrc  
make cray

source $env/ookami/fujitsu.bashrc 
make fujitsu

source $env/ookami/gnu.bashrc
make gcc

./compare.py 0 out.*
