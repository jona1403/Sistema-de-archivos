#include "../Headers/mkfs.h"
#include "../Headers/VariablesGlobales.h"
#include <cstring>

mkfs ::mkfs()
{
}

void mkfs ::controller(mkfs *disco)
{
    if (disco->addd.empty())
    {
        this->formatear(disco);
    }
    else
    {
        cout << "Coando add de mkfs" << endl;
    }
}

void mkfs ::formatear(mkfs *disco)
{
    char type;
    munt mountaux;
    diskmount discoaux;
    bool encontrada = false;
    FILE *file;
    FILE *file2;
    int discos = 0;
    for (int i = 0; i < Discos_Montados.size(); i++)
    {
        for (int j = 0; j < Discos_Montados[i].particiones.size(); j++)
        {
            if (Discos_Montados[i].particiones[j].mount_id ==
                id.c_str())
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
    file = fopen(discoaux.disk_path.c_str(), "rb+");
    if (file == NULL)
    {
        cout << "Error al leer el archivo 1" << endl;
    }
    else
    {
        discos = 1;
    }

    file2 = fopen((discoaux.disk_path.substr(0, discoaux.disk_path.length() - 4) + "_ESPEJO" + ".dsk").c_str(), "rb+");
    if (file2 == NULL)
    {
        cout << "Error al leer el archivo 2" << endl;
    }
    else
    {
        if (discos == 0)
        {
            discos = 2;
        }
        else if (discos == 1)
        {
            discos = 3;
        }
    }

    if (discos == 0)
    {
        return;
    }

    fseek(file, 0, SEEK_SET);
    mbr _mbr;
    bool esPrimaria = false;
    bool esLogica = false;
    fread(&_mbr, sizeof(mbr), 1, file);
    int extendida = -1;
    for (int i = 0; i < 4; i++)
    {
        if (_mbr.mbr_partition[i].part_status == '1')
        {
           /* if (_mbr.mbr_partition[i].part_type == 'E')
            {
                cout << "Entra ext" << endl;
                extendida = i;
            }*/

            if (disco->id == mountaux.mount_id)
            {
                if(mountaux.mount_part.part_size>0){
                    esPrimaria = true;
                }else{
                    esLogica = true;
                }
                break;
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

    
    /*if (!esPrimaria && extendida >= 0)
    {
        esLogica = true;
    }*/
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
    // pendiente
    fseek(file, start, SEEK_SET);
    if (strcasecmp(disco->type.c_str(), "full"))
    {
        char buff;
        buff = '\0';
        for (int i = 0; i < siz; i++)
        {
            fwrite(&buff, sizeof(buff), 1, file);
            fseek(file, start + i, SEEK_SET);
        }
    }
    Super_Bloque superbloque;
    /*if(regex_match(disco->fs, r2fs) || disco->fs.empty()){
        superbloque.s_filesystem_type = 2;
    }else{*/
    superbloque.s_filesystem_type = 3;
    //}
    superbloque.s_inodes_count = n;
    superbloque.s_free_inodes_count = n;
    superbloque.s_blocks_count = 3 * n;
    superbloque.s_free_blocks_count = 3 * n;
    superbloque.s_mtime = time(0);
    superbloque.s_mnt_count = 1;
    superbloque.s_magic = 0xEF53;
    superbloque.s_inode_size = sizeof(Tabla_Inodos);
    superbloque.s_block_size = sizeof(Bloque_archivos);
    superbloque.s_first_ino = 3;
    superbloque.s_first_blo = 3;



    // Se asignan los comienzos del sistema ext 3 journaling, bitmap de  inodos, bitmap de bloques, inodos, bloques
    superbloque.s_bm_inode_start = start + sizeof(Super_Bloque) + n * sizeof(journal);
    superbloque.s_bm_block_start = superbloque.s_bm_inode_start + n;
    superbloque.s_inode_start = superbloque.s_bm_block_start + 3 * n;
    superbloque.s_block_start = superbloque.s_inode_start + n * sizeof(Tabla_Inodos);

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, start, SEEK_SET);
    fwrite(&superbloque, sizeof(Super_Bloque), 1, file);
    fclose(file);
    char bitmapinodos[n];
    char bitmapbloques[3 * n];

    // Se llena el bitmap de inodos
    for (int i = 0; i < n; i++)
    {
        bitmapinodos[i] = '0';
    }
    // Se llena el bitmap de bloques
    for (int i = 0; i < n * 3; i++)
    {
        bitmapbloques[i] = '0';
    }

    bitmapinodos[0] = '1';
    bitmapinodos[1] = '1';

    bitmapbloques[0] = '1';
    bitmapbloques[1] = '1';

    // Se escribe el bitmap de inodos en el disco
    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_bm_inode_start, SEEK_SET);
    fwrite(&bitmapinodos, sizeof(bitmapinodos), 1, file);
    fclose(file);
    // Se escribe el bitmap de inodos en el disco
    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_bm_block_start, SEEK_SET);
    fwrite(&bitmapbloques, sizeof(bitmapbloques), 1, file);
    fclose(file);
    // Se escribe el journal si la particion es ext3
    // if(regex_match(disco->type, r3fs)){
    journal journall;
    journall.estado = 0;
    journall.fecha_op = time(0);
    journall.path[0] = '\0';
    journall.tipo = -1;
    strcpy(journall.contenido, "--");
    journall.tamanio = 0;
    journall.tipo_op[0] = '\0';

    // Creo el journal de la carpeta
    journal journalCarpeta;
    journalCarpeta.estado = 1;
    strcpy(journalCarpeta.tipo_op, "mkfs");
    journalCarpeta.fecha_op = time(0);
    strcpy(journalCarpeta.path, "/");
    journalCarpeta.id_propietario = '1';
    strcpy(journalCarpeta.contenido, "-");
    journalCarpeta.tipo = '0';

    // Creo el journal del archivo
    journal journalArchivo;
    journalArchivo.estado = 1;
    strcpy(journalArchivo.tipo_op, "mkfs");
    journalArchivo.fecha_op = time(0);
    strcpy(journalArchivo.path, "/users.txt");
    journalArchivo.id_propietario = '1';
    strcpy(journalArchivo.contenido, "1,G,root1,U,root,root,123");
    journalArchivo.tipo = '1';

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, start + sizeof(Super_Bloque), SEEK_SET);
    fwrite(&journall, sizeof(journal), 1, file);
    fclose(file);

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, start + sizeof(Super_Bloque) + sizeof(journal), SEEK_SET);
    fwrite(&journalCarpeta, sizeof(journal), 1, file);
    fclose(file);

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, start + sizeof(Super_Bloque) + 2 * sizeof(journal), SEEK_SET);
    fwrite(&journalArchivo, sizeof(journal), 1, file);
    fclose(file);
    //}

    // Se crea el inodo raiz
    Tabla_Inodos inodoRaiz;
    inodoRaiz.i_uid = 1;
    inodoRaiz.l_gid = 1;
    inodoRaiz.i_size = 0;
    inodoRaiz.i_atime = time(0);
    inodoRaiz.i_ctime = time(0);
    inodoRaiz.i_mtime = time(0);
    inodoRaiz.i_type = 0;
    inodoRaiz.i_block[0] = superbloque.s_block_start;
    for (int i = 1; i < 15; i++)
    {
        inodoRaiz.i_block[i] = -1;
    }
    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_inode_start, SEEK_SET);
    fwrite(&inodoRaiz, sizeof(Tabla_Inodos), 1, file);
    fclose(file);
    // Se crea el bloque para la carpeta raiz
    Bloque_Carpetas carpetaRaiz;

    strcpy(carpetaRaiz.name, "root");
    strcpy(carpetaRaiz.padre, "");

    carpetaRaiz.b_content[0].b_inodo = superbloque.s_inode_start + sizeof(Tabla_Inodos);
    strcpy(carpetaRaiz.b_content[0].b_name, "users.txt");

    carpetaRaiz.b_content[1].b_inodo = -1;
    carpetaRaiz.b_content[1].b_name[0] = '0';

    carpetaRaiz.b_content[2].b_inodo = -1;
    carpetaRaiz.b_content[2].b_name[0] = '0';

    carpetaRaiz.b_content[3].b_inodo = -1;
    carpetaRaiz.b_content[3].b_name[0] = '0';

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_block_start, SEEK_SET);
    fwrite(&carpetaRaiz, sizeof(Bloque_Carpetas), 1, file);
    fclose(file);
    // Inodo de archivo
    Tabla_Inodos inodoArchivo;
    inodoArchivo.i_uid = 1;
    inodoArchivo.l_gid = 1;
    inodoArchivo.i_size = sizeof("1,G,root1,U,root,root,123");
    inodoArchivo.i_atime = time(0);
    inodoArchivo.i_ctime = time(0);
    inodoArchivo.i_mtime = time(0);
    inodoArchivo.i_type = 1;
    inodoArchivo.i_block[0] = superbloque.s_block_start + sizeof(Bloque_Carpetas);
    for (int i = 1; i < 15; i++)
    {
        inodoArchivo.i_block[i] = -1;
    }

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_inode_start + sizeof(Tabla_Inodos), SEEK_SET);
    fwrite(&inodoArchivo, sizeof(Tabla_Inodos), 1, file);
    fclose(file);
    // Bloque de archivo de usuario
    Bloque_archivos usuariostxt;
    strcpy(usuariostxt.name, "Usuarios.txt");
    strcpy(usuariostxt.b_content, "1,G,root1,U,root,root,123");

    file = fopen(discoaux.disk_path.c_str(), "rb+");
    fseek(file, superbloque.s_block_start + sizeof(Bloque_Carpetas), SEEK_SET);
    fwrite(&usuariostxt, sizeof(Bloque_archivos), 1, file);
    fclose(file);
    cout<<"Formateo completo"<<endl;
}