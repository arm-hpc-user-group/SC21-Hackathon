#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "Usage: $0 binary [binary...]"
    exit -1
fi

while [[ $# -gt 0 ]] ; do
  (armllvm-objdump -D -mattr=+sve $1 > $1.dis && echo "$1.dis generated") || echo "Disassemble error"
  shift
done
