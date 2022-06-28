#include "../Headers/mkfile.h"
#include "../Headers/VariablesGlobales.h"
#include <cstring>

mkfile ::mkfile() {}

void mkfile ::controller(mkfile *doc) {
    create(doc);
}

int mkfile ::cantidadRutas(string path)
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

string mkfile ::nombre(string path)
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

void mkfile ::createRecursivo(int posInodo, string path, string pathfisc, int posSB, int n, int start, string contenido)
{
    FILE *file;
    Tabla_Inodos inodo;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, posInodo, SEEK_SET);
    fread(&inodo, sizeof(Tabla_Inodos), 1, file);
    fclose(file);

    Super_Bloque superbloque;
    file = fopen(pathfisc.c_str(), "rb+");
    fseek(file, posSB, SEEK_SET);
    fread(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);

    // Se separa la ruta para validacion de existencias
    int posInit = 0;
    int posFound = 0;
    string splitted;
    char pattern = '/';
    // es la ruta que queda
    string str = path;

    while (posFound >= 0)
    {
        posFound = str.find(pattern, posInit);
        // es la carpeta que se lee
        splitted = str.substr(posInit, posFound - posInit);
        posInit = posFound + 1;

        if (!splitted.empty())
        {
            if (posFound >= 0)
            {
                str = str.substr(posFound, str.length());
            }

            break;
        }
    }
    Bloque_Carpetas auxcar;
    bool encontrada = false;
    string nm;
    // Se busca el bloque en el inodo
    for (int i = 0; i < 15; i++)
    {
        cout << "Entra a los inodos" << endl;
        if (inodo.i_block[i] > 0)
        {
            cout << "Encontro uno ocupado" << endl;
            if (i < 12)
            {
                file = fopen(pathfisc.c_str(), "rb+");
                fseek(file, inodo.i_block[i], SEEK_SET);
                fread(&auxcar, sizeof(Bloque_Carpetas), 1, file);
                fclose(file);
                if (auxcar.name == splitted)
                {
                    mkfile *mkfl = new mkfile();
                    if (mkfl->cantidadRutas(str) > 1)
                    {
                        nm = nombre(str);
                        for (int j = 0; j < 4; j++)
                        {
                            if (nm == auxcar.b_content[j].b_name)
                            {
                                Tabla_Inodos in;
                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, auxcar.b_content[j].b_inodo, SEEK_SET);
                                fread(&in, sizeof(Tabla_Inodos), 1, file);
                                fclose(file);
                                createRecursivo(auxcar.b_content[j].b_inodo, str, pathfisc, posSB, n, start, contenido);
                                return;
                            }
                            cout << "La carpeta " << nm << " " << auxcar.b_content[j].b_name << " no existe." << endl;
                        }
                    }
                    else
                    {
                        // Aqui se crea
                        nm = nombre(str);
                        for (int j = 0; j < 4; j++)
                        {
                            if (auxcar.b_content[j].b_inodo < 0)
                            {
                                strcpy(auxcar.b_content[j].b_name, nm.c_str());

                                auxcar.b_content[j].b_inodo = superbloque.s_inode_start + ((superbloque.s_first_ino - 1) * sizeof(Tabla_Inodos));

                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, inodo.i_block[i], SEEK_SET);
                                fwrite(&auxcar, sizeof(Bloque_Carpetas), 1, file);
                                fclose(file);

                                Tabla_Inodos Inodo_Aux;
                                Inodo_Aux.i_uid = 1;
                                Inodo_Aux.l_gid = 1;
                                Inodo_Aux.i_size = 0;
                                Inodo_Aux.i_atime = time(0);
                                Inodo_Aux.i_ctime = time(0);
                                Inodo_Aux.i_mtime = time(0);
                                Inodo_Aux.i_type = 1;
                                // Aqui se debe guardar la posicion del bloque
                                Inodo_Aux.i_block[0] = superbloque.s_block_start + ((superbloque.s_first_blo - 1) * sizeof(Bloque_Carpetas));
                                for (int x = 1; x < 15; x++)
                                {
                                    Inodo_Aux.i_block[x] = -1;
                                }

                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, auxcar.b_content[j].b_inodo, SEEK_SET);
                                fwrite(&Inodo_Aux, sizeof(Tabla_Inodos), 1, file);
                                fclose(file);

                                Bloque_archivos bloquearch;
                                strcpy(bloquearch.name, nm.c_str());
                                strcpy(bloquearch.b_content, contenido.c_str());
                                //Bloque_Carpetas Bloque_Aux;


                                superbloque.s_first_blo++;
                                superbloque.s_first_ino++;
                                superbloque.s_free_blocks_count--;
                                superbloque.s_free_inodes_count--;

                                char bmi[n];
                                char bmb[3 * n];
                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, superbloque.s_bm_block_start, SEEK_SET);
                                fread(&bmb, sizeof(bmb), 1, file);
                                bmb[superbloque.s_first_blo - 1] = '1';
                                fseek(file, superbloque.s_bm_block_start, SEEK_SET);
                                fwrite(&bmb, sizeof(bmb), 1, file);
                                fclose(file);

                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, superbloque.s_bm_inode_start, SEEK_SET);
                                fread(&bmi, sizeof(bmi), 1, file);
                                bmi[superbloque.s_first_ino - 1] = '1';
                                fseek(file, superbloque.s_bm_inode_start, SEEK_SET);
                                fwrite(&bmi, sizeof(bmi), 1, file);
                                fclose(file);

                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, Inodo_Aux.i_block[0], SEEK_SET);
                                fwrite(&bloquearch, sizeof(Bloque_archivos), 1, file);
                                fclose(file);

                                file = fopen(pathfisc.c_str(), "rb+");
                                fseek(file, start, SEEK_SET);
                                fwrite(&superbloque, sizeof(Super_Bloque), 1, file);
                                fclose(file);
                                cout << "Directorio Creado" << endl;
                                return;
                            }
                        }
                    }
                }
                // Aqui se validan los bloques directos
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
    //}
}

void mkfile ::create(mkfile *dir)
{

    munt mountaux;
    diskmount discoaux;
    bool encontrada = false;

    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id ==
                dir->id.c_str())
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
            if (dir->id == mountaux.mount_id && mountaux.mount_part.part_size > 0)
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

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_inode_start, SEEK_SET);
    fread(&inodoraiz, sizeof(Tabla_Inodos), 1, file);
    fclose(file);

    if (cantidadRutas(dir->path) > 1)
    {
        cout << "Aqui se comienza" << endl;
        createRecursivo(superbloque.s_inode_start, dir->path, discoaux.disk_path, start, n, start, dir->cont);
    }
}