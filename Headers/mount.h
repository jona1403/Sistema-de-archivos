#include <iostream>
#include <string>
//#include "estructuras.h"
#include "VariablesGlobales.h"

using namespace std;
class mount{
    public:
        mount();
        string path;
        string name;
        void Mount_Partition(mount *disco);
        int ExisteExtendida(particion mbr_partition[4]);
        int posLogic(string path, string nombre, int inicio);
        void showMount();
        void Unmount_Partition(string id);
};