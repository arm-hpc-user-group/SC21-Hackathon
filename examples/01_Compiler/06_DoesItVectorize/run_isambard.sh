#!/bin/bash

env=$PWD/../../env

source $env/isambard/arm.bashrc  
make arm

source $env/isambard/cray.bashrc  
make cray

source $env/isambard/fujitsu.bashrc 
make fujitsu

source $env/isambard/gnu.bashrc
make gcc

./compare.py 0 out.*
