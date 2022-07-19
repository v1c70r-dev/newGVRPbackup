#include <iostream>
#include "instanciaService.h"
#include "parametrosService.h"
#include "saService.h"
#include "outputService.h"
#include <string>
#include <random>
#include <ctime>
using namespace std;

int main(int argc, char** argv){
    string name = argv[1];//Nombre de la instancia
    string nombreInstancia = name+".dat"; 
    string rutaInstancias = "./Instancias/"; 
    string mode_ = argv[2];
    
    string it_ = argv[3];

    ///////Modo////////
    bool GreedyMode_ = false;
    bool SASMode_ = false;
    bool SAOPTMode_ = false;
    if(mode_.compare("G")==0){GreedyMode_=true;
    }else if(mode_.compare("SAS")==0){SASMode_=true;
    }else if(mode_.compare("SAO")==0){SAOPTMode_=true;}
    ///////////////////

    ///////// Estructuras y variables comunes //////////////
    vector<string> dataArchivo;
    InstanciaService iService;
    parametrosService pService;
    saService sService;
    OutputService oService; 

    InstanciaService::restriccionesInstancia restricciones_;
    InstanciaService::Nodo deposito_;
    vector<InstanciaService::Nodo> listaClientes_;
    vector<InstanciaService::Nodo> listaEstaciones_;
    vector<InstanciaService::Nodo> solucion_;
    vector<vector<InstanciaService::Nodo>> conjuntoSoluciones_;
    OutputService::dataSol dataSolucion_;
    vector<OutputService::dataSol> conjuntoDataSoluciones_;
    double tiempoEjecucion_;
    unsigned t0exe, t1exe;

    //Se construyen las listas de clientes y estaciones, y el nodo depósito. Se definen las restricciones de la instancia
    iService.leerArchivo(rutaInstancias.append(nombreInstancia), dataArchivo);
    iService.buildRestriccionesInstancia(dataArchivo[0], restricciones_);
    iService.buildListaNodos(dataArchivo,deposito_,listaClientes_,listaEstaciones_);

    float tiempoSol_ = 0;
    float distSol_ = 0;
    float combustibleRemanente_ = 0;
    bool flag = true;
    //////////////////////////////////////////////////////////

    ///////////// Variables y estructuras SA ////////////////
    float temperatura_ = 650;
    float alfa_ = 0.9;
    bool flagSAO = false;
    /////////////////////////////////////////////////////////

    ///////////////////////////// Loop principal ////////////////////////////////
    if(GreedyMode_){
        t0exe = clock();
        float borrar = 0;
        while(flag){
            solucion_ = pService.greedy1(solucion_, deposito_, listaClientes_,listaEstaciones_,restricciones_,tiempoSol_, distSol_,combustibleRemanente_);
            flag = sService.factible(solucion_,restricciones_,borrar);
            if(flag){
                //Escribe en el archivo solo si la solución es válida
                conjuntoSoluciones_.push_back(solucion_);
                dataSolucion_.combustibleRemanente = combustibleRemanente_;
                dataSolucion_.distanciaRecorrida = distSol_;
                dataSolucion_.tiempoUsado = tiempoSol_;
                conjuntoDataSoluciones_.push_back(dataSolucion_);
            }
        }
        t1exe = clock();
        tiempoEjecucion_ = (double(t1exe-t0exe)/CLOCKS_PER_SEC);
        oService.escribirArchivo(name, conjuntoSoluciones_, conjuntoDataSoluciones_, tiempoEjecucion_, restricciones_.maxDistance, mode_,it_);
        //oService.showConjuntoSoluciones(conjuntoSoluciones_,conjuntoDataSoluciones_);
        // oService.showDeposito(deposito);
        // oService.showListaNodos(solucion_);
        // oService.showListaNodos(listaClientes_);
        // oService.showListaNodos(listaEstaciones);
        // oService.showRestricciones(restricciones);
    }else if(SASMode_){
        t0exe = clock();
        while(flag){
            solucion_ = pService.greedy1(solucion_, deposito_, listaClientes_,listaEstaciones_,restricciones_,tiempoSol_, distSol_,combustibleRemanente_);
            flag = sService.factible(solucion_,restricciones_,combustibleRemanente_);
            if(flag){
                solucion_ = sService.simulatedAnnealing(temperatura_,alfa_, solucion_,listaClientes_,
                 restricciones_, distSol_, tiempoSol_,combustibleRemanente_, mode_);
                //SA solo retorna soluciones válidas, por lo que se agregan al archivo
                conjuntoSoluciones_.push_back(solucion_);
                dataSolucion_.combustibleRemanente = combustibleRemanente_;
                dataSolucion_.distanciaRecorrida = distSol_;
                dataSolucion_.tiempoUsado = tiempoSol_;
                conjuntoDataSoluciones_.push_back(dataSolucion_); 
            }
        }
        t1exe = clock();
        tiempoEjecucion_ = (double(t1exe-t0exe)/CLOCKS_PER_SEC);
        oService.escribirArchivo(name, conjuntoSoluciones_, conjuntoDataSoluciones_, tiempoEjecucion_, restricciones_.maxDistance, mode_,it_);
    }else if(SAOPTMode_){
        t0exe = clock();
        while(flag){
            solucion_ = pService.greedy1(solucion_, deposito_, listaClientes_,listaEstaciones_,restricciones_,tiempoSol_, distSol_,combustibleRemanente_);
            cout<<"++++++++++++++Greedy+++++++++++++"<<endl;
            flag = sService.factible(solucion_,restricciones_,combustibleRemanente_);
            cout<<"+++++++++++++++++++++++++++++++++"<<endl;
            if(flag){
                solucion_ = sService.simulatedAnnealing(temperatura_,alfa_, solucion_,listaClientes_,
                 restricciones_, distSol_, tiempoSol_,combustibleRemanente_, mode_);
                //SA solo retorna soluciones válidas, por lo que se agregan al archivo
                conjuntoSoluciones_.push_back(solucion_);
                dataSolucion_.combustibleRemanente = combustibleRemanente_;
                dataSolucion_.distanciaRecorrida = distSol_;
                dataSolucion_.tiempoUsado = tiempoSol_;
                conjuntoDataSoluciones_.push_back(dataSolucion_); 
            }
        }
        t1exe = clock();
        tiempoEjecucion_ = (double(t1exe-t0exe)/CLOCKS_PER_SEC);
        oService.escribirArchivo(name, conjuntoSoluciones_, conjuntoDataSoluciones_, tiempoEjecucion_, restricciones_.maxDistance, mode_,it_);
    }else{
        cout<<"Error! Valores válidos para mode: G, SAS, SAO"<<endl;
    }

}