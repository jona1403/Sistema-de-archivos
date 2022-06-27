#ifndef REP_H
#define REP_H

#include "string"
//#include "VariablesGlobales.h"
using namespace std;

class rep
{
public:
    rep();
    string name;
    string path;
    string id;
    void controller(rep *disco);
    // Listo
    void repMBR(rep *disco);
    // Listo
    void repDisck(rep *disco);
    void repInode(rep *disco);
    void repJournaling(rep *disco);
    void repBlock(rep *disco);
    void repbmInode(rep *disco);
    void repbmBlock(rep *disco, int type);
    // En ello
    void repTree(rep *disco);
    string TreeRecursivo(int posInodo, string pathfisc, int n, int start, int apuntt);


    //Estos son los metodos para el comando syncronice
    void Syncronice(rep *disco);
    string SyncroniceRecursivo(int posInodo, string pathfisc, int n, int start, int apuntt);


    int cantidadRutas(string path);
    string nombre(string path);
    void repSB(rep *disco);
    void repFile(rep *disco);
    void repls(rep *disco);

private:
    int numNodo;
};

#endif
