#include "string"
#include "time.h"
#include <iostream>
#include <ctime>
#include "vector"

using namespace std;


typedef struct
{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
} particion;

typedef struct
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    char disk_fit;
    particion mbr_partition[4];
} mbr;

typedef struct
{
    char part_status;
    char part_fit;
    int  part_start;
    int  part_size;
    int  part_next;
    char part_name[16];
} ebr;

typedef struct{
    string mount_id;
    string mount_path;
    string mount_name;
    char mount_type;
    particion mount_part;
    ebr mount_logic;
} munt;

typedef struct{
    string disk_path;
    string disk_id;
    //int disk_num;
    string disk_letra;
    vector<munt> particiones;
}diskmount;

typedef struct {
    int estado;
    char tipo_op[10];
    int tipo;
    char path[100];
    time_t fecha_op;
    char contenido[100];
    char id_propietario;
    int tamanio;
} journal;

typedef struct{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
}Super_Bloque;

typedef struct{
    int i_uid;
    int l_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
}Tabla_Inodos;

typedef struct{
    bool estaLogeado;
    string id;
    string usuario;
}Logeado;

typedef struct{
    char b_name[12];
    int b_inodo;
}content;

typedef struct{
    char padre[12];
    char name[12];
    content b_content[4];
}Bloque_Carpetas;

typedef struct{
    char name[15];
    char b_content[73];
}Bloque_archivos;

typedef struct{
    int b_pointers[16];
}Bloque_Apuntadores;

typedef struct{
    char b_name[15];
    char b_content[50];
}Bloque;

class estructuras
{
public:
    estructuras();
};