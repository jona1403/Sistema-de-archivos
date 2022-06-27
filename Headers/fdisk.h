#include "string"
#include "estructuras.h"
using namespace std;

class fdisk{
    public:
        fdisk();
        int size;
        string unit;
        string path;
        string type;
        string fit;
        string deleten;
        string name;
        string adden;

        void Controller(fdisk *disco);
        //Declaracion de los metodos de creacion de particiones
        void Create_Logic(fdisk *disco);
        void Create_Primary_Extended(fdisk *disco);
        void Add_Space(fdisk *disco);
        void Remove_Space(fdisk *disco);
        void Delete(fdisk *disco);
        void delete_Fast(fdisk *disco);
        void delete_Full(fdisk *disco);
        int posLogic(string path, string nombre, int inicio);
        void writeCero(string path, int inicio, int tamanio);
        bool EstaVacia(particion mbr_partition[4]);
        bool EstaLLena(particion mbr_partition[4]);
        int ExisteExtendida(particion mbr_partition[4]);
        //Declaracion de los metodos de ajustes
        void Best_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int& fin);
        void Worst_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int& fin);
        void First_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int& fin);
        //void Fit_L(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int& fin);
        //Muestra los datos
        void ShowPartitions(fdisk *disco);
};