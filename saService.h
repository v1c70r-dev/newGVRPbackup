#ifndef SASERVICE_H
#define SASERVICE_H
#include "instanciaService.h"
#include "parametrosService.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <string>
//#include <iterator>
#include <algorithm> 
#include <random>
using namespace std;

class saService {

    private:
        // struct nodosArco{//Guarda pares de nodos, ej arco d0-c1
        //     InstanciaService::Nodo nodo0; //do
        //     InstanciaService::Nodo nodo1; //c1
        //     int marcaNodo0 = 0; //0 no marcado, 1 marcado
        //     int marcaNodo1 = 0;
        //     int nuevaPosNodo0 = -1;
        //     int nuevaPosNodo1 = -1;
        // };
    public:
    
        bool factible(vector<InstanciaService::Nodo>&solucion, InstanciaService::restriccionesInstancia restricciones){
            bool solValida = false;
            parametrosService pService;
            float v = restricciones.speed;
            float distAcumulada = 0;
            float tiempoAcumulado = 0;
            float combustibleRemanente = restricciones.maxDistance;
            int n = solucion.size()-1;

            for(int i=0;i<n;i++){
                    distAcumulada += pService.distanciaViaje(solucion[i],solucion[i+1]);
                    combustibleRemanente -= pService.distanciaViaje(solucion[i],solucion[i+1]);
                if(solucion[i+1].tipo.compare("c")==0){
                    tiempoAcumulado += pService.tiempoViaje(solucion[i],solucion[i+1],v) + restricciones.serviceTime;
                }else if(solucion[i+1].tipo.compare("f")==0){
                    tiempoAcumulado += pService.tiempoViaje(solucion[i],solucion[i+1],v) + restricciones.refuelTime;
                    combustibleRemanente = restricciones.maxDistance;
                }else{//depósito final
                    tiempoAcumulado += pService.tiempoViaje(solucion[i],solucion[i+1],v);
                }
            }
            
            if(tiempoAcumulado<=restricciones.maxTime && combustibleRemanente>=0){// || restricciones.maxDistance<distAcumulada)
                solValida=true;
            }else{
                solValida=false;
            }

            //Si todos los nodos componentes son tipo f => sol no factible 
            //(Esto es por el tipo de greedy que estoy haciendo. En este greedy se buscan solo los clientes alcanzables
            // y solo se agregan FS cuando existan restricciones de combustible. Por eso, para este greedy en particular
            // tener solo elementos tipo f es un error (se genera un loop))
            bool soloFs=true;
            for(InstanciaService::Nodo nodo:solucion){
                if(nodo.tipo.compare("c")==0){
                    soloFs=false;
                    break;
                }
            }
            if(soloFs){
                solValida=false;
            }

            //////////////////////////
            //cout<<"(F)tiempo Solucion: "<<tiempoAcumulado << " Tmax: "<<restricciones.maxTime<<endl;
            //cout<<"(F)Dist Solucion: "<<distAcumulada << " Dmax: "<<restricciones.maxDistance<<endl; 
            //cout<<"(F)combustibleRemanente: "<<combustibleRemanente <<endl; 
            //////////////////////////
            return solValida;
        }

        /*
        * tiempoTotalRecorrido()
        * Parámetros: vector solucion
        * Función: calcula el tiempo usado por la solución
        * Retorna: tiempo del recorrido
        */
        float tiempoTotalRecorrido(vector<InstanciaService::Nodo>sol,InstanciaService::restriccionesInstancia restricciones){
            parametrosService ps;
            float t = 0; 
            float v = restricciones.speed;
            //Tiempo viaje + (tiempo recarga|| tiempo atención)
            for(int i=0; i<sol.size()-1 ;i++){
                t+=ps.tiempoViaje(sol[i],sol[i+1],v);
                if(sol[i+1].tipo.compare("c")==0){//tiempo atencion cliente
                    t+=restricciones.serviceTime;
                }else if(sol[i+1].tipo.compare("f")==0){//tiempo recarga
                    t+=restricciones.refuelTime;
                }
            }
            return t;
        }
        /*
        * distanciaTotalRecorrida()
        * Parámetros: vector solucion
        * Función: calcula la distancia recorrida por la solución
        * Retorna: Distancia recorrida
        */
        float distanciaTotalRecorrida(vector<InstanciaService::Nodo>sol){
            float dt = 0;
            parametrosService ps;
            for (int i = 0; i < sol.size()-1; i++){
                dt += ps.distanciaViaje(sol[i],sol[i+1]);
            }
            return dt;
        }

        /*
        * randInt()
        * Parámetros: Rango valores
        * Función: Genera numero aleatorios enteros entre [min,max]
        * Retorna: Número aleatorio
        */
        int randInt(int min, int max){
            random_device seeder;
            mt19937 engine(seeder());
            uniform_int_distribution<int> dist(min, max);
            return dist(engine);
        }
        /*
        * swapNodos()
        * Parámetros: vector de nodos
        * Función: Cambia de posicion 2 nodos
        * Retorna: Vector con los nodos cambiados
        */
        vector<InstanciaService::Nodo> swapNodos(vector<InstanciaService::Nodo> Sc){
            vector<InstanciaService::Nodo>aux;
            int sizeSc = Sc.size();
            int p1 = 0;
            int p2 = 0;

            if(sizeSc==3){return Sc;} //Evita quedarse atascado en el loop de abajo

            while(p1==p2){
                p1 = randInt(1, sizeSc-2);
                p2 = randInt(1, sizeSc-2);
            }

            for (int i = 0; i < sizeSc; i++){
                if(i==p1){
                    aux.push_back(Sc[p2]);
                }else if(i==p2){
                    aux.push_back(Sc[p1]);
                }else{
                    aux.push_back(Sc[i]);
                }
            }
            //aux = Sn_;
            return aux;
        }
        /*
        Función: Revisa si todos los nodos de la lista conjuntoArcCorte están marcados
        Retorna: true cuando están todos marcados, false caso contrario
        */
        // bool todosMarcados(vector<nodosArco> conjuntoArcCorte){
        //     bool allMarcados = true;
        //     for(nodosArco na:conjuntoArcCorte){
        //         if(na.marcaNodo0 == 0 || na.marcaNodo1 == 0){
        //             allMarcados = false;
        //         } 
        //     }
        //     //cout<<"allMarcados: "<<allMarcados<<endl;
        //     return allMarcados;
        // }

        // bool adyacentes(int cut0, int cut1, int cut2, vector<nodosArco> conjuntoArcs){
        //     bool sonAdyacentes = false;
        //     nodosArco arco0 = conjuntoArcs[cut0];
        //     nodosArco arco1 = conjuntoArcs[cut1];
        //     nodosArco arco2 = conjuntoArcs[cut2];
        //     if((arco0.nodo0.id == arco1.nodo0.id && arco0.nodo0.tipo == arco1.nodo0.tipo) 
        //     || (arco0.nodo0.id == arco1.nodo1.id && arco0.nodo0.tipo == arco1.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }else if((arco0.nodo1.id == arco1.nodo0.id && arco0.nodo1.tipo == arco1.nodo0.tipo) 
        //     || (arco0.nodo1.id == arco1.nodo1.id && arco0.nodo1.tipo == arco1.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }else if((arco1.nodo0.id == arco2.nodo0.id && arco1.nodo0.tipo == arco2.nodo0.tipo) 
        //     || (arco1.nodo0.id == arco2.nodo1.id && arco1.nodo0.tipo == arco2.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }else if((arco1.nodo1.id == arco2.nodo0.id && arco1.nodo1.tipo == arco2.nodo0.tipo) 
        //     || (arco1.nodo1.id == arco2.nodo1.id && arco1.nodo1.tipo == arco2.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }else if((arco0.nodo0.id == arco2.nodo0.id && arco0.nodo0.tipo == arco2.nodo0.tipo) 
        //     || (arco0.nodo0.id == arco2.nodo1.id && arco0.nodo0.tipo == arco2.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }else if((arco0.nodo1.id == arco2.nodo0.id && arco0.nodo1.tipo == arco2.nodo0.tipo) 
        //     || (arco0.nodo1.id == arco2.nodo1.id && arco0.nodo1.tipo == arco2.nodo1.tipo)){
        //         sonAdyacentes = true;
        //     }
        //     return sonAdyacentes;
        // }

        // bool arcoYaExistente(vector<nodosArco> conjuntoArcs,vector<nodosArco> conjuntoNuevosArcs){
        //     bool yaExiste = false;
        //     for(nodosArco newArcs: conjuntoNuevosArcs){
        //         for(nodosArco Arcs: conjuntoArcs){
        //             if((newArcs.nodo0.id==Arcs.nodo0.id && newArcs.nodo0.tipo==Arcs.nodo0.tipo) 
        //             ||(newArcs.nodo0.id==Arcs.nodo1.id && newArcs.nodo0.tipo==Arcs.nodo1.tipo)){
        //                 if((newArcs.nodo1.id==Arcs.nodo0.id && newArcs.nodo1.tipo==Arcs.nodo0.tipo) 
        //                 ||(newArcs.nodo1.id==Arcs.nodo1.id && newArcs.nodo1.tipo==Arcs.nodo1.tipo)){
        //                     yaExiste = true;
        //                     break;
        //                 }
        //             }
        //         }
        //     }
        //     return yaExiste;
        // }

        // void showConjuntoArcos(vector<nodosArco> conjArcs){
        //     cout<<"\nConjunto Arcos "<<endl;
        //     for(nodosArco na:conjArcs){
        //         cout<<"["<<na.nodo0.tipo<<""<<na.nodo0.id<<"--"<<na.nodo1.tipo<<""<<na.nodo1.id<<"]"<<endl;
        //         cout<<"nuevaPosNodo0 "<<na.nuevaPosNodo0 <<endl;
        //         cout<<"nuevaPosNodo1 "<<na.nuevaPosNodo1 <<endl;
        //     }
        // }


        vector<InstanciaService::Nodo> reverse(vector<InstanciaService::Nodo>& lista,int position1, int position2){
            vector<InstanciaService::Nodo>aux;
            int j = 0; int k = 0;
            if(position1>position2){
                j = position1;
                k = position2;
            }else{
                j = position2;
                k = position1;
            }

            for(int i = j-1; i>k; i--){
                aux.push_back(lista[i]);
            }

            int c = 0;
            for(int i = 0; i < lista.size(); i++){
                if(i>k && i<j){
                    lista[i] = aux[c];
                    c++;
                }
            }
            return lista;
        }

        /*
        * threeOPT()
        * Parámetros:
        *   Sc
        *   nMaxIteraciones: Define el número máximo de iteraciones. 
        * Función: 
        * Retorna: 
        */
        vector<InstanciaService::Nodo>twoOPT(vector<InstanciaService::Nodo>& Sc){
            vector<InstanciaService::Nodo>newSc;
            InstanciaService::Nodo nodoAux; 
            //Elegir 2 nodos que no sean d0
            int n = Sc.size()-2;
            int pos1 = randInt(1,n);
            int pos2 = randInt(1,n);
            while(pos1 == pos2){
                pos1 = randInt(1,n);
                pos2 = randInt(1,n);
            }
            //Copiar Sc en aux
            for(InstanciaService::Nodo n:Sc){
                newSc.push_back(n);
            }
            //Swap entre ambos nodos
            nodoAux = newSc[pos1];
            newSc[pos1] = newSc[pos2];
            newSc[pos2] = nodoAux;
            //Generar lista reversa entre ambos nodos
            if(abs(pos1-pos2)<3){
                //Equivalente a swap
                return newSc;
            }else{
                newSc = reverse(newSc,pos1,pos2);
            }
            Sc = newSc;
            return newSc;
        }

        /*
        * simulatedAnnealing()
        * Parámetros: 
        * Función: 
        * Retorna: 
        */
        vector<InstanciaService::Nodo> simulatedAnnealing(float T, float alfa, vector<InstanciaService::Nodo> Sc_,
        vector<InstanciaService::Nodo>& listaClientes, InstanciaService::restriccionesInstancia restricciones,
        float& distanciaSolucion, float& tiempoSolucion, string mode){

            parametrosService pService;
            vector<InstanciaService::Nodo>Sc = Sc_;
            vector<InstanciaService::Nodo>Sn;
            vector<InstanciaService::Nodo>Sbest;
            float dEval=0; float distSn=0; float distSc=0; float distSbest=0;
            float e_eval = 0;
            float T_termino = 1;//Criterio de término 
            Sbest = Sc;
            bool solValida = false;

            while(T > T_termino){
                // while(!solValida){
                //     if(mode.compare("SAS")==0){
                //         Sn = swapNodos(Sc);
                //     }else if(mode.compare("SAO")==0){
                //         Sn = twoOPT(Sc);
                //     }
                //     solValida = factible(Sn,restricciones);
                //     cout<<"solValida "<<solValida<<endl;
                // }

                ////////////////SA acepta sol infactibles///////////
                
                if(mode.compare("SAS")==0){
                    Sn = swapNodos(Sc);
                }else if(mode.compare("SAO")==0){
                    Sn = twoOPT(Sc);
                }
                solValida = factible(Sn,restricciones);
                cout<<"solValida "<<solValida<<endl;
                
                ////////////////
                
                distSn = distanciaTotalRecorrida(Sn);
                distSc = distanciaTotalRecorrida(Sc);
                distSbest = distanciaTotalRecorrida(Sbest);
                dEval = -abs(distSn-distSc);
                e_eval = exp(dEval/T);
                //Sn es mejor que Sc
                if(distSn < distSc){
                    Sc = Sn;
                    distSc = distanciaTotalRecorrida(Sc);
                }else if( ((float) rand()/RAND_MAX) < e_eval){
                    //Es posible aceptar soluciones de peor calidad para explorar
                    Sc = Sn;
                    distSc = distanciaTotalRecorrida(Sc);
                }
                //Sc es mejor que Sbest 
                if(distSc < distSbest){
                    Sbest = Sc;
                }
                T *= alfa;
            }        
            tiempoSolucion = tiempoTotalRecorrido(Sbest,restricciones);//tiempo de Sbest 
            distanciaSolucion = distSbest;
            listaClientes = pService.setClientesVisitables(Sbest,listaClientes);
            return Sbest;
        }

};

#endif