#ifndef MKDISK_H
# define MKDISK_H
#include "string"

using namespace std;

class mkdisk{
    public:
        mkdisk();
        int size;
        string fit;
        string unit;
        string path;
        string name;
        void driver(mkdisk *disco);
        void create(mkdisk *disco);
        void createRuta(string ruta);
        void mostrardatos(mkdisk *disco);
        bool existeruta(string ruta);
        bool es_multiplo_8(int numero);
        string split(string ruta);
};
#endif