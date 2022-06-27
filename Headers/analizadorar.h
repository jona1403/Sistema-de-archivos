#ifndef ANALIZADORAR_H
# define ANALIZADORAR_H

  
#include "string"
using namespace std;

class AnalizadorAr{
    public:
        AnalizadorAr();
        string leer(string ruta);
        void split(string str, char pattern);

};
#endif