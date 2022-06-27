#ifndef ANALIZADOR_H
# define ANALIZADOR_H

  
#include "string"
#include <vector>
using namespace std;

class Analizador{
    public:
        Analizador();
        void comillaspath();
        void analizar(char *comando);
        void analizarparams(char *comando);
        void analizarparamsInd(char *comando);
        void limpiarParams();
        void analizarparamsmkd(char *comando2);
        void analizarparamsrmd(char *comando2);
        void analizarparamsexe(char *comando2);
        string leer(string ruta);
        void split(string str, char pattern);

};
#endif