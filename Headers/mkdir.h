#ifndef MKDIR_H
# define MKDIR_H
#include "string"
using namespace std;

class mkdir{
    public:
        mkdir();
        string id;
        string path;
        string p;
        void controller(mkdir *dir);
        void create(mkdir *dir);
        int cantidadRutas(string path);
        void Createdir(int posInodo, string path, string pathfisc, int posSB, int n, int start);
        string nombre(string path);
        string auxp(string str, string nm);
        void CreateinRoot(int posInodo, string path, string pathfisc, int posSB, int n, int start);
        //Createdir(Tabla_Inodos inodo, string path, diskmount disco, Super_Bloque superbloque, int n, int start);
};


#endif