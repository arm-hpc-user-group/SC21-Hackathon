#!/bin/bash

ls *.log

echo "Native instructions: memtrace"
python ./postprocess.py `ls memtrace*`

echo ""
echo ""
echo "Emulated instructions: memtrace"
python ./postprocess.py `ls sve*`

python ./merge.py --memtrace -o out.log `ls *.log`
