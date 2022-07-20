#!/bin/bash

##################################### restricciones ######################################
## echo "AB1"
## touch Resultados/restriccionesAB1.txt
## echo "name   Customers   Stations   MaxTime   MaxDistance    Speed   ServiceTime   RefuelTime" >> Resultados/restriccionesAB1.txt
## for (( counter=101; counter<=120; counter++ ))
## do
## #echo AB$counter
## head -1 Instancias/AB${counter}.dat
## wait
## head -1 Instancias/AB${counter}.dat >> Resultados/restriccionesAB1.txt
## wait
## done
## 
## echo "AB2"
## touch Resultados/restriccionesAB2.txt
## echo "name   Customers   Stations   MaxTime   MaxDistance    Speed   ServiceTime   RefuelTime" >> Resultados/restriccionesAB2.txt
## for (( counter=201; counter<=220; counter++ ))
## do
## #echo AB$counter
## head -1 Instancias/AB${counter}.dat
## wait
## head -1 Instancias/AB${counter}.dat >> Resultados/restriccionesAB2.txt
## wait
## done


############################################## Experimentos 1 2 3 4 ################

rm Resultados/*.txt

# echo "AB1 Greedy"
# touch Resultados/resultadosAB1Greedy.txt
# echo "solQuality       nCliServed       Cars       exeTime" >> Resultados/resultadosAB1Greedy.txt
# for (( counter=101; counter<=120; counter++ ))
# do
# #echo AB$counter
# head -1 outGreedy/AB${counter}0.out
# wait
# head -1 outGreedy/AB${counter}0.out >> Resultados/resultadosAB1Greedy.txt
# wait
# done

# echo "AB2 Greedy"
# touch Resultados/resultadosAB2Greedy.txt
# echo "solQuality       nCliServed       Cars       exeTime" >> Resultados/resultadosAB2Greedy.txt
# for (( counter=201; counter<=220; counter++ ))
# do
# #echo AB$counter
# head -1 outGreedy/AB${counter}0.out
# wait
# head -1 outGreedy/AB${counter}0.out >> Resultados/resultadosAB2Greedy.txt
# wait
# done

# echo "AB1 SAS"
# touch Resultados/resultadosAB1SAS.txt
# echo "solQuality       nCliServed         Cars       exeTime" >> Resultados/resultadosAB1SAS.txt
# for (( counter=201; counter<=220; counter++ ))
# do
# head -1 outSAS/AB${counter}0.out
# wait
# head -1 outSAS/AB${counter}0.out >> Resultados/resultadosAB1SAS.txt
# wait
# done

echo "AB2 SAS"
touch Resultados/resultadosAB2SAS.txt
echo "solQuality       nCliServed         Cars       exeTime" >> Resultados/resultadosAB2SAS.txt
for (( counter=201; counter<=220; counter++ ))
do
head -1 outSAS/AB${counter}.out
wait
head -1 outSAS/AB${counter}.out >> Resultados/resultadosAB2SAS.txt
wait
done

# echo "AB1 SAO"
# touch Resultados/resultadosAB1OPT.txt
# echo "solQuality       nCliServed         Cars       exeTime" >> Resultados/resultadosAB1OPT.txt
# for (( counter=201; counter<=220; counter++ ))
# do
# head -1 outSAOPT/AB${counter}0.out
# wait
# head -1 outSAOPT/AB${counter}0.out >> Resultados/resultadosAB1OPT.txt
# wait
# done

echo "AB2 SAO"
touch Resultados/resultadosAB2OPT.txt
echo "solQuality       nCliServed         Cars       exeTime" >> Resultados/resultadosAB2OPT.txt
for (( counter=201; counter<=220; counter++ ))
do
head -1 outSAOPT/AB${counter}.out
wait
head -1 outSAOPT/AB${counter}.out >> Resultados/resultadosAB2OPT.txt
wait
done

