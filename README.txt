Ejecución:
    * Para facilitar la obtención de data sin tener que ir cambiado repetidamente el código
    agregué algunas facilidades:

    Ejemplo ejecución: make name=AB101 mode=G

        name: Nombre de la instancia a ejecutar
        mode:
            mode=G , 
                Se generan archivos de lectura .out en la carpeta outGreedy lo cuales contienen las soluciones
                al ejecutar solo búsqueda greedy sobre las instancias
            mode=SAS
                Se generan archivos de lectura .out en la carpeta outSAS lo cuales contienen las soluciones
                al ejecutar Simulated annealing + swap sobre las instancias
            mode=SAO
                Se generan archivos de lectura .out en la carpeta outSA3OPT lo cuales contienen las soluciones
                al ejecutar Simulated annealing + 3-OPT sobre las instancias

    * Para eliminar archivos *.o: make clean
    * Para eliminar archivos *.o y *.out de todas las carpetas: make cleanAll
    * Para eliminar archivos *.o y *.out de la carpeta outGreedy: make cleanG
    * Para eliminar archivos *.o y *.out de la carpeta outSAS: make cleanSAS
    * Para eliminar archivos *.o y *.out de la carpeta outSA3OPT: make cleanSAO

# Observaciones:
* Agregué unos script de bash que permiten generar todas soluciones para su posterior análisis:
    Ejecución: bash loopGvrp.sh
    Ejecución: bash firstLine.sh

* Las carpetas outGreedy, outSAOPT y outSAS deben estar presentes o el programa no guardará los archivos.
    