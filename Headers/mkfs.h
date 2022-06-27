#ifndef MKFS_H
#define MKFS_H

#include "string"
//#include "VariablesGlobales.h"
using namespace std;

class mkfs{
    public:
        mkfs();
        string id;
        string type;
        string fs;
        string addd;
        string unit;
        void formatear(mkfs *disco);
        void controller(mkfs *disco);
};

#endif //MKFS_H