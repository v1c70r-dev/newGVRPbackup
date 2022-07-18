#ifndef OUTPUTSERVICE_H
#define OUTPUTSERVICE_H
#include "instanciaService.h"
#include "parametrosService.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class OutputService{
    public:
        /////// estructuras que permitirán escribir en archivo //////
        struct dataSol {
            float combustibleRemanente;
            float distanciaRecorrida;
            float tiempoUsado;
        };

    public:
        /*Las siguientes funcionen tienen como objetivo facilitar la escritura del código, por ende puede ignorarlas*/
        void showListaNodos(vector<InstanciaService::Nodo>lista){
            cout<<"[";
            for(InstanciaService::Nodo n:lista){
                cout<<n.tipo << n.id <<" - ("<< n.latitud << " , "<< n.longitud <<")"<<endl;
            }
            cout<<"]";
        }

        void showRestricciones(InstanciaService::restriccionesInstancia r){
            cout<<"[";
            //Name #Customers #Stations MaxTime(min) MaxDistance(miles) Speed(miles\min) ServiceTime(min) RefuelTime(min)
            cout<< "name: "<<r.name<<endl;
            cout<< "nCustomers: "<<r.nCustomers<<endl;
            cout<< "nStations: "<<r.nStations<<endl;
            cout<< "maxTime(min): "<<r.maxTime<<endl;
            cout<< "maxDistance(miles): "<<r.maxDistance<<endl;
            cout<< "speed(miles/min): "<<r.speed<<endl;
            cout<< "serviceTime(min): "<<r.serviceTime<<endl;
            cout<< "refuelTime(min): "<<r.refuelTime<<endl;
            cout<<"]";
        }

        void showDeposito(InstanciaService::Nodo d){
            cout<<d.id << d.tipo <<" - ("<< d.latitud << " , "<< d.longitud <<")"<<endl;
        }

        void showConjuntoSoluciones(vector<vector<InstanciaService::Nodo>> conjuntoSoluciones,vector<dataSol> conjuntoDataSoluciones){
            dataSol ds;
            for(int i=0; i < conjuntoSoluciones.size(); i++){
                showListaNodos(conjuntoSoluciones[i]);
                ds = conjuntoDataSoluciones[i];
                cout<<"CombustRemanente "<<ds.combustibleRemanente<<endl;
                cout<<"TiempoSol "<<ds.tiempoUsado<<endl;
                cout<<"DistSol "<<ds.distanciaRecorrida<<endl;
            }
        }

        /*
        * contarClientes()
        * Parámetros: Solución  
        * Función: Cuenta el número de clientes de la solución
        * Retorna: #clientes
        */
        int contarClientes(vector<InstanciaService::Nodo>sol){
            int nCli=0;
            //string out = "";
            for(InstanciaService::Nodo nodo:sol){
                //out += nodo.tipo+""+to_string(nodo.id);
                if(nodo.tipo.compare("c")==0){
                    nCli++;
                }
            }
            //cout<<"\n"+out<<endl;
            //cout<<"nCli"<<nCli<<endl;
            return nCli;
        }

        string fun(vector<InstanciaService::Nodo> solucion, float distSol, float tiempoSol, float combRemanente, float combMax){
            //Ruta_vehículo 1 Distancia recorrida Tiempo transcurrido Distancia excedida
            string data = "";
            string ruta = "";
            float combExcedido = 0;
            if(combMax-combRemanente<0){
               combExcedido = -1*(combMax-combRemanente); 
            }else{
                combExcedido = 0;
            }

            for(InstanciaService::Nodo nodo:solucion){
                ruta += nodo.tipo + to_string(nodo.id) + " "; 
            }
            data = ruta+"      "+to_string(distSol)+"       "+to_string(tiempoSol)+"       "+to_string(combExcedido)+"\n";
            return data;
        }

        /*
        * escribirArchivo()
        * Parametros: 
        * Funcion: 
        * Retorno:  
        */
        void escribirArchivo(string nameFile,vector<vector<InstanciaService::Nodo>> conjuntoSoluciones,
        vector<dataSol> conjuntoDataSoluciones, double tiempoEjecucion, float distMax, string mode){
        
            float distanciaTotal = 0;
            int nClientesTotal = 0;
            int n = conjuntoSoluciones.size();
            dataSol ds;
            string data;
            vector<string> dataToFile;

            for(int i=0; i < n; i++){
                ds = conjuntoDataSoluciones[i];
                nClientesTotal += contarClientes(conjuntoSoluciones[i]);
                distanciaTotal += ds.distanciaRecorrida;
                data = fun(conjuntoSoluciones[i], ds.distanciaRecorrida, ds.tiempoUsado, ds.combustibleRemanente, distMax);
                dataToFile.push_back(data);
            }

            string solQuality = to_string(distanciaTotal);
            string nCliServed = to_string(nClientesTotal);
            string nCars = to_string(n);
            string exeTime = to_string(tiempoEjecucion);

            ofstream file;
            if(mode.compare("G")==0){
                file.open("outGreedy/"+nameFile+".out",ios::out);        
            }else if(mode.compare("SAS")==0){
                file.open("outSAS/"+nameFile+".out",ios::out);
            }else if(mode.compare("SAO")==0){
                file.open("outSAO/"+nameFile+".out",ios::out);
            }
            //Calidad de la solución [millas] #Clientes atendidos #Vehiculos Tiempo de ejecución [s]
            file << solQuality+"       "+nCliServed+"       "+nCars+"       "+exeTime+" \n";

            for(string s: dataToFile){
                //Ruta vehículo 1 Distancia recorrida Tiempo transcurrido Distancia excedida
                file<<s;
            }
            file.close();
        }

};

#endif