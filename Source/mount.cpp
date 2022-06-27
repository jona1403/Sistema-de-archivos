#include "../Headers/mount.h"

mount ::mount() {}

int mount::ExisteExtendida(particion mbr_partition[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (mbr_partition[i].part_type == 'E' || mbr_partition[i].part_type == 'e')
        {
            return i;
        }
    }
    return -1;
}

int mount ::posLogic(string path, string nombre, int inicio)
{
    FILE *file;
    file = fopen(path.c_str(), "rb+");
    fseek(file, inicio, SEEK_SET);
    ebr _ebr;
    fread(&_ebr, sizeof(ebr), 1, file);
    while (_ebr.part_next > 0)
    {
        if (_ebr.part_name == nombre)
        {
            fclose(file);
            return _ebr.part_start;
        }
        fseek(file, _ebr.part_next, SEEK_SET);
        fread(&_ebr, sizeof(ebr), 1, file);
    }
    fclose(file);
    return -1;
}

void mount::Mount_Partition(mount *disco)
{
    munt moAux;
    diskmount disco_montar;
    bool disco_ya_montado = false;
    bool particion_ya_montada = false;
    int index = 0;
    // Se verifica si el disco ya esta montado
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        if (Discos_Montados[i].disk_path == disco->path)
        {
            disco_ya_montado = true;
            index = i;
            break;
        }
    }

    if (!disco_ya_montado)
    {
        disco_montar.disk_path = disco->path;
        disco_montar.disk_letra = (char)(64 + 1 + Discos_Montados.size());
        disco_montar.disk_id = "vd";
    }
    else
    {
        disco_montar = Discos_Montados[index];
    }

    // Se verifica si la particion ya esta montada
    for (int i = 0; i < disco_montar.particiones.size(); i++)
    {
        if (disco_montar.particiones[i].mount_name == disco->name)
        {
            particion_ya_montada = true;
            cout << "Partición ya montada" << endl;
            return;
        }
    }

    moAux.mount_id = "vd" + disco_montar.disk_letra + to_string(disco_montar.particiones.size()+1);
    moAux.mount_name = disco->name;
    moAux.mount_path = disco->path;


    FILE *file;
    file = fopen(disco->path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    mbr _mbr;
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    int indexPart = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_name == disco->name)
        {
            indexPart = i;
            break;
        }
    }

    if (indexPart >= 0)
    {
        _mbr.mbr_partition[indexPart].part_status = '1';
        file = fopen(disco->path.c_str(), "rb+");
        fseek(file, 0, SEEK_SET);
        fwrite(&_mbr, sizeof(mbr), 1, file);
        fclose(file);
        moAux.mount_type = 'P';
        moAux.mount_part = _mbr.mbr_partition[indexPart];
        disco_montar.particiones.push_back(moAux);
    }
    else
    {
        int posEx = ExisteExtendida(_mbr.mbr_partition);
        if (posEx < 0)
        {
            cout << "No existe la particion buscada" << endl;
            return;
        }
        int pos = posLogic(disco->path, disco->name, _mbr.mbr_partition[posEx].part_start);
        if (pos < 0)
        {
            cout << "No existe la particion buscada L" << endl;
            return;
        }
        file = fopen(disco->path.c_str(), "rb+");
        fseek(file, pos, SEEK_SET);
        ebr ebr1;
        fread(&ebr1, sizeof(ebr), 1, file);
        ebr1.part_status ='1';
        fseek(file, pos, SEEK_SET);
        fwrite(&ebr1, sizeof(ebr), 1, file);
        fclose(file);
        moAux.mount_type = 'L';
        moAux.mount_logic = ebr1;
        disco_montar.particiones.push_back(moAux);
    }
    if (!disco_ya_montado)
    {
        Discos_Montados.push_back(disco_montar);
    }
    else
    {
        Discos_Montados[index] = disco_montar;
    }
}

void mount ::showMount()
{
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            cout << Discos_Montados[i].particiones[j].mount_id << endl;
        }
    }
}

void mount::Unmount_Partition(string id)
{
    int index1 = 0, index2 = 0;
    bool encontrado = false;
    vector<munt> aux;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == id)
            {
                index1 = i;
                index2 = j;
                encontrado = true;
                break;
            }
        }
        if (encontrado)
        {
            break;
        }
    }

    for (int i = 0; i < Discos_Montados[index1].particiones.size(); i++)
    {
        if (i != index2)
        {
            aux.push_back(Discos_Montados[index1].particiones[i]);
        }
    }
    Discos_Montados[index1].particiones = aux;
}