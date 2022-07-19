#!/bin/bash

echo "AB220 OPT"
for (( counter=1; counter<=20; counter++ ))
do
#echo AB$counter
head -1 outSAOPT/AB220$counter.out
wait
done
