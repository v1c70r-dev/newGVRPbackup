#!/bin/bash
#Greedy
#AB1
for (( iteracion=101; iteracion<=120; iteracion++ ))
do
make name="AB"${iteracion} mode="G" 
wait
done

# #AB2
for (( iteracion=201; iteracion<=220; iteracion++ ))
do
make name="AB"${iteracion} mode="G" 
wait
done

#SA + swap
#AB1
for (( iteracion=101; iteracion<=120; iteracion++ ))
do
make name="AB"${iteracion} mode="SAS" 
wait
done

#AB2
for (( iteracion=201; iteracion<=220; iteracion++ ))
do
make name="AB"${iteracion} mode="SAS"
wait
done

#SA + 2-opt
#AB1
for (( iteracion=101; iteracion<=120; iteracion++ ))
do
make name="AB"${iteracion} mode="SAO" 
wait
done

#AB2
for (( iteracion=201; iteracion<=220; iteracion++ ))
do
make name="AB"${iteracion} mode="SAO" 
wait
done
