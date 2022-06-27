#include "../Headers/analizador.h"
#include "../Headers/VarGlobales.h"
#include "../Headers/analizadorar.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>

string path, name, unit, type, fit, del, addd, mov, id, p, cont, fileid,
    dest, iddest, order, filen;
int size;

Analizador ::Analizador() {}

void Analizador ::comillaspath()
{
    string a = path;
    if (path.empty())
    {
        return;
    }
    if (a.substr(0, 1) == "\"")
    {
        cout << "entra" << endl;
        path = path.substr(1, path.length() - 2);
    }
}

void Analizador ::analizarparamsInd(char *comando2)
{
    // cout << "comando: " << comando << endl;

    int posInit = 0;
    int posFound = 0;
    string splitted;
    string comando = comando2;
    string comandos[2];
    int taman = 0;
    while (posFound >= 0)
    {
        if (taman > 0)
        {
            posInit++;
        }
        posFound = comando.find('=', posInit);
        splitted = comando.substr(posInit, posFound - posInit);
        posInit = posFound + 1;
        comandos[taman] = splitted;
        taman++;
    }

    if (strcasecmp(comandos[0].c_str(), "$size") == 0 || strcasecmp(comandos[0].c_str(), "@size") == 0)
    {
        cout << "Size: " << comandos[1] << endl;
        size = stoi(comandos[1]);
    }
    else if (strcasecmp(comandos[0].c_str(), "$path") == 0)
    {
        if (comandos[1].substr(0, 1) == "\"")
        {
            path = comandos[1].substr(1, comandos[1].length() - 2);
        }
        else
        {
            path = comandos[1];
        }
        cout << "Path: " << path << endl;
    }
    else if (strcasecmp(comandos[0].c_str(), "$name") == 0)
    {
        if (comandos[1].substr(0, 1) == "\"")
        {
            name = comandos[1].substr(1, comandos[1].length() - 2);
        }
        else
        {
            name = comandos[1];
        }
        cout << "Name: " << name << endl;
    }
    else if (strcasecmp(comandos[0].c_str(), "@unit") == 0)
    {
        cout << "Unit: " << comandos[1] << endl;
        unit = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@type") == 0)
    {
        cout << "type: " << comandos[1] << endl;
        type = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@fit") == 0)
    {
        cout << "Fit: " << comandos[1] << endl;
        fit = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@delete") == 0)
    {
        cout << "Delete: " << comandos[1] << endl;
        del = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@add") == 0)
    {
        cout << "Add: " << comandos[1] << endl;
        addd = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@mov") == 0)
    {
        cout << "Comando mov" << endl;
    }
    else if (strcasecmp(comandos[0].c_str(), "$id") == 0)
    {
        cout << "Id: " << comandos[1] << endl;
        id = comandos[1];
    }
    else if (strcasecmp(comandos[0].c_str(), "@p") == 0)
    {
        cout << "Comando p" << endl;
        p = "p";
    }
    else if (strcasecmp(comandos[0].c_str(), "@cont") == 0)
    {
        if (comandos[1].substr(0, 1) == "\"")
        {
            cont = comandos[1].substr(1, comandos[1].length() - 2);
        }
        else
        {
            cont = comandos[1];
        }

        cout << "Cont: " << comandos[1] << endl;
    }
}

void Analizador ::analizarparams(char *comando)
{
    char *token = strtok(comando, " ");
    if (token != NULL)
    {
        while (token != NULL)
        {
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            // printf("Token: %s\n", token);
            analizarparamsInd(token);
            token = strtok(NULL, " ");
        }
    }
}

void Analizador ::limpiarParams()
{
    path = name = unit = type = fit = del = addd = mov = id = p = cont = fileid =
        dest = iddest = order = filen = "";
}

void Analizador::analizar(char *comando)
{
    char *token = strtok(comando, " ");

    if (token != NULL)
    {
        if (strcasecmp(token, "mkdisk") == 0)
        {
            mkdisk *mk = new mkdisk();
            token = strtok(NULL, "");
            analizarparams(token);
            mmkdisk->path = path;
            mmkdisk->name = mk->name = name;
            mmkdisk->fit = mk->fit = fit;
            mmkdisk->size = mk->size = size;
            mmkdisk->unit = mk->unit = unit;
            //mk->path = path.substr(0, path.length() - 4) + "_ESPEJO" + ".dsk";
            mmkdisk->driver(mmkdisk);
            mmkdisk = new mkdisk();
            //mk->driver(mk);
        }
        else if (strcasecmp(token, "rmdisk") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);

            rrmkdisk->path = path; // substr(1, path.length() - 2);
            rrmkdisk->driver(rrmkdisk);
        }
        else if (strcasecmp(token, "fdisk") == 0)
        {
            fdisk *fd = new fdisk();
            token = strtok(NULL, "");
            analizarparams(token);
            ffdisk->unit = fd->unit = unit;
            ffdisk->type = fd->type = type;
            ffdisk->size = fd->size = size;
            ffdisk->path = path;
            ffdisk->name = fd->name = name;
            ffdisk->fit = fd->fit = fit;
            ffdisk->deleten = fd->deleten = del;
            ffdisk->adden = fd->adden = addd;
            fd->path = path.substr(0, path.length() - 4) + "_ESPEJO" + ".dsk";
            ffdisk->Controller(ffdisk);
            cout<<"------------------------"<<endl;
            cout<<"------Particiones-------"<<endl;
            cout<<"------------------------"<<endl;
            ffdisk->ShowPartitions(ffdisk);
            //ffdisk->path = ffdisk->path.substr(0, ffdisk->path.length() - 4) + "_ESPEJO" + ".dsk";
            /*cout<<"------------------------"<<endl;
            cout<<"-----Particiones 2------"<<endl;
            cout<<"------------------------"<<endl;*/
            fd->Controller(fd);
            //fd->ShowPartitions(fd);
            fd = new fdisk();
            ffdisk = new fdisk();

        }
        else if (strcasecmp(token, "mount") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            mmount->path = path;
            mmount->name = name;
            mmount->Mount_Partition(mmount);
            mmount->showMount();
            mmount = new mount();
        }
        else if (strcasecmp(token, "unmount") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            mmount->Unmount_Partition(id);
            mmount = new mount();
        }
        else if (strcasecmp(token, "mkfs") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            mmkfs->id = id;
            mmkfs->type = type;
            mmkfs->addd = addd;
            mmkfs->unit = unit;
            mmkfs->formatear(mmkfs);
            mmkfs = new mkfs();
        }
        else if (strcasecmp(token, "mkfile") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            mmkfile->cont= cont;
            mmkfile->id= id;
            mmkfile->path = path;
            mmkfile->p = p;
            mmkfile->controller(mmkfile);
            mmkfile = new mkfile();
        }
        else if (strcasecmp(token, "exec") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            AnalizadorAr *an = new AnalizadorAr();
            an->split(an->leer(path), '\n');
            an = new AnalizadorAr();
        }
        else if (strcasecmp(token, "rm") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
        }
        else if (strcasecmp(token, "mkdir") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            mmkdir->id = id;
            mmkdir->p = p;
            mmkdir->path = path;
            mmkdir->create(mmkdir);
            mmkdir = new mkdir();
        }
        else if (strcasecmp(token, "cp") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
        }
        else if (strcasecmp(token, "ls") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
        }
        else if (strcasecmp(token, "pause") == 0)
        {
            string entradap;
            cout << "Ingrese una tecla para continuar la ejecución del programa: ";
            cin >> entradap;
            cout << endl;
        }
        else if (strcasecmp(token, "recovery") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
        }
        else if (strcasecmp(token, "loss") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
        }
        else if (strcasecmp(token, "rep") == 0)
        {
            token = strtok(NULL, "");
            analizarparams(token);
            rrep->id = id;
            rrep->name = name;
            rrep->path = path;
            rrep->controller(rrep);
            rrep = new rep();
        }
        else if(strcasecmp(token, "syncronice") == 0){
            token = strtok(NULL, "");
            analizarparams(token);
            rrep->id = id;
            rrep->path = path;
            rrep->Syncronice(rrep);
            rrep = new rep();
        }
        else
        {
            cout << "Comando invalido: " << token << endl;
            token = strtok(NULL, " ");
        }
    }
    limpiarParams();
}
