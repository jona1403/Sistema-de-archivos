#ifndef MKFILE_H
# define MKFILE_H
#include "string"

using namespace std;

class mkfile{
    public:
        mkfile();
        string id;
        string path;
        string p;
        int size;
        string cont;
        string nombre(string path);
        int cantidadRutas(string path);
        void controller(mkfile *doc);
        void create(mkfile *dir);
        void createRecursivo(int posInodo, string path, string pathfisc, int posSB, int n, int start, string contenido);
};
#endif