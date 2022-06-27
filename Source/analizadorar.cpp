#include "../Headers/analizadorar.h"
#include "../Headers/analizador.h"
#include "../Headers/VariablesGlobales.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>

AnalizadorAr :: AnalizadorAr(){}

string AnalizadorAr::leer(string ruta)
{
    string cadena;
    string texto;
    ifstream archivo;

    archivo.open(ruta, ios::in);
    if (archivo.fail())
    {
        cout << "Error En lectura de Archivo";
    }

    while (!archivo.eof())
    {
        getline(archivo, texto);
        cadena += texto + "\n";
    }

    archivo.close();
    return cadena;
}

void AnalizadorAr::split(string str, char pattern)
{
    char linea[100];
    int posInit = 0;
    int posFound = 0;
    string splitted;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        // comandos.push_back(splitted);
        cout << splitted << endl;
        strcpy(linea, splitted.c_str());
        Analizador *an = new Analizador();
        an->analizar(linea);
        an = new Analizador();
    }
}