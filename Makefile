start: main
	./main $(name) $(mode)

main: main.o 
	g++ main.o -o main

main.o: main.cpp instanciaService parametrosService saService outputService
	g++ -g -c main.cpp

outputService: outputService.o instanciaService parametrosService
	g++ -g -c outputService.cpp

saService: saService.o instanciaService parametrosService
	g++ -g -c saService.cpp

parametrosService: parametrosService.o parametrosService.h instanciaService
	g++ -g -c parametrosService.cpp

instanciaService: instanciaService.o instanciaService.h
	g++ -g -c instanciaService.cpp

clean:
	rm *.o 

cleanG:
	rm *.o outGreedy/*.out 

cleanSAS:
	rm *.o outSAS/*.out

cleanSAO:
	rm *.o outSAOPT/*.out

cleanAll:
	rm *.o outSAS/*.out outGreedy/*.out outSAOPT/*.out Resultados/*.txt
#target:dependencies
#	actions