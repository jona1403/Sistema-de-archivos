#include <iostream>
#include "mkdisk.h"
#include "rmdisk.h"
#include "fdisk.h"
#include <vector>

extern mkdisk *mmkdisk;
extern rmdisk *rrmkdisk;
extern fdisk *ffdisk;
extern Logeado loger;
extern vector<diskmount> Discos_Montados;

class VariablesGlobales
{
public:
    VariablesGlobales();
};
