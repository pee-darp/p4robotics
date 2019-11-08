#!usr/bin/bash
prog=$1
make $prog
git add $prog.c
git commit -m "updated"
git push
../scripts/p4r-submit.sh $prog
