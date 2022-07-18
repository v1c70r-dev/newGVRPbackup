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
        * threeOPT()
        * Parámetros:
        *   Sc
        *   nMaxIteraciones: Define el número máximo de iteraciones. 
        * Función: 
        * Retorna: 
        */
        vector<InstanciaService::Nodo> threeOPT(vector<InstanciaService::Nodo> Sc, int nMaxIteraciones){
            vector<InstanciaService::Nodo>aux;
            /*
                Elegir 3 arcos al azar
                Elegir al azar una de las posibles uniones:
                    Al cortar los 3 arcos siempre se generan 6 cabezas de posible unión => 3 uniones
                    Elegir el orden de unión al azar
                Reescribir la solución 
                    

            */
            return aux;
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
                while(!solValida){
                    if(mode.compare("SAS")==0){
                        Sn = swapNodos(Sc);
                    }else if(mode.compare("SAO")==0){
                        //Sn = threeOPT(Sc);
                    }
                    solValida = factible(Sn,restricciones);
                }
                
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