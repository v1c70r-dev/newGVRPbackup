#ifndef INSTANCIASERVICE_H
#define INSTANCIASERVICE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class InstanciaService{
    public:
        struct restriccionesInstancia{
            string name;
            int nCustomers;
            int nStations;
            float maxTime;//min 
            float maxDistance;//(miles) 
            float speed;//(miles/min) 
            float serviceTime;//(min) 
            float refuelTime;//(min)
        }; 

        struct  Nodo{
            int id;
            string tipo;
            float latitud;
            float longitud;
        };

    public:
        /*
        leerArchivo()
        Parametros: 
            nombreArchivo: Nombre del archivo a leer
        Funcion: Guarda en data la info contenida en el archivo 
        Retorno: -
        */
        void leerArchivo(string nombreArchivo, vector<string> &data){
            fstream instancia;
            instancia.open(nombreArchivo, ios::in);
            if(instancia.is_open()){
                string tp;
                while(getline(instancia, tp)){ 
                    data.push_back(tp);
                }
            }
            instancia.close();
        }

        /*
        * buildRestriccionesInstancia()
        * Parametros: string r : Contiene todos los datos de restricciones de la instancia (name, #customers,etc,...)
        * Función: Almacena los datos de restricciones de la instancia en un struct para su posterior uso
        */
        restriccionesInstancia buildRestriccionesInstancia(string r, restriccionesInstancia& restricciones){
            stringstream ss(r);
            string temp;
            vector<string> data;
            //La data de la estructura de la instancia es almacenada en un vector
            while (ss >> temp){
                data.push_back(temp);
            }
            //Llenado de información en la instancia de tipo EstructuraInstancia
            restricciones.name = data[0];
            restricciones.nCustomers = stoi(data[1]);
            restricciones.nStations = stoi(data[2]);
            restricciones.maxTime = stof(data[3]);
            restricciones.maxDistance = stof(data[4]);
            restricciones.speed = stof(data[5]);
            restricciones.serviceTime = stof(data[6]);
            restricciones.refuelTime = stof(data[7]);  
            return restricciones;  
        }
        
        /*
        * buildNodo()
        * Parametros: string s : 
        * Función:  
        */
        Nodo buildNodo(string s){
            stringstream ss(s);
            string temp;
            vector<string> data;
            //La data del nodo es almacenada en un vector
            while (ss >> temp){
                data.push_back(temp);
            }
            //Construcción del nodo
            Nodo nodo;
            nodo.id = stoi(data[0]);
            nodo.tipo = data[1];
            nodo.latitud = stof(data[2]);
            nodo.longitud = stof(data[3]);
            return nodo;
        }
        
        /*
        * dataNodos
        */
        vector<string> dataNodos(vector<string> dataTot){
            vector<string> data_nodos;
            for(string row:dataTot){
              stringstream ss(row);
              string temp;
              int count = 0;
              while (ss >> temp){
                count ++;
              }
              if(count == 4){
                data_nodos.push_back(row);
              }
            }
            return data_nodos;
        }

        /*
        *buildListaNodos()
        * Parametros: vector<string>& data 
        * Función: Construye las listas de nodos combustible, Fuel y depósito
        */
        void buildListaNodos(vector<string>& data, Nodo&deposito, vector<Nodo>&listaClientes, vector<Nodo>&listaEstaciones){
            vector<Nodo> listaNodos;
            for(string s: dataNodos(data)){
                listaNodos.push_back(buildNodo(s));        
            }
            for(Nodo nodo:listaNodos){
                if(nodo.tipo.compare("d")==0){
                    deposito = nodo;
                }else if(nodo.tipo.compare("f")==0 && nodo.id != 0){
                    listaEstaciones.push_back(nodo);
                }else if(nodo.tipo.compare("c")==0){
                    listaClientes.push_back(nodo);
                }
            }
        }
};
#endif