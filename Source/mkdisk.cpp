#include "../Headers/mkdisk.h"
#include "../Headers/estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

mkdisk :: mkdisk(){}

bool mkdisk :: es_multiplo_8(int numero){
    if(numero%8 == 0){
        return true;
    }
    return false;
}

//Controla las validaciones necesarias para la ejecucion del comando
void mkdisk :: driver(mkdisk *disco){
    disco->path += disco->name;
    particion mbr_partition[4];
    if(!es_multiplo_8(disco->size) || disco->size<=0 || (disco->path.empty())){
        cout<<"Falta de parametros obligatorios: "<<endl;
        if(!es_multiplo_8(disco->size) || disco->size<=0){
            cout<<"     Valor del size no valido: "<<disco->size<<"."<<endl;
        }
        if(disco->path.empty()){
            cout<<"     Ruta no ingresada."<<endl;
        }
        return;
    }

    //Se verifica la existencia del directorio 
    if(!existeruta(split(disco->path))){
        this->createRuta(split(disco->path));
    }

    //Se le asigna un valor predeterminado al fit en caso de no tener
    if(disco->fit.empty()){
        disco->fit = "FF";
    }

    //Se le asigna un valor predeterminado al unit en caso de no tener
    if(disco->unit.empty()){
        disco->unit = "m";
    }
    mkdisk *mkespejo = new mkdisk();
    mkespejo->size = disco->size;
    mkespejo->fit = disco->fit;
    mkespejo->unit = disco->unit;
    mkespejo->name = disco->name;
    mkespejo->path = disco->path.substr(0, disco->path.length()-4)+"_ESPEJO"+".dsk";
    //Aqui se crea el disco principal
    this->create(disco);
    //this->mostrardatos(disco);
    //Aqui se crea el disco espejo
    //disco->path = disco->path.substr(0, disco->path.length()-4)+"_ESPEJO"+".dsk";
    mkespejo->create(mkespejo);
    //this->mostrardatos(mkespejo);


}

void mkdisk ::mostrardatos(mkdisk *disco)
{
    FILE *archivo;
    archivo = fopen(disco->path.c_str(), "rb+");
    //Verificamos que el archivo exista
    if (archivo == NULL)
    {
        cout << "Disco inexistente\n";
        return;
    }
    //Obtenemos el mbr del disco
    fseek(archivo, 0, SEEK_SET);
    mbr mbr;
    fread(&mbr, sizeof(mbr), 1, archivo);

    cout<<"Se creó un disco con las siguientes especificaciones:"<<endl;
    cout<<"tamanio: "<<mbr.mbr_tamano<<endl;
    cout<<"fit: "<<mbr.disk_fit<<endl;
    cout<<"fecha: "<< ctime(&mbr.mbr_fecha_creacion)<<endl;
    cout<<"signature: "<<mbr.mbr_disk_signature<<endl;
    cout<<endl;
    fclose(archivo);
}

//Hace un split de la ruta, ejemplo: /home/documentos/nuevo/disco.dk -> /home/documentos/nuevo 
string mkdisk :: split(string ruta){
    for(int i = ruta.length(); i>=0; i--){
        if(ruta[i] == '/'){
            return ruta.substr(0, i); 
        }
    }
    return "";
}

//Verifica la existencia de directorios
bool mkdisk :: existeruta(string ruta){
  DIR * directorio;
  if (directorio = opendir(ruta.c_str()))
  {
    closedir(directorio);
    return true;             
  }   
  else
  {
   return false;   
  }
}

//Crea un directorio en caso de no existir
void mkdisk ::createRuta(string ruta){

    if (mkdir(ruta.c_str(), 0777) == -1){
        cerr << "Error :  " << strerror(errno) << endl;
    }else{
        cout << "Directorio "<<ruta<<" creado."<<endl;
    }
}

//Crea el disco
void mkdisk :: create(mkdisk *disco){
    FILE *file;
    file = fopen(disco->path.c_str(), "wb");
    char buffer[1024];

    //int size = 0;
    if (strcasecmp(disco->unit.c_str(), "k") == 0)
    {
        for (int i = 0; i < 1024; i++)
        {
            buffer[i] = '\0';
        }

        for (int i = 0; i < disco->size; i++)
        {
            fwrite(&buffer, 1024, 1, file);
        }
        size = size*1024;
    }
    else if (strcasecmp(disco->unit.c_str(), "m") == 0 || disco->unit.empty() == 1)
    {
        for (int i = 0; i < 1024; i++)
        {
            buffer[i] = '\0';
        }
        for (int j = 0; j < (disco->size) * 1024; j++)
        {
            fwrite(&buffer, 1024, 1, file);
        }
        size = size*1024*1024;
    }
    fclose(file);

    mbr mbr_disco;

    int mbr_disk_signature;
    char mbr_disk_fit;
    particion mbr_partitions[4];

    if(strcasecmp(disco->fit.c_str(), "wf") == 0){
        mbr_disco.disk_fit = 'W';
    }else if(strcasecmp(disco->fit.c_str(), "ff") == 0 || disco->fit.empty()){
        mbr_disco.disk_fit = 'F';
    }else if(strcasecmp(disco->unit.c_str(), "bf") == 0){
        mbr_disco.disk_fit = 'B';
    }else{
        return;
    }

    mbr_disco.mbr_fecha_creacion = time(0);
    mbr_disco.mbr_disk_signature = rand();
    mbr_disco.mbr_tamano = size;
    particion vacia;
    vacia.part_status = '0';
    vacia.part_type = '-';
    vacia.part_fit = '-';
    vacia.part_start = -1;
    vacia.part_size = -1;
    vacia.part_name[0] = '\0';

    for(int i = 0; i<4; i++){
        mbr_disco.mbr_partition[i] = vacia;
    }

    file = fopen(disco->path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    fwrite(&mbr_disco, sizeof(mbr), 1, file);
    fclose(file);

    cout<<"El disco fue creado exitosamente."<<disco->path<<endl;
}