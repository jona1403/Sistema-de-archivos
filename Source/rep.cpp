#include "../Headers/rep.h"
#include <iostream>
#include <cstring>
//#include "../Headers/estructuras.h"
#include "../Headers/VariablesGlobales.h"

rep::rep() {}

void rep ::controller(rep *disco)
{
    if (strcasecmp(disco->name.c_str(), "mbr") == 0)
    {
        repMBR(disco);
    }
    else if (strcasecmp(disco->name.c_str(), "disk") == 0)
    {
        cout << "Generando reporte disk" << endl;
        repDisck(disco);
    }
    else if (strcasecmp(disco->name.c_str(), "inode") == 0)
    {
        // repInode(disco, 1);
    }
    else if (strcasecmp(disco->name.c_str(), "journaling") == 0)
    {
    }
    else if (strcasecmp(disco->name.c_str(), "block") == 0)
    {
        // repInode(disco, 0);
    }
    else if (strcasecmp(disco->name.c_str(), "bm_inode") == 0)
    {
        repbmBlock(disco, 1);
    }
    else if (strcasecmp(disco->name.c_str(), "bm_block") == 0)
    {
        repbmBlock(disco, 0);
    }
    else if (strcasecmp(disco->name.c_str(), "tree") == 0)
    {
        repTree(disco);
    }
    else if (strcasecmp(disco->name.c_str(), "sb") == 0)
    {
        repSB(disco);
    }
    else if (strcasecmp(disco->name.c_str(), "file") == 0)
    {
    }
    else if (strcasecmp(disco->name.c_str(), "ls") == 0)
    {
    }
    else
    {
        cout << "El atributo " << disco->name.c_str() << ", no es valido" << endl;
    }
}

void rep ::repbmBlock(rep *disco, int type)
{
    munt mountaux;
    diskmount discoaux;
    bool encontrada = false;

    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id ==
                disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                discoaux = Discos_Montados[i];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }
    if (!encontrada)
    {
        cout << "NO ENCONTRADA" << endl;
    }
    FILE *file;
    file = fopen(discoaux.disk_path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    bool esPrimaria = false;
    bool esLogica = false;
    int extendida = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_status == '1')
        {
            if (disco->id == mountaux.mount_id && mountaux.mount_part.part_size > 0)
            {
                esPrimaria = true;
            }
            else
            {
                esLogica = true;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {

        if (_mbr.mbr_partition[i].part_type == 'E')
        {
            extendida = i;
            break;
        }
    }

    if (!esPrimaria && !esLogica)
    {
        cout << "Esta particion no se encuentra en este disco" << endl;
        return;
    }

    int n = 0;
    int siz = 0;
    int start = 0;
    if (esLogica)
    {
        start = mountaux.mount_logic.part_start + sizeof(ebr);
        siz = mountaux.mount_logic.part_size;
        n = (mountaux.mount_logic.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }
    else
    {
        start = mountaux.mount_part.part_start;
        siz = mountaux.mount_part.part_size;
        n = (mountaux.mount_part.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }
    Super_Bloque superbloque;
    char bitmapinodos[n];
    char bitmapbloques[3 * n];
    Tabla_Inodos inodoraiz;

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_bm_inode_start, SEEK_SET);
    fread(&bitmapinodos, n, 1, file);
    fclose(file);

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_bm_block_start, SEEK_SET);
    fread(&bitmapbloques, 3 * n, 1, file);
    fclose(file);

    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=TB;";
    graph += "node [shape=record];";

    if (type == 0)
    {
        graph += "1 [label=\"{BM_INODOS|{";
        for (int i = 0; i < n; i++)
        {
            graph += bitmapinodos[i];
            if (((i + 1) % 20) == 0)
            {
                graph += "}|{";
            }
            else
            {
                graph += "|";
            }
        }
        graph += "}";
    }
    else if (type == 1)
    {
        graph += "1 [label=\"{BM_Bloques|{";
        for (int i = 0; i < (3 * n); i++)
        {
            graph += bitmapbloques[i];
            if (((i + 1) % 20) == 0)
            {
                graph += "}|{";
            }
            else
            {
                graph += "|";
            }
        }
        graph += "}";
    }
    graph += "}\"];";

    graph += "}";

    // cout<<"Pasa"<<endl;

    FILE *file2;
    file2 = fopen("ReporteBM.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteBM.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
    cout << "Reporte de bitmap generado exitosamente !!" << endl;
}

void rep ::repSB(rep *disco)
{
    munt mountaux;
    bool encontrada = false;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }

    int start = 0;
    if (mountaux.mount_type == 'L')
    {
        start = mountaux.mount_logic.part_start + sizeof(ebr);
    }
    else
    {
        start = mountaux.mount_part.part_start;
    }
    FILE *file;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    Super_Bloque superbloque;
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=LR;";
    graph += "node [shape=record];";

    graph += "1 [label=\"";

    graph += "Reporte de SUPERBLOQUE | { sb_nombre_hd |";
    graph += disco->path.c_str();
    graph += "}|";
    graph += "{sb_arbol_virtual_count|";
    graph += to_string(superbloque.s_blocks_count);
    graph += "}|";
    graph += "{sb_detalle_directorio_count|";
    graph += to_string(superbloque.s_blocks_count);
    graph += "}|";
    graph += "{sb_inodos_count|";
    graph += to_string(superbloque.s_inodes_count);
    graph += "}|";
    graph += "{sb_bloques_count|";
    graph += to_string(superbloque.s_blocks_count);
    graph += "}|";
    graph += "{sb_arbol_virtual_free|";
    graph += to_string(superbloque.s_free_blocks_count);
    graph += "}|";
    graph += "{sb_detalle_directorio_free|";
    graph += to_string(superbloque.s_free_blocks_count);
    graph += "}|";
    graph += "{sb_inodos_free|";
    graph += to_string(superbloque.s_free_inodes_count);
    graph += "}|";
    graph += "{sb_bloques_free|";
    graph += to_string(superbloque.s_free_blocks_count);
    graph += "}|";
    graph += "{sb_date_creacion|";
    graph += to_string(superbloque.s_mtime);
    graph += "}|";
    graph += "{sb_date_ultimo_montaje|";
    graph += to_string(superbloque.s_mtime);
    graph += "}|";
    graph += "{sb_montajes_count|";
    graph += to_string(superbloque.s_mnt_count);
    graph += "}|";
    graph += "{sb_ap_bitmap_arbol_directorio|}|";
    graph += "{sb_ap_arbol_directorio|}|";
    graph += "{sb_ap_bitmap_detalle_directorio|}|";
    graph += "{sb_ap_detalle_directorio|}|";
    graph += "{sb_ap_bitmap_inodos|}|";
    graph += "{sb_ap_inodos|}|";
    graph += "{sb_ap_bitmap_bloques|}|";
    graph += "{sb_ap_bloques|}|";
    graph += "{sb_ap_log|}|";
    graph += "{sb_size_struct_arbol_directorio|}|";
    graph += "{sb_size_struct_detalle_directorio|}|";
    graph += "{sb_size_struct_inodo|}|";
    graph += "{sb_size_struct_bloque|}|";
    graph += "{sb_first_free_bit_arbol_directorio|}|";
    graph += "{sb_first_free_bit_detalle_directorio|}|";
    graph += "{sb_first_free_bit_tabla_inodos|}|";
    graph += "{sb_first_free_bit_bloques|}|";
    graph += "{sb_magic_num|";
    graph += to_string(superbloque.s_magic);
    graph += "}";

    graph += "\"];";
    graph += "}";

    FILE *file2;
    file2 = fopen("ReporteSB.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteSB.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
}
// Falta terminarlo
void rep ::repInode(rep *disco)
{
    this->numNodo = 1;
    munt mountaux;
    bool encontrada = false;
    FILE *file;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }

    file = fopen(mountaux.mount_path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    bool esPrimaria = false;
    bool esLogica = false;
    int extendida = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_status == '1')
        {
            if (disco->id == mountaux.mount_id && mountaux.mount_part.part_size > 0)
            {
                esPrimaria = true;
            }
            else
            {
                esLogica = true;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {

        if (_mbr.mbr_partition[i].part_type == 'E')
        {
            extendida = i;
            break;
        }
    }

    if (!esPrimaria && !esLogica)
    {
        cout << "Esta particion no se encuentra en este disco" << endl;
        return;
    }

    int n = 0;
    int siz = 0;
    int start = 0;
    if (esLogica)
    {
        start = mountaux.mount_logic.part_start + sizeof(ebr);
        siz = mountaux.mount_logic.part_size;
        n = (mountaux.mount_logic.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }
    else
    {
        start = mountaux.mount_part.part_start;
        siz = mountaux.mount_part.part_size;
        n = (mountaux.mount_part.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }

    Super_Bloque superbloque;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    int taman = 0;
    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=TB;";
    graph += "node [shape=record];";

    // graph += this->Inode_Block_Recursivo(superbloque.s_inode_start, mountaux.mount_path, n, start, 0);

    graph += "}";

    FILE *file2;
    file2 = fopen("ReporteIB.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteIB.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
    cout << "Reporte generado exitosamente !!" << endl;
}

void rep ::repDisck(rep *disco)
{

    munt mountaux;
    bool encontrada = false;
    FILE *file;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }
    mbr _mbr;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    int taman = 0;
    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=TB;";
    graph += "node [shape=record];";
    graph += "1 [label=\"{MBR";
    graph += "|Tamanio: " + to_string(_mbr.mbr_tamano);
    // graph+=_mbr.mbr_fecha_creacion;
    graph += "|Signature: " + to_string(_mbr.mbr_disk_signature);
    graph += "}|";
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_size < 0)
        {
            graph += "{Libre";
            graph += "}|";
        }
        else
        {
            if (_mbr.mbr_partition[i].part_type == 'P')
            {
                string name = _mbr.mbr_partition[i].part_name;
                graph += "{Primaria";
                graph += "|Nombre: " + name;
                graph += "|Tamanio: " + to_string(_mbr.mbr_partition[i].part_size);
                graph += "}|";
            }
            else if (_mbr.mbr_partition[i].part_type == 'E')
            {
                graph += "{Extendida|{";
                // graph+= to_string(taman);
                bool haySiguiente = true;
                FILE *file1;
                ebr _ebr;
                file1 = fopen(mountaux.mount_path.c_str(), "rb+");
                fseek(file1, _mbr.mbr_partition[i].part_start, SEEK_SET);
                fread(&_ebr, sizeof(ebr), 1, file1);
                while (haySiguiente)
                {
                    // cout<<"Enciclado"<<endl;
                    graph += "EBR|{Logica";
                    if (_ebr.part_size > 0)
                    {
                        string nme = _ebr.part_name;
                        graph += "|Nombre: " + nme;
                        graph += "|Tamanio: " + to_string(_ebr.part_size);
                    }
                    else
                    {
                        graph += "|Libre";
                    }
                    graph += "}|";
                    if (_ebr.part_next > 0)
                    {
                        fseek(file1, _ebr.part_next, SEEK_SET);
                        fread(&_ebr, sizeof(ebr), 1, file1);
                    }
                    else
                    {
                        haySiguiente = false;
                    }
                }
                graph += "}}|";
            }
        }
    }
    graph += "\"];";

    graph += "}";

    // cout<<"Pasa"<<endl;

    FILE *file2;
    file2 = fopen("ReporteDK.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteDK.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
    cout << "Reporte del disco generado exitosamente !!" << endl;
}

void rep::repMBR(rep *disco)
{

    munt mountaux;
    bool encontrada = false;
    FILE *file;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }

    mbr _mbr;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=LR;";
    graph += "node [shape=record];";

    graph += "1 [label=\"";
    graph += "REPORTE DE MBR | {mbr_tamanio|";
    graph += to_string(_mbr.mbr_tamano);
    graph += "} | { mbr_disk_signature |";
    graph += to_string(_mbr.mbr_disk_signature);
    graph += "} | { mbr_disk_fit |";
    string f(1, _mbr.disk_fit);
    graph += "}|";
    for (int i = 0; i < 4; i++)
    {
        graph += "Particion |";
        graph += "{Part_status|";
        graph += _mbr.mbr_partition[i].part_status;
        graph += "}| ";

        graph += "{Part_type|";
        graph += _mbr.mbr_partition[i].part_type;
        graph += "}| ";

        graph += "{Part_fit|";
        graph += _mbr.mbr_partition[i].part_fit;
        graph += "}| ";

        graph += "{Part_start|";
        graph += to_string(_mbr.mbr_partition[i].part_start);
        graph += "}| ";

        graph += "{Part_size|";
        graph += to_string(_mbr.mbr_partition[i].part_size);
        graph += "}| ";

        graph += "{Part_name|";
        graph += _mbr.mbr_partition[i].part_name;
        graph += "}|";

        if (_mbr.mbr_partition[i].part_type == 'E')
        {
            file = fopen(mountaux.mount_path.c_str(), "rb+");
            fseek(file, _mbr.mbr_partition[i].part_start, SEEK_SET);
            ebr _ebr;
            fread(&_ebr, sizeof(ebr), 1, file);
            cout << _ebr.part_next << endl;
            bool existeSiguiente = true;
            while (existeSiguiente)
            {
                graph += "EBR Particion Lógica |";
                graph += "{Part_name|";
                graph += _ebr.part_name;
                graph += "}|";
                graph += "{Part_status|";
                if (_ebr.part_status == '1')
                {
                    graph += "1";
                }
                else
                {
                    graph += "0";
                }
                graph += "}| ";

                graph += "{Part_next|";
                graph += to_string(_ebr.part_next);
                graph += "}| ";

                graph += "{Part_fit|";
                if (_ebr.part_fit == 'F')
                {
                    graph += "F";
                }
                else if (_ebr.part_fit == 'B')
                {
                    graph += "B";
                }
                else if (_ebr.part_fit == 'W')
                {
                    graph += "W";
                }
                graph += "}| ";

                graph += "{Part_start|";
                graph += to_string(_ebr.part_start);
                graph += "}| ";

                graph += "{Part_size|";
                graph += to_string(_ebr.part_size);
                graph += "}| ";

                if (_ebr.part_next < 1)
                {
                    existeSiguiente = false;
                }
                else
                {
                    fseek(file, _ebr.part_next, SEEK_SET);
                    fread(&_ebr, sizeof(ebr), 1, file);
                }
            }
        }
    }

    graph += "\"];";

    graph += "}";

    FILE *file2;
    file2 = fopen("ReporteMBR.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteMBR.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
}

void rep ::repTree(rep *disco)
{
    this->numNodo = 1;
    munt mountaux;
    bool encontrada = false;
    FILE *file;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }

    file = fopen(mountaux.mount_path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    bool esPrimaria = false;
    bool esLogica = false;
    int extendida = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_status == '1')
        {
            if (disco->id == mountaux.mount_id && mountaux.mount_part.part_size > 0)
            {
                esPrimaria = true;
            }
            else
            {
                esLogica = true;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {

        if (_mbr.mbr_partition[i].part_type == 'E')
        {
            extendida = i;
            break;
        }
    }

    if (!esPrimaria && !esLogica)
    {
        cout << "Esta particion no se encuentra en este disco" << endl;
        return;
    }

    int n = 0;
    int siz = 0;
    int start = 0;
    if (esLogica)
    {
        start = mountaux.mount_logic.part_start + sizeof(ebr);
        siz = mountaux.mount_logic.part_size;
        n = (mountaux.mount_logic.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }
    else
    {
        start = mountaux.mount_part.part_start;
        siz = mountaux.mount_part.part_size;
        n = (mountaux.mount_part.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }

    Super_Bloque superbloque;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    int taman = 0;
    string graph = "";
    graph += "digraph G{";
    graph += "fontname=\"Helvetica,Arial,sans-serif\"";
    graph += "node [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "edge [fontname=\"Helvetica,Arial,sans-serif\"]";
    graph += "concentrate=True;";
    graph += "rankdir=TB;";
    graph += "node [shape=record];";

    graph += this->TreeRecursivo(superbloque.s_inode_start, mountaux.mount_path, n, start, 0);

    graph += "}";

    FILE *file2;
    file2 = fopen("ReporteTree.dot", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    string aa = "";
    aa += "dot -Tpng ReporteTree.dot -o ";
    aa += disco->path.c_str();
    system(aa.c_str());
    cout << "Reporte Tree generado exitosamente !!" << endl;
}

int rep ::cantidadRutas(string path)
{
    int cantidad = 0;
    char pattern = '/';
    int posInit = 0;
    int posFound = 0;
    string splitted;
    string str = path;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        if (!splitted.empty())
        {
            cout << splitted << endl;
            cantidad++;
        }
        posInit = posFound + 1;
    }
    return cantidad;
}

string rep ::nombre(string path)
{
    char pattern = '/';
    int posInit = 0;
    int posFound = 0;
    string splitted;
    string str = path;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        splitted = str.substr(posInit, posFound - posInit);
        if (!splitted.empty())
        {
            return splitted;
        }
        posInit = posFound + 1;
    }
    return "";
}

string rep ::TreeRecursivo(int posInodo, string pathfisc, int n, int start, int apuntt)
{
    int apuntaux;
    string graph = "";
    FILE *file;
    Tabla_Inodos inodo;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, posInodo, SEEK_SET);
    fread(&inodo, sizeof(Tabla_Inodos), 1, file);
    fclose(file);

    Super_Bloque superbloque;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    Bloque_Carpetas auxcar;
    string nm;

    // Aqui se grafica el inodo
    apuntaux = this->numNodo;
    if (apuntt != 0)
    {
        graph += "\n" + to_string(apuntt) + "->" + to_string(apuntaux) + "\n";
    }
    graph += to_string(this->numNodo++) + " [label=\"{";
    graph += "INODO";
    graph += "|{Tipo: |" + to_string(inodo.i_type);
    graph += "}";
    graph += "|{Tamanio: |" + to_string(inodo.i_size);
    graph += "}";
    for (int i = 0; i < 15; i++)
    {
        graph += "|{AD" + to_string(i + 1) + ": |" + to_string(inodo.i_block[i]);
        graph += "}";
    }
    graph += "}\"]";

    // Se busca el bloque en el inodo
    for (int i = 0; i < 15; i++)
    {
        if (inodo.i_block[i] > 0)
        {
            if (i < 12)
            {
                cout << "TIPO: " << to_string(inodo.i_type) << endl;
                if (to_string(inodo.i_type) == "0")
                {
                    file = fopen(pathfisc.c_str(), "rb+");
                    fseek(file, inodo.i_block[i], SEEK_SET);
                    fread(&auxcar, sizeof(Bloque_Carpetas), 1, file);
                    fclose(file);

                    graph += to_string(this->numNodo++) + " [label=\"{";
                    graph += "Bloque";
                    string pa = auxcar.padre;
                    graph += "|{Padre: |" + pa;
                    graph += "}";
                    string carno = auxcar.name;
                    graph += "|{Nombre: |" + carno;
                    graph += "}";
                    for (int i = 0; i < 4; i++)
                    {
                        graph += "|{API: |" + to_string(auxcar.b_content[i].b_inodo);
                        graph += "}";
                    }

                    graph += "}\"]";
                    int apuntb = this->numNodo - 1;
                    graph += "\n" + to_string(apuntaux) + "->" + to_string(this->numNodo - 1) + "\n";

                    for (int i = 0; i < 4; i++)
                    {
                        if (auxcar.b_content[i].b_inodo > 0)
                        {
                            graph += this->TreeRecursivo(auxcar.b_content[i].b_inodo, pathfisc, n, start, apuntb);
                        }
                    }
                }
                else
                {
                    Bloque_archivos auxarchivo;
                    file = fopen(pathfisc.c_str(), "rb+");
                    fseek(file, inodo.i_block[i], SEEK_SET);
                    fread(&auxarchivo, sizeof(Bloque_archivos), 1, file);
                    fclose(file);
                    graph += "\n" + to_string(apuntaux) + "->" + to_string(this->numNodo) + "\n";
                    graph += to_string(this->numNodo++) + " [label=\"{";
                    graph += "Bloque Archivo";
                    string pa = auxarchivo.b_content;
                    graph += "|{Contenido: |" + pa;
                    graph += "}";

                    graph += "}\"]";
                }
                return graph;
            }
            else if (i == 12)
            {
                // aqui los bloques simples indirectos
            }
            else if (i == 13)
            {
                // aqui los bloques dobles indirectos
            }
            else if (i == 14)
            {
                // aqui los bloques triples indirectos
            }
        }
    }
    return graph;
}

void rep ::Syncronice(rep *disco)
{
    munt mountaux;
    bool encontrada = false;
    FILE *file;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id == disco->id.c_str())
            {
                encontrada = true;
                mountaux = Discos_Montados[i].particiones[j];
                break;
            }
        }
        if (encontrada)
        {
            break;
        }
    }

    if (!encontrada)
    {
        cout << "Particion no montada" << endl;
        return;
    }

    file = fopen(mountaux.mount_path.c_str(), "rb+");
    mbr _mbr;
    fseek(file, 0, SEEK_SET);
    fread(&_mbr, sizeof(mbr), 1, file);
    fclose(file);
    bool esPrimaria = false;
    bool esLogica = false;
    int extendida = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_status == '1')
        {
            if (disco->id == mountaux.mount_id && mountaux.mount_part.part_size > 0)
            {
                esPrimaria = true;
            }
            else
            {
                esLogica = true;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {

        if (_mbr.mbr_partition[i].part_type == 'E')
        {
            extendida = i;
            break;
        }
    }

    if (!esPrimaria && !esLogica)
    {
        cout << "Esta particion no se encuentra en este disco" << endl;
        return;
    }

    int n = 0;
    int siz = 0;
    int start = 0;
    if (esLogica)
    {
        start = mountaux.mount_logic.part_start + sizeof(ebr);
        siz = mountaux.mount_logic.part_size;
        n = (mountaux.mount_logic.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }
    else
    {
        start = mountaux.mount_part.part_start;
        siz = mountaux.mount_part.part_size;
        n = (mountaux.mount_part.part_size - sizeof(Super_Bloque)) /
            (4 + sizeof(journal) + sizeof(Tabla_Inodos) + 3 * sizeof(Bloque_archivos));
    }

    Super_Bloque superbloque;
    file = fopen(mountaux.mount_path.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    int taman = 0;
    string graph = "";
    graph = "[\n";

    graph += this->SyncroniceRecursivo(superbloque.s_inode_start, mountaux.mount_path, n, start, 0);

    graph += "]";

    FILE *file2;
    file2 = fopen("../backend/Syncronice.json", "w+");
    fwrite(graph.c_str(), graph.length(), 1, file2);
    fclose(file2);

    cout << "Syncronice generado exitosamente." << endl;
}

string rep ::SyncroniceRecursivo(int posInodo, string pathfisc, int n, int start, int apuntt)
{
    int apuntaux;
    string graph = "";
    FILE *file;
    Tabla_Inodos inodo;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, posInodo, SEEK_SET);
    fread(&inodo, sizeof(Tabla_Inodos), 1, file);
    fclose(file);

    Super_Bloque superbloque;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    Bloque_Carpetas auxcar;
    string nm;

    // Se busca el bloque en el inodo
    for (int i = 0; i < 15; i++)
    {
        if (inodo.i_block[i] > 0)
        {
            if (i < 12)
            {
                cout << "TIPO: " << to_string(inodo.i_type) << endl;
                if (to_string(inodo.i_type) == "0")
                {
                    file = fopen(pathfisc.c_str(), "rb+");
                    fseek(file, inodo.i_block[i], SEEK_SET);
                    fread(&auxcar, sizeof(Bloque_Carpetas), 1, file);
                    fclose(file);
                    graph += "{\n";
                    string carno = auxcar.name;
                    graph += "\"nombre\":\"" + carno + "\",\n";
                    graph += "\"propietario\":\"\",\n";
                    graph += "\"tipo\":0,\n";
                    graph += "\"grupo\":[],\n";
                    graph += "\"contenido\":[\n";
                    for (int i = 0; i < 4; i++)
                    {
                        if (auxcar.b_content[i].b_inodo > 0)
                        {
                            graph += this->SyncroniceRecursivo(auxcar.b_content[i].b_inodo, pathfisc, n, start, apuntt) + "\n";
                        }
                        if (i < 3)
                        {
                            if (auxcar.b_content[i + 1].b_inodo > 0)
                            {
                                graph += ",\n";
                            }
                        }
                    }
                    graph += "]\n";
                    graph += "}\n";
                    if (i < 14)
                    {
                        if (inodo.i_block[i + 1] > 0)
                        {
                            graph += ",\n";
                        }
                    }
                }
                else
                {
                    Bloque_archivos auxarchivo;
                    file = fopen(pathfisc.c_str(), "rb+");
                    fseek(file, inodo.i_block[i], SEEK_SET);
                    fread(&auxarchivo, sizeof(Bloque_archivos), 1, file);
                    fclose(file);

                    graph += "{\n";
                    graph += "\"nombre\":\"\",\n";
                    graph += "\"propietario\":\"\",\n";
                    graph += "\"tipo\":1,\n";
                    graph += "\"grupo\":[],\n";
                    string bc = auxarchivo.b_content;
                    graph += "\"contenido\":\""+bc+"\"";
                    graph+= "}";
                    if (i < 14)
                    {
                        if (inodo.i_block[i + 1] > 0)
                        {
                            graph += ",\n";
                        }
                    }
                }
                return graph;
            }
            else if (i == 12)
            {
                // aqui los bloques simples indirectos
            }
            else if (i == 13)
            {
                // aqui los bloques dobles indirectos
            }
            else if (i == 14)
            {
                // aqui los bloques triples indirectos
            }
        }
    }
    return graph;
}