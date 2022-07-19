#!/bin/bash
#NOMBRE = "AB220"
#MODO = "SAO"

for (( iteracion=1; iteracion<=20; iteracion++ ))
do
make name="AB220" mode="SAS" it=${iteracion}
wait
done

#iteracion = ""
#make name=$(nombre) mode=$(modo) it= $(iteracion)
#wait