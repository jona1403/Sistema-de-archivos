
#include "../Headers/fdisk.h"
#include <iostream>
#include <string>
#include <cstring>
#include "../Headers/mkdisk.h"

using namespace std;

fdisk ::fdisk() {}

// Probar
int fdisk::ExisteExtendida(particion mbr_partition[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (mbr_partition[i].part_type == 'E' || mbr_partition[i].part_type == 'e')
        {
            if (mbr_partition[i].part_size > 0)
            {
                return i;
            }
        }
    }
    return -1;
}

// Probar(Se usa en los delete)
int fdisk ::posLogic(string path, string nombre, int inicio)
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

// Revisar
void fdisk ::ShowPartitions(fdisk *disco)
{
    cout << "ENTRA0" << endl;
    FILE *archivo;
    archivo = fopen(disco->path.c_str(), "rb+");
    // Verificamos que el archivo exista
    if (archivo == NULL)
    {
        cout << "Disco inexistente"<<disco->path<<endl;
        return;
    }
    // Obtenemos el mbr del disco
    fseek(archivo, 0, SEEK_SET);
    mbr mbr;
    fread(&mbr, sizeof(mbr), 1, archivo);

    for (int i = 0; i < 4; i++)
    {
        if (true)
        {
            cout << "Nombre: " << mbr.mbr_partition[i].part_name << endl;
            cout << "Tamanio: " << mbr.mbr_partition[i].part_size << endl;
            cout << "Fit: " << mbr.mbr_partition[i].part_fit << endl;
            cout << "Start: " << mbr.mbr_partition[i].part_start << endl;
            cout << "Tipo: " << mbr.mbr_partition[i].part_type << endl;
            if (mbr.mbr_partition[i].part_type == 'E')
            {
                cout << "Es Extendida" << endl;
                FILE *file2;
                file2 = fopen(disco->path.c_str(), "rb+");
                cout << "Aqui se leyo:" << mbr.mbr_partition[i].part_start << endl;
                fseek(file2, mbr.mbr_partition[i].part_start, SEEK_SET);
                ebr _ebr;
                fread(&_ebr, sizeof(_ebr), 1, file2);
                fclose(file2);
                cout << _ebr.part_next << endl;
                cout << _ebr.part_fit << endl;
                cout << _ebr.part_name << endl;
                cout << _ebr.part_size << endl;
                cout << _ebr.part_start << endl;
            }
        }
    }
    fclose(archivo);
}

// Iniciarlo
void fdisk ::Add_Space(fdisk *disco) {}
// Iniciarlo
void fdisk ::Remove_Space(fdisk *disco) {}

// Correcto
void fdisk ::Delete(fdisk *disco)
{
    if (strcasecmp(disco->deleten.c_str(), "fast") == 0)
    {
        delete_Fast(disco);
    }
    else
    {
        delete_Full(disco);
    }
}

// Probar
void fdisk ::delete_Fast(fdisk *disco)
{
    int index = -1;
    FILE *file;
    file = fopen(disco->path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_name == disco->name)
        {
            _mbr.mbr_partition[i].part_size = -1;
            fseek(file, 0, SEEK_SET);
            fwrite(&_mbr, sizeof(mbr), 1, file);
            fclose(file);
            return;
        }
    }

    index = ExisteExtendida(_mbr.mbr_partition);
    if (index >= 0)
    {
        int pos = posLogic(disco->path, disco->name, _mbr.mbr_partition[index].part_start);
        if (pos <= 0)
        {
            cout << "No existe la particion" << endl;
        }
        else
        {
            ebr _ebr;
            fseek(file, pos, SEEK_SET);
            fread(&_ebr, sizeof(ebr), 1, file);
            _ebr.part_size = -1;
            fseek(file, pos, SEEK_SET);
            fwrite(&_ebr, sizeof(ebr), 1, file);
        }
    }
    else
    {
        cout << "No existe la particion" << endl;
    }
    fclose(file);
}

// Probar
void fdisk ::writeCero(string path, int inicio, int tamanio)
{

    FILE *file;
    file = fopen(path.c_str(), "wb");
    char buffer[1024];
    fseek(file, inicio, SEEK_SET);

    for (int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }

    for (int i = 0; i < (tamanio / 1024); i++)
    {
        fwrite(&buffer, 1024, 1, file);
    }

    fclose(file);
}

// Probar
void fdisk ::delete_Full(fdisk *disco)
{
    int index = -1;
    FILE *file;
    file = fopen(disco->path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_name == disco->name)
        {

            writeCero(disco->path, _mbr.mbr_partition[i].part_start, _mbr.mbr_partition[i].part_size);

            _mbr.mbr_partition[i].part_status = '0';
            _mbr.mbr_partition[i].part_fit = '-';
            _mbr.mbr_partition[i].part_type = '-';
            _mbr.mbr_partition[i].part_start = -1;
            _mbr.mbr_partition[i].part_size = -1;
            _mbr.mbr_partition[i].part_name[0] = '\0';
            fseek(file, 0, SEEK_SET);
            fwrite(&_mbr, sizeof(mbr), 1, file);
            fclose(file);
            return;
        }
    }

    index = ExisteExtendida(_mbr.mbr_partition);
    if (index >= 0)
    {
        int pos = posLogic(disco->path, disco->name, _mbr.mbr_partition[index].part_start);
        if (pos < 0)
        {
            cout << "No existe la particion" << endl;
        }
        else
        {
            ebr _ebr;
            fseek(file, pos, SEEK_SET);
            fread(&_ebr, sizeof(ebr), 1, file);
            writeCero(disco->path, _ebr.part_start, _ebr.part_size);
            _ebr.part_size = -1;
            fseek(file, pos, SEEK_SET);
            fwrite(&_ebr, sizeof(ebr), 1, file);
        }
    }
    else
    {
        cout << "No existe la particion" << endl;
    }
    fclose(file);
}

// Revisar
// Agregar validaciones de nombre repetido y de path existente, deleten y adden
void fdisk::Controller(fdisk *disco)
{

    if (!disco->deleten.empty())
    {
        Delete(disco);
    }
    else if (!disco->adden.empty())
    {
    }
    else
    {
        if (disco->size <= 0 || disco->path.empty() || disco->name.empty())
        {
            cout << "Falta de parametros obligatorios para la creacion de una particion: " << endl;
            if (disco->size <= 0)
            {
                cout << " Size del disco invalida: " << disco->size << endl;
            }
            if (disco->path.empty())
            {
                cout << " Path invalido: " << disco->path << endl;
            }
            if (disco->name.empty())
            {
                cout << " Nombre invalido: " << disco->name << endl;
            }
            return;
        }
        mkdisk *mk = new mkdisk();
        if (!mk->es_multiplo_8(disco->size))
        {
            cout << "El Size no es multiplo de 8" << endl;
            return;
        }

        FILE *file;
        file = fopen(disco->path.c_str(), "r");
        if (file != NULL)
        {
            if (strcasecmp(disco->type.c_str(), "l") == 0)
            {
                Create_Logic(disco);
            }
            else
            {
                Create_Primary_Extended(disco);
            }
        }
    }
}

// Correcto
// Devuelve si esta vacia o no
bool fdisk::EstaVacia(particion mbr_partition[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (mbr_partition[i].part_size > 0)
        {
            return false;
        }
    }
    return true;
}

// Correcto
bool fdisk::EstaLLena(particion mbr_partition[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (mbr_partition[i].part_size < 0)
        {
            return false;
        }
    }
    return true;
}

// Correcto
// Primer ajuste
void fdisk::First_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int &fin)
{
    int espacioDisponible;

    // Se establece el tamanio de la particion
    if (disco->unit == "B" || disco->unit == "b")
    {
        tamanioparticion = disco->size;
    }
    else if (disco->unit == "K" || disco->unit == "k" || disco->unit.empty())
    {
        tamanioparticion = disco->size * 1024;
    }
    else if (disco->unit == "M" || disco->unit == "m")
    {
        tamanioparticion = disco->size * 1024 * 1024;
    }
    //cout<<"tam:"<<tamanioparticion<<" mbr: "<<_mbr.mbr_tamano;
    // Se recorre el arreglo de particiones en busca de espacios
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_size < 0)
        {
            if (i == 0)
            {
                inicio = sizeof(mbr);
            }
            else
            {
                inicio = _mbr.mbr_partition[i - 1].part_start + _mbr.mbr_partition[i - 1].part_size;
            }
            for (int j = i; j < 4; j++)
            {
                if (_mbr.mbr_partition[j].part_size > 0 || j == 3)
                {
                    if (j == 3 && _mbr.mbr_partition[j].part_size < 0)
                    {
                        fin = _mbr.mbr_tamano - 1;
                    }
                    else
                    {
                        fin = _mbr.mbr_partition[j].part_start - 1;
                    }
                    espacioDisponible = fin - inicio;
                    if (espacioDisponible >= tamanioparticion)
                    {

                        cout << i << "  " << inicio << "  " << fin << "  " << tamanioparticion << endl;
                        index = i;
                        return;
                    }
                    i = j;
                }
            }
        }
    }
    index = -1;
    inicio = -1;
    tamanioparticion = -1;
    return;
}

// Probar cambios
// Mejor ajuste
void fdisk::Best_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int &fin)
{
    int espacioDisponible;
    int temporal = 0;
    int tempinicio = 0;
    int tempfin = 0;
    int indice;
    // Se calcula el tamaño deseado para la particion
    if (disco->unit == "B" || disco->unit == "b")
    {
        tamanioparticion = disco->size;
    }
    else if (disco->unit == "K" || disco->unit == "k" || disco->unit.empty())
    {
        tamanioparticion = disco->size * 1024;
    }
    else if (disco->unit == "M" || disco->unit == "m")
    {
        tamanioparticion = disco->size * 1024 * 1024;
    }
    // Se recorre el arreglo de particiones en busca de espacios
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_size < 0)
        {
            if (i == 0)
            {
                inicio = sizeof(mbr);
            }
            else
            {
                inicio = _mbr.mbr_partition[i - 1].part_start + _mbr.mbr_partition[i - 1].part_size;
            }
            for (int j = i; j < 4; j++)
            {
                if (_mbr.mbr_partition[j].part_size > 0 || j == 3)
                {
                    if (j == 3 && _mbr.mbr_partition[j].part_size < 0)
                    {
                        fin = _mbr.mbr_tamano - 1;
                    }
                    else
                    {
                        fin = _mbr.mbr_partition[j].part_start - 1;
                    }
                    espacioDisponible = fin - inicio;
                    if (espacioDisponible >= tamanioparticion)
                    {
                        if (temporal == 0)
                        {
                            tempinicio = inicio;
                            tempfin = fin;
                            temporal = espacioDisponible;
                            indice = i;
                        }
                        else
                        {
                            if (espacioDisponible > temporal)
                            {
                                tempinicio = inicio;
                                tempfin = fin;
                                temporal = espacioDisponible;
                                indice = i;
                            }
                        }
                    }
                    i = j;
                }
            }
        }
    }
    if (indice >= 0)
    {

        index = indice;
        tempinicio = inicio;
        tempfin = fin;
        // tamanioparticion = temporal;
        return;
        // return indice, tempinicio, tempfin, temporal;
    }

    inicio = -1;
    index = -1;
    fin = -1;
    tamanioparticion = -1;
    return;
    // return -1 ,-1, -1, -1;
}

// Probar cambios
// Peor ajuste
void fdisk::Worst_Fit(mbr _mbr, fdisk *disco, int &tamanioparticion, int &inicio, int &index, int &fin)
{
    int espacioDisponible;
    int temporal = 0;
    int tempinicio = 0;
    int tempfin = 0;
    int indice;
    // Se calcula el tamaño deseado para la particion
    if (disco->unit == "B" || disco->unit == "b")
    {
        tamanioparticion = disco->size;
    }
    else if (disco->unit == "K" || disco->unit == "k" || disco->unit.empty())
    {
        tamanioparticion = disco->size * 1024;
    }
    else if (disco->unit == "M" || disco->unit == "m")
    {
        tamanioparticion = disco->size * 1024 * 1024;
    }
    // Se recorre el arreglo de particiones en busca de espacios
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_size < 0)
        {
            if (i == 0)
            {
                inicio = sizeof(mbr);
            }
            else
            {
                inicio = _mbr.mbr_partition[i - 1].part_start + _mbr.mbr_partition[i - 1].part_size;
            }
            for (int j = i; j < 4; j++)
            {
                if (_mbr.mbr_partition[j].part_size > 0 || j == 3)
                {
                    if (j == 3 && _mbr.mbr_partition[j].part_size < 0)
                    {
                        fin = _mbr.mbr_tamano - 1;
                    }
                    else
                    {
                        fin = _mbr.mbr_partition[j].part_start - 1;
                    }
                    espacioDisponible = fin - inicio;
                    if (espacioDisponible >= tamanioparticion)
                    {
                        if (temporal == 0)
                        {
                            tempinicio = inicio;
                            tempfin = fin;
                            temporal = espacioDisponible;
                            indice = i;
                        }
                        else
                        {
                            if (espacioDisponible < temporal)
                            {
                                tempinicio = inicio;
                                tempfin = fin;
                                temporal = espacioDisponible;
                                indice = i;
                            }
                        }
                    }
                    i = j;
                }
            }
        }
    }
    if (indice >= 0)
    {
        index = indice;
        inicio = tempinicio;
        fin = tempfin;
        // tamanioparticion = temporal;
        return;
        // return indice, tempinicio, tempfin, temporal;
    }

    index = -1;
    inicio = -1;
    fin = -1;
    tamanioparticion = -1;
    return;
    // return -1 ,-1, -1, -1;
}

// Probar
// Metodo para la creacion de particiones primarias y extendidas
void fdisk::Create_Primary_Extended(fdisk *disco)
{
    int inicio = 0, fin = 0, index = 0, tamanio = 0;
    FILE *archivo;
    mbr _mbr;
    archivo = fopen(disco->path.c_str(), "rb+");
    // Obtenemos el mbr del disco
    fseek(archivo, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, archivo);

    if (EstaLLena(_mbr.mbr_partition))
    {
        fclose(archivo);
        cout << "Ya Existen 4 particiones dentro del disco" << endl;
        return;
    }
    if (_mbr.disk_fit == 'B')
    {
        Best_Fit(_mbr, disco, tamanio, inicio, index, fin);
    }
    else if (_mbr.disk_fit == 'F')
    {
        First_Fit(_mbr, disco, tamanio, inicio, index, fin);
        //cout << "TAMANIO:" << tamanio << endl;
    }
    else if (_mbr.disk_fit == 'W')
    {
        Worst_Fit(_mbr, disco, tamanio, inicio, index, fin);
    }
    if (index >= 0)
    {
        // Se asigna el nombre
        for (int i = 0; i < disco->name.size(); i++)
        {
            _mbr.mbr_partition[index].part_name[i] = disco->name[i];
        }
        if (strcasecmp(disco->fit.c_str(), "bf") == 0)
        {
            _mbr.mbr_partition[index].part_fit = 'B';
        }
        else if (strcasecmp(disco->fit.c_str(), "ff") == 0)
        {
            _mbr.mbr_partition[index].part_fit = 'F';
        }
        else if (strcasecmp(disco->fit.c_str(), "wf") == 0 || disco->fit.empty())
        {
            _mbr.mbr_partition[index].part_fit = 'W';
        }

        _mbr.mbr_partition[index].part_size = tamanio;
        _mbr.mbr_partition[index].part_start = inicio + 1;
        _mbr.mbr_partition[index].part_status = '0';
        strcpy(_mbr.mbr_partition[index].part_name, disco->name.c_str());
        if (disco->type == "p" || disco->type == "P" || disco->type.empty())
        {
            _mbr.mbr_partition[index].part_type = 'P';
        }
        else if (disco->type == "e" || disco->type == "E")
        {
            // Falta escribir el ebr para las particiones extendidas

            _mbr.mbr_partition[index].part_type = 'E';
            FILE *file2;
            file2 = fopen(disco->path.c_str(), "rb+");
            // cout<<"Aqui se guardo:"<<_mbr.mbr_partition[index].part_start<<endl;
            fseek(file2, _mbr.mbr_partition[index].part_start, SEEK_SET);
            ebr _ebr;
            _ebr.part_fit = '-';
            _ebr.part_next = -1;
            _ebr.part_size = -1;
            _ebr.part_start = inicio + 1;
            _ebr.part_status = '0';
            _ebr.part_name[0] = '\0';

            fwrite(&_ebr, sizeof(ebr), 1, file2);
            fclose(file2);
        }
    }
    fseek(archivo, 0, SEEK_SET);
    fwrite(&_mbr, sizeof(mbr), 1, archivo);
    fclose(archivo);
}

// Probar
void fdisk::Create_Logic(fdisk *disco)
{
    int tamanioparticion = 0;
    if (disco->unit == "B" || disco->unit == "b")
    {
        tamanioparticion = disco->size;
    }
    else if (disco->unit == "K" || disco->unit == "k" || disco->unit.empty())
    {
        tamanioparticion = disco->size * 1024;
    }
    else if (disco->unit == "M" || disco->unit == "m")
    {
        tamanioparticion = disco->size * 1024 * 1024;
    }

    FILE *file;
    file = fopen(disco->path.c_str(), "rb+");
    mbr _mbr;
    ebr _ebr;
    int espacioDisp;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    int index = ExisteExtendida(_mbr.mbr_partition);
    int posicion;
    fseek(file, _mbr.mbr_partition[index].part_start, SEEK_SET);
    fread(&_ebr, sizeof(ebr), 1, file);

    while (_ebr.part_next > 0)
    {
        espacioDisp = _ebr.part_size - sizeof(ebr);
        if (_ebr.part_size < 0 && tamanioparticion <= espacioDisp)
        {
            break;
        }
        fseek(file, _ebr.part_next, SEEK_SET);
        fread(&_ebr, sizeof(ebr), 1, file);
    }

    ebr nuevo;
    if (strcasecmp(disco->fit.c_str(), "bf") == 0)
    {
        nuevo.part_fit = 'B';
    }
    else if (strcasecmp(disco->fit.c_str(), "ff") == 0)
    {
        nuevo.part_fit = 'F';
    }
    else if (strcasecmp(disco->fit.c_str(), "wf") == 0 || disco->fit.empty())
    {
        nuevo.part_fit = 'W';
    }
    nuevo.part_size = tamanioparticion;
    nuevo.part_start = _ebr.part_start;
    nuevo.part_status = '0';
    strcpy(nuevo.part_name, disco->name.c_str());

    if (_ebr.part_next < 0)
    {
        nuevo.part_next = nuevo.part_size + nuevo.part_start + 1;
        int tamDisp = _mbr.mbr_partition[index].part_start + _mbr.mbr_partition[index].part_size - nuevo.part_start;
        if (nuevo.part_size > tamDisp)
        {
            cout << "Espacio insuficiente para crear la particióm " << disco->name << endl;
            return;
        }
    }
    else
    {
        nuevo.part_next = _ebr.part_next;
    }
    fseek(file, nuevo.part_start, SEEK_SET);
    fwrite(&nuevo, sizeof(ebr), 1, file);
    if (_ebr.part_next < 0)
    {
        ebr vacio;
        vacio.part_fit = '-';
        vacio.part_next = -1;
        vacio.part_size = -1;
        vacio.part_start = nuevo.part_start + nuevo.part_size + 1;
        vacio.part_status = '0';
        vacio.part_name[0] = '\0';
        fseek(file, vacio.part_start, SEEK_SET);
        fwrite(&vacio, sizeof(ebr), 1, file);
    }
    fclose(file);
}
