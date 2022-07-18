#ifndef PARAMETROSSERVICE_H
#define PARAMETROSSERVICE_H
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
// #include <iterator>
// #include <fstream>
// #include <algorithm> 
#include "instanciaService.h"
using namespace std;

class parametrosService{
    public:
        /*
        * distanciaViaje()
        * Parámetros: Dos Nodos - InstanciaService::Nodo
        * Función: Calcula la distancia de Haversine entre dos nodos  
        * Retorno: Distancia de Haversine - float
        */
        float distanciaViaje(InstanciaService::Nodo n1, InstanciaService::Nodo n2){
            const static double radioTierra = 3985.8;//millas
            //Convertir lat y long a radianes
            float lat_n1 = n1.latitud / 57.29578; //n1.latitud * (pi/180)
            float lat_n2 = n2.latitud / 57.29578;
            float long_n1 = n1.longitud / 57.29578;
            float long_n2 = n2.longitud / 57.29578;

            //Deltas
            float dLat = lat_n2 - lat_n1;
            float dLong = long_n2 - long_n1;                

            //Fórmula de Harvesine
            float a = sin(dLat/2) * sin(dLat/2) + cos(lat_n1) *cos(lat_n2) * sin(dLong/2) * sin(dLong/2);
            float c = 2 * atan2(sqrt(a), sqrt(1-a));       
            float dist = radioTierra * c;

            return dist;                 
        }

        float tiempoViaje(InstanciaService::Nodo n1, InstanciaService::Nodo n2, float velocidad){
            return distanciaViaje(n1,n2)/velocidad;
        }
        
        InstanciaService::Nodo nodoMasCercano(vector<InstanciaService::Nodo> lista, InstanciaService::Nodo nodoActual_){        
            InstanciaService::Nodo nodoMasCercano;
            float distMin = 10000;//millas
            float distaux = 0;
            for(InstanciaService::Nodo nodo:lista){
                distaux = distanciaViaje(nodo,nodoActual_);
                if(distaux < distMin){
                    nodoMasCercano = nodo;
                    distMin = distaux; 
                } 
            }
            return nodoMasCercano;
        } 

        /*
        * setClientesVisitables()
        * Parámetros: Recibe la solución parcial Sc
        * Función: Modifica la lista de clientes visitables. Si un nodo visitable ya fue visitado, se retira de la lista
        * Retorna: true si la lista fue modificada   
        */
        vector<InstanciaService::Nodo> setClientesVisitables(vector<InstanciaService::Nodo>solParcial,
        vector<InstanciaService::Nodo> cliVisitables){
            // Idea: cliVisitables = cliVistables\solParcial
            vector<InstanciaService::Nodo> aux;
            bool yaVisitado = false;
            int sps = solParcial.size();
            int cvs = cliVisitables.size();
            for(int i=0; i<cvs; i++){
                if(cliVisitables[i].id==0){
                    aux.push_back(cliVisitables[i]);
                }else{
                    for(int j=0; j<sps; j++){
                        if(solParcial[j].id==cliVisitables[i].id && solParcial[j].tipo.compare(cliVisitables[i].tipo)==0){
                            //Si cliente ya fue visitado (pertenece a la solución parcial), no agregarlo a la lista de cliVisitables
                            yaVisitado = true;
                            break;
                        }
                    }
                    if(!yaVisitado){
                        aux.push_back(cliVisitables[i]);
                    }
                    yaVisitado=false;
                }
            }
            return aux;
        }
        
        /*
        greedy1
        Busca el nodo cliente más cercano y recarga combustible solo cuando es necesario => Prioriza soluciones con un mayor número de nodos clientes,
        pero las soluciones entregadas son menos heterogeneas (en términos del tipo de composición) 
        */
        vector<InstanciaService::Nodo> greedy1(vector<InstanciaService::Nodo>& solucion, InstanciaService::Nodo deposito,
        vector<InstanciaService::Nodo>& listaClientes, vector<InstanciaService::Nodo>& listaEstaciones,
        InstanciaService::restriccionesInstancia restricciones,float& tiempoSol, float& distSol, float& combustRemanente){
            
            InstanciaService::Nodo nodoAnterior;
            InstanciaService::Nodo nodoActual;
            InstanciaService::Nodo nodoNuevo;
            InstanciaService::Nodo nodoNuevoRecarga;
            float combustibleRemanente = restricciones.maxDistance;
            float tiempoRemanente = restricciones.maxTime;
            float const v = restricciones.speed;
            bool flag = true;
            float dc = 0; float tc = 0; float df = 0; float tf = 0;
            vector<InstanciaService::Nodo>empty;
            solucion = empty;
            nodoAnterior.id = -1; nodoAnterior.tipo = "none"; 
            nodoActual = deposito;//Vehículo comienza en el depósito
            solucion.push_back(deposito);
            distSol = 0;
            while(flag){ 
                nodoNuevo = nodoMasCercano(listaClientes,nodoActual);
                //Si agregando el nuevo nodo a la solucion se produce un subciclo, no se agrega y el vehiculo vuelve al deposito
                if((nodoAnterior.id == nodoNuevo.id && nodoAnterior.tipo.compare(nodoNuevo.tipo)==0) 
                || (nodoActual.id == nodoNuevo.id && nodoActual.tipo.compare(nodoNuevo.tipo)==0)){
                    solucion.push_back(deposito);
                    distSol += distanciaViaje(nodoActual,deposito);
                    combustibleRemanente -= distanciaViaje(nodoActual,deposito);
                    tiempoRemanente -= tiempoViaje(nodoActual,deposito,v);
                    nodoAnterior = nodoActual;
                    nodoActual = deposito;
                    flag = false;
                    // cout<<"Termina en subciclo"<<endl;
                }else{
                    //Verificar si es posible volver al depósito desde el nodoNuevo
                    dc = combustibleRemanente -(distanciaViaje(nodoActual,nodoNuevo)+distanciaViaje(nodoNuevo,deposito));
                    tc = tiempoRemanente-(tiempoViaje(nodoActual,nodoNuevo,v)+tiempoViaje(nodoNuevo,deposito,v)+restricciones.serviceTime);
                    if(dc>=0 && tc>=0){
                        //vehiculo visita el nodoNuevo => se actualiza solucion,listaClientes, timeMax, distMax
                        solucion.push_back(nodoNuevo);
                        distSol += distanciaViaje(nodoActual,nodoNuevo);
                        combustibleRemanente -= distanciaViaje(nodoActual,nodoNuevo);
                        tiempoRemanente -= tiempoViaje(nodoActual,nodoNuevo,v)+restricciones.serviceTime;
                        listaClientes = setClientesVisitables(solucion,listaClientes);
                        nodoAnterior = nodoActual;
                        nodoActual = nodoNuevo;
                    }else{
                        if(dc<0 && tc>=0){
                            //Si el problema es el combustible, ver si es posible ir a una estación, recargar, y luego volver al depósito
                            nodoNuevoRecarga = nodoMasCercano(listaEstaciones,nodoActual);
                            df = restricciones.maxDistance - (distanciaViaje(nodoActual,nodoNuevoRecarga)+distanciaViaje(nodoNuevoRecarga,deposito));
                            tf = tiempoRemanente - (tiempoViaje(nodoActual,nodoNuevoRecarga,v)+tiempoViaje(nodoNuevoRecarga,deposito,v)+restricciones.refuelTime);
                            if(df>=0 && tf>=0){
                                //vehiculo visita el nodoNuevoRecarga => se actualiza solucion, timeMax, distMax
                                if((nodoAnterior.id == nodoNuevoRecarga.id && nodoAnterior.tipo.compare(nodoNuevoRecarga.tipo)==0) 
                                || (nodoActual.id == nodoNuevoRecarga.id && nodoActual.tipo.compare(nodoNuevoRecarga.tipo)==0)){
                                    //Se genera un subciclo entre estaciones, volver al depósito y terminar el recorrido                           
                                    solucion.push_back(deposito);
                                    distSol += distanciaViaje(nodoActual,deposito);
                                    combustibleRemanente -= distanciaViaje(nodoActual,deposito);
                                    tiempoRemanente -= tiempoViaje(nodoActual,deposito,v);
                                    nodoAnterior = nodoActual;
                                    nodoActual = deposito;
                                    flag = false;
                                    //cout<<"Termina en subciclo estaciones"<<endl;
                                }else{
                                    //Se recarga combustible
                                    solucion.push_back(nodoNuevoRecarga);
                                    distSol += distanciaViaje(nodoActual,nodoNuevoRecarga);
                                    //combustibleRemanente -= distanciaViaje(nodoActual,nodoNuevoRecarga);
                                    combustibleRemanente = restricciones.maxDistance;
                                    tiempoRemanente -= (tiempoViaje(nodoActual,nodoNuevoRecarga,v)+restricciones.refuelTime);
                                    nodoAnterior = nodoActual;
                                    nodoActual = nodoNuevoRecarga; 
                                }
                            }else{
                                //Si el problema es el tiempo,volver al deposito y terminar el recorrido
                                solucion.push_back(deposito);
                                distSol += distanciaViaje(nodoActual,deposito);
                                combustibleRemanente -= distanciaViaje(nodoActual,deposito);
                                tiempoRemanente -= tiempoViaje(nodoActual,deposito,v);
                                nodoAnterior = nodoActual;
                                nodoActual = deposito;
                                flag = false;
                                //cout<<"Termina porq no es posible ir a una estacion"<<endl;
                            }
                        }else {
                            //Si el problema es el tiempo, volver al depósito y terminar el recorrido
                            //cout<<"\ntimeMax "<<tiempoRemanente<<" tiempoViaje(nodoActual,deposito,v) "<<tiempoViaje(nodoActual,deposito,v)<<endl;                           
                            solucion.push_back(deposito);
                            distSol += distanciaViaje(nodoActual,deposito);
                            combustibleRemanente -= distanciaViaje(nodoActual,deposito);
                            tiempoRemanente -= tiempoViaje(nodoActual,deposito,v);
                            nodoAnterior = nodoActual;
                            nodoActual = deposito;
                            flag = false;
                            //cout<<"Termina por falta de tiempo"<<endl;
                        }
                    } 
                }

            } 
            combustRemanente = combustibleRemanente; 
            tiempoSol = restricciones.maxTime - tiempoRemanente;

            // cout<<"(G)combustRemanente "<<combustibleRemanente<<endl;
            // cout<<"(G)tiempoSol "<<tiempoSol<<endl;
            // cout<<"(G) distSol"<<distSol<<endl;
            return solucion;
        }

        /*
        greedy2
        Busca el nodo más cercano (independiente del tipo) => Genera soluciones iniciales más heterogeneas
        */
        void greedy2(vector<InstanciaService::Nodo>& solucion, InstanciaService::Nodo deposito,
        vector<InstanciaService::Nodo>& listaClientes, vector<InstanciaService::Nodo>& listaEstaciones,
        InstanciaService::restriccionesInstancia restricciones,float& tiempoSol, float& distSol){
            
            InstanciaService::Nodo nodoAnterior;
            InstanciaService::Nodo nodoActual;
            InstanciaService::Nodo nodoNuevo;
            InstanciaService::Nodo nodoNuevoRecarga;
            float distMax = restricciones.maxDistance;
            float timeMax = restricciones.maxTime;
            float const v = restricciones.speed;
            bool flag = true;
            float dc = 0; float tc = 0; float df = 0; float tf = 0;
            vector<InstanciaService::Nodo>empty;
            solucion = empty;
            nodoAnterior.id=-1; nodoAnterior.tipo="none";
            nodoActual = deposito;//Vehículo comienza en el depósito
            solucion.push_back(deposito);

            float dMinC = 0;
            float dMinF = 0;
            InstanciaService::Nodo nodoAuxC;
            InstanciaService::Nodo nodoAuxF;
            while(flag){
                nodoAuxC = nodoMasCercano(listaClientes,nodoActual);
                nodoAuxF = nodoMasCercano(listaEstaciones,nodoActual);
                dMinC = distanciaViaje(nodoActual,nodoAuxC);
                dMinF = distanciaViaje(nodoActual,nodoAuxF);
                if(dMinC <= dMinF){
                    nodoNuevo = nodoAuxC;
                }else{
                    nodoNuevo = nodoAuxF;
                }
            } 
        }
};

#endif