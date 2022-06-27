#include "../Headers/rmdisk.h"
#include "../Headers/mkdisk.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;
rmdisk ::rmdisk() {}

// Controlador del comando, verifica los parametros obligatorios y la existencia del directorio y disco
void rmdisk::driver(rmdisk *disco)
{

    if (disco->path.empty())
    {
        cout << "Falta de parametros obligatorios, ruta no ingresada." << endl;
    }
    FILE *archivo;
    archivo = fopen(disco->path.c_str(), "rb+");
    // Verificamos que el archivo exista
    if (archivo == NULL)
    {
        archivo = fopen((disco->path.substr(0, disco->path.length() - 4) + "_ESPEJO" + ".dsk").c_str(), "rb+");
        if (archivo == NULL)
        {
            cout << "Disco inexistente\n";
            return;
        }else{
            disco->path = disco->path.substr(0, disco->path.length() - 4) + "_ESPEJO" + ".dsk";
        }

    }
    cout<<"delete: "<< disco->path<<endl;

    _delete(disco);
}

// Elimina el disco
void rmdisk::_delete(rmdisk *disco)
{
    string entrada;
    cout << "¿Esta seguro de eliminar el disco? y/N: ";
    getline(cin, entrada);
    if (entrada == "y" || entrada == "Y")
    {
        remove(disco->path.c_str());
        cout << "disco eliminado exitosamente\n";
    }
    else if (entrada == "n" || entrada == "N")
    {
        cout << "disco no eliminado\n";
    }
    else
    {
        cout << "Entrada invalida\n";
    }
}