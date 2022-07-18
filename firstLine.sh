#!/bin/bash

echo "AB1"
for (( counter=101; counter<=120; counter++ ))
do
#echo AB$counter
head -1 AB$counter.out
wait
done

echo "AB2"
#AB2
for (( counter=201; counter<=220; counter++ ))
do
#echo AB$counter
head -1 AB$counter.out
wait
done