#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jni_FileJNI.h"
#include "time.h"

#define DISK "disk"

#define VER 1

#define USR_SIZE 25
#define USR_NAME_MAX 12
#define USR_PSW_MAX 12

#define SUPER_AREA_SIZE 512
#define SUPER_BEGIN 1024
#define SUPER_SIZE 7

#define INODE_BITMAP_AREA_SIZE 1024
#define INODE_BITMAP_BEGIN 2048
#define INODE_BITMAP_SIZE 1024

#define DATA_BITMAP_AREA_SIZE 10240
#define DATA_BITMAP_BEGIN 3096
#define DATA_BITMAP_SIZE 10240

#define SELECT_BIT '1'
#define DESELECT_BIT '0'

#define INODE_TABLE_AREA_SIZE 51200
#define INODE_TABLE_BEGIN 13336
#define INODE_TABLE_SIZE 711

#define DATA_TABLE_AREA_SIZE 10421760
#define DATA_TABLE_BEGIN 64536
#define DATA_TABLE_SIZE 9614

#define FILE_NAME_MAX 52
#define FILE_SIZE_MAX 17
#define FATHER_DIR_NAME ".."
#define THIS_DIR_NAME "."
#define ROOT "/"
#define USR "usr"
#define HOME "home"
#define DEFAULT_USR "root"

#define DELETED 0
#define NOT_DELETED 1
#define TYPE_DIR 1
#define TYPE_FILE 0

#define MOUNTED 1
#define NOT_MOUNTED 0

typedef unsigned char byte;

// Inode - DataBlock 比例失衡


// 72 bytes
typedef struct SuperBlock{
    int mount;
    long ver;
    long block_size;
    long data_area_size;
    long inode_area_size;
    long inode_begin;
    long data_begin;
    long data_size;
    long inode_size;
    long remain_inode_num;
    long remain_data_num;
    time_t mount_time;
    time_t last_write_time;
    int valid;
}SuperBlock;

// 60 bytes
typedef struct Pair{
    char file_name[FILE_NAME_MAX];
    int inode_no;
    int delete;
}Pair;

// 1084 bytes
typedef struct BlockFile{
    int delete;
    long no;
    int flag;
    byte data[1024];
    long next;
    byte fill[44]; // 填充位
}BlockFile;

// 1084 bytes
typedef struct BlockDir{
    int delete;
    long no;
    int flag;
    char name[FILE_NAME_MAX];
    Pair file_info[FILE_SIZE_MAX];
}BlockDir;

// 72 bytes
typedef struct Inode{
    int no;
    long size;
    int flag;
    long data_num;
    long data_begin;
    char owner[12];
    int power;
    time_t create_time;
    time_t update_time;
    time_t read_time;
    int delete;
}Inode;

////// SUPER
void init_super_block();

SuperBlock* read_super_block();

void print_super_block();

int update_super_block(SuperBlock* sb);

SuperBlock* update_remain_block();





////// BITMAP
void select_inode_bitmap(int no);

void select_data_bitmap(long no);

void delete_inode_bitmap(int no);

void delete_data_bitmap(long no);

void print_inode_bitmap();

void print_data_bitmap();

int find_inode_bitmap(int no);

int find_data_bitmap(long no);

long get_inode_bitmap_num();

long get_data_bitmap_num();


///// COMMON
void copyPair(Pair* new, Pair* origin);

int find_empty_block();

void print_pair(Pair*);

int update_dir_pair(BlockDir* block, Pair* pair);

int find_pair(const char[FILE_NAME_MAX], Pair* pair);





////// INODE
void print_inode(Inode* inode);

int find_empty_inode();

Inode* find_inode(int no);

Inode* create_inode(int flag, const char ower[USR_NAME_MAX], int power);

int delete_inode(int no);

int update_inode(Inode* new);

void init_inode();


////DATA
void init_data();


//// DIR
void print_dir_block(BlockDir* dir);

BlockDir* create_dir_block(const char name[FILE_NAME_MAX]);

int delete_dir_block(long no);

int update_dir_block(BlockDir* new);

BlockDir* find_dir_block(long no);

BlockDir* get_root_block();



//// FILE
void print_file_block(BlockFile* file);

BlockFile* create_empty_file_block();

void delete_file_block(long no);

int update_file_block(BlockFile* file);

BlockFile* find_file_block(long no);

void ls(int);

Inode* mount();

int mkdir1(const char[FILE_NAME_MAX], int);

int touch(const char[FILE_NAME_MAX], int);

int rm(int no, int father);

int cd(const char[FILE_NAME_MAX], int);

int mv_in(const char[FILE_NAME_MAX], int); // 移入

int mv_out(int, const char[FILE_NAME_MAX]); // 移除


Inode* create_dir(const char name[FILE_NAME_MAX], const char owner[USR_NAME_MAX], int power, Inode *father);

Inode* create_file(const char name[FILE_NAME_MAX], Inode *father); // 移入外部文件

Inode* create_empty_file(const char name[FILE_NAME_MAX], Inode* father); // 创建新文件

int delete_file(Inode*);

int delete_dir(Inode*);

////// SUPRE
void init_super_block(){
    SuperBlock* tmp = (SuperBlock*)malloc(sizeof(SuperBlock));

    time_t now;
    time(&now);

    tmp->mount = MOUNTED;
    tmp->block_size = 1024;
    tmp->ver = VER;
    tmp->inode_area_size = INODE_TABLE_AREA_SIZE;
    tmp->inode_begin = INODE_TABLE_BEGIN;
    tmp->inode_size = INODE_TABLE_SIZE;
    tmp->data_area_size = DATA_TABLE_AREA_SIZE;
    tmp->data_size = DATA_TABLE_SIZE;
    tmp->data_begin = DATA_TABLE_BEGIN;
    tmp->remain_data_num = DATA_TABLE_SIZE;
    tmp->remain_inode_num = INODE_TABLE_SIZE;
    tmp->last_write_time = now;
    tmp->mount_time = now;
    tmp->valid = 0;

    update_super_block(tmp);
}

SuperBlock* read_super_block(){
    FILE* fr = fopen(DISK, "rb");
    fseek(fr, SUPER_BEGIN, 0);
    SuperBlock* sb = (SuperBlock*)malloc(sizeof(SuperBlock));
    if(0 == fread(sb, sizeof(SuperBlock), 1, fr))
        return NULL;

    fclose(fr);
    return sb;
}

void print_super_block(){
    update_remain_block();
    SuperBlock* sb = read_super_block();
    if(sb == NULL)
        return;

    printf("SUPER_BLOCK =>\n block_size : %ld b, ver: %ld, inode_area_size: %.2ld kb, data_area_size: %.2ld kb,\n "
                   "inode_begin: %.2ld M, data_begin: %.2ld M, remain_inode: %ld/%d, remain_data: %ld/%d\n\n",
           sb->block_size, sb->ver, (sb->inode_area_size / 1024), (sb->data_area_size / 1024), (sb->inode_begin/ 1024), (sb->data_begin/ 1024),
           sb->remain_inode_num, INODE_TABLE_SIZE, sb->remain_data_num, DATA_TABLE_SIZE);
}

int update_super_block(SuperBlock* sb){
    if(sb == NULL)
        return 0;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, SUPER_BEGIN, 0);
    int time = 0;
    while(time < SUPER_SIZE){
        fwrite(sb, sizeof(SuperBlock), 1, fw);
        fflush(fw);
        time ++;
    }
    fclose(fw);
    return 1;
}

SuperBlock* update_remain_block(){
    SuperBlock* sb = read_super_block();

    sb->remain_inode_num = INODE_TABLE_SIZE - get_inode_bitmap_num();
    sb->remain_data_num = DATA_TABLE_SIZE - get_data_bitmap_num();
    time_t now;
    time(&now);
    sb->last_write_time = now;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, SUPER_BEGIN, 0);
    int time = 0;
    while(time < SUPER_SIZE){
        fwrite(sb, sizeof(SuperBlock), 1, fw);
        fflush(fw);
        time ++;
    }
    fclose(fw);
}



////// BITMAP
void print_data_bitmap(){
    FILE *fr = fopen(DISK, "rb");
    fseek(fr, DATA_BITMAP_SIZE, 0);
    char* c = (char*)malloc(1);
    int i;
    for(i = 0 ;i < DATA_BITMAP_SIZE;i ++){
        fread(c, 1, 1, fr);
        if(i % 10 == 0)
            printf("\n");
        printf("%c", c[0]);
    }
    fclose(fr);
}

void print_inode_bitmap(){
    FILE *fr = fopen(DISK, "rb");
    fseek(fr, INODE_BITMAP_SIZE, 0);
    int i;
    for(i = 0 ;i < INODE_BITMAP_SIZE;i ++){
        if(i % 10 == 0)
            printf("\n");
        printf("%c", fgetc(fr));
    }
    fclose(fr);
}

void select_inode_bitmap(int no){
    if(no >= INODE_BITMAP_SIZE)
        return;

    byte flag = SELECT_BIT;
    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, INODE_BITMAP_BEGIN + no, 0);
    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void select_data_bitmap(long no){
    if(no >= DATA_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "rb+");

    byte flag = SELECT_BIT;
    fseek(fw, DATA_BITMAP_BEGIN + no, 0);
    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void delete_inode_bitmap(int no){
    if(no >= INODE_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "rb+");

    byte flag = DESELECT_BIT;
    fseek(fw, sizeof(byte)*no + INODE_BITMAP_BEGIN, 0);

    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void delete_data_bitmap(long no){
    if(no >= DATA_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "rb+");

    byte flag = DESELECT_BIT;
    fseek(fw, sizeof(byte)*no + DATA_BITMAP_BEGIN, 0);

    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

int find_inode_bitmap(int no){
    FILE *fr = fopen(DISK, "rb");
    fseek(fr, sizeof(byte)*no + INODE_BITMAP_BEGIN, 0);
    byte flag = (byte) fgetc(fr);
    fclose(fr);

    if(flag == SELECT_BIT)
        return 1;

    return 0;
}

int find_data_bitmap(long no){
    FILE *fr = fopen(DISK, "rb");
    fseek(fr, sizeof(byte)*no + DATA_BITMAP_BEGIN, 0);
    byte flag  = (byte) fgetc(fr);
    fclose(fr);

    if(flag == SELECT_BIT)
        return 1;

    return 0;
}

long get_inode_bitmap_num(){
    int num = 0;

    FILE *fr = fopen(DISK, "rb");
    fseek(fr, INODE_BITMAP_BEGIN, 0);
    int i;
    for(i = 0 ;i < INODE_TABLE_SIZE;i ++){
        if(fgetc(fr) == SELECT_BIT)
            num ++;
    }
    fclose(fr);
    return num;
}

long get_data_bitmap_num(){
    int num = 0;

    FILE *fr = fopen(DISK, "rb");
    fseek(fr, DATA_BITMAP_BEGIN, 0);
    int i;
    for(i = 0 ;i < DATA_TABLE_SIZE;i ++){

        if(fgetc(fr) == SELECT_BIT)
            num ++;
    }
    fclose(fr);
    return num;
}



///// COMMON
void copyPair(Pair* origin, Pair* new){
int i;
for(i = 0; i < FILE_SIZE_MAX;i ++){
origin->inode_no = new->inode_no;
origin->delete = new->delete;
strcpy(origin->file_name, new->file_name);
}
}

int find_empty_block(){
    FILE *fr = fopen(DISK, "rb");
    int block_num = 0;

    BlockDir* tmp = (BlockDir *)malloc(sizeof(BlockDir));
    fseek(fr, DATA_TABLE_BEGIN, 0);
    while(block_num < DATA_TABLE_SIZE && fread(tmp, sizeof(BlockDir), 1, fr) != 0){
        if(tmp->delete == DELETED)
        return block_num;

        block_num ++;
    }

    fclose(fr);
    return block_num;
}

//ERROR
void print_pair(Pair* pair){
    if(pair == NULL)
        return;
    int i;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
        return;
        printf("    Pair => no:%d, file_name: %s, inode_no: %d, delete: %d\n", i, p.file_name, p.inode_no, p.delete);
    }
}

int update_dir_pair(BlockDir* dir, Pair* pair){
    if(dir == NULL || pair == NULL)
        return 0;

    int i = 0;
    while(i < FILE_SIZE_MAX){
        Pair* p = dir->file_info + i;
        i ++;
        if(p->delete == DELETED)
        continue;
        if(strcmp(p->file_name, pair->file_name) == 0)
            return 0;
    }

    i = 0;
    while(i < FILE_SIZE_MAX){
        Pair* p = dir->file_info + i;
        if(p->delete == DELETED){
            copyPair(p, pair);
            break;
        }
        i ++;
    }

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, DATA_TABLE_BEGIN + sizeof(BlockDir)*(dir->no), 0);

    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

int find_pair(const char name[FILE_NAME_MAX], Pair* pair){
    if(pair == NULL)
        return -1;
    int i;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
        return -1;
        if(strcmp(pair[i].file_name, name) == 0){
            return pair[i].inode_no;
        }
    }
    return -1;
}




//// INODE
void print_inode(Inode* inode){
    if(inode == NULL)
        return;

    struct tm *tmp_create = NULL;
    struct tm *tmp_update = NULL;
    struct tm *tmp_read = NULL;

    time_t t_create = inode->create_time;
    time_t t_update = inode->update_time;
    time_t t_read = inode->read_time;

    tmp_create = gmtime(&t_create);
    tmp_update = gmtime(&t_update);
    tmp_read = gmtime(&t_read);

    if(inode->flag == TYPE_FILE){
        printf("INODE ==> type: %s, no: %d, size: %ld, data_num: %ld, data_begin: %ld, owener: %s, power: %d, delete: %d, create_time: %d.%d.%d %d:%d:%d\n",
               "file",
               inode->no,
               inode->size,
               inode->data_num,
               inode->data_begin,
               inode->owner,
               inode->power,
               inode->delete,
                (1900+tmp_create->tm_year), (1+tmp_create->tm_mon), (tmp_create->tm_mday),tmp_create->tm_hour, tmp_create->tm_min, tmp_create->tm_sec);
    }else{
        printf("INODE ==> type: %s, no:%d, data_begin: %ld, owner: %s, power: %d, delete: %d, create_time:  %d.%d.%d %d:%d:%d\n",
               "dir",
               inode->no,
               inode->data_begin,
               inode->owner,
               inode->power,
               inode->delete,
                (1900+tmp_create->tm_year), (1+tmp_create->tm_mon), (tmp_create->tm_mday),tmp_create->tm_hour, tmp_create->tm_min, tmp_create->tm_sec);
    }
}

int find_empty_inode(){
    FILE *fr = fopen(DISK, "rb");
    int block_num = 0;

    Inode* tmp = (Inode *)malloc(sizeof(Inode));
    fseek(fr, INODE_TABLE_BEGIN, 0);
    while(block_num < INODE_TABLE_SIZE && fread(tmp, sizeof(Inode), 1, fr) != 0){
        if(tmp->delete == DELETED)
        return block_num;
        block_num ++;
    }

    fclose(fr);
    return block_num;
}

Inode* find_inode(int no){
    if(no >= INODE_TABLE_SIZE)
        return  NULL;

    FILE* fr = fopen(DISK, "rb");
    Inode* tmp = (Inode*)malloc(sizeof(Inode));

    fseek(fr, INODE_TABLE_BEGIN + sizeof(Inode)*no, 0);

    if(fread(tmp, sizeof(Inode), 1, fr) == 0 || tmp->delete == DELETED){
        fclose(fr);
        return NULL;
    }

    fclose(fr);
    return tmp;
}

Inode* create_inode(int flag, const char ower[USR_NAME_MAX], int power){
    FILE* fw = fopen(DISK, "rb+");
    int pos = find_empty_inode();
    Inode* inode = (Inode *)malloc(sizeof(Inode));
    inode->no = pos;
    inode->delete = NOT_DELETED;
    inode->flag = flag;
    inode->data_num = 1;

    strcpy(inode->owner, ower);
    inode->power = power;
    //获取当前时间
    time_t now;
    time(&now);
    inode->create_time = now;
    inode->update_time = now;
    inode->read_time = now;

    fseek(fw, sizeof(Inode)*pos + INODE_TABLE_BEGIN, 0);
    fwrite(inode, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);

    return inode;
}

int delete_inode(int no){
    if(no >= INODE_TABLE_SIZE)
        return 0;

    Inode* inode = find_inode(no);
    if(inode == NULL)
        return 0;

    inode->delete = DELETED;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, sizeof(Inode) * no + INODE_TABLE_BEGIN, 0);
    fwrite(inode, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
    return 1;
}

int update_inode(Inode* new){
    if(new == NULL)
        return 0;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, INODE_TABLE_BEGIN + sizeof(Inode) * new->no, 0);
    fwrite(new, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
    return 1;
}

void init_inode(){
    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, INODE_TABLE_BEGIN, 0);
    Inode* inode = (Inode*)malloc(sizeof(Inode));
    inode->delete = DELETED;
    int num = 0;
    while(num < INODE_TABLE_SIZE){
        fwrite(inode, sizeof(Inode), 1, fw);
        num ++;
    }
}




///// BLOCK
void init_data(){
    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, DATA_TABLE_BEGIN, 0);
    BlockFile *block = (BlockFile* )malloc(sizeof(BlockFile));
    block->delete = DELETED;
    int num = 0;
    while(num < DATA_TABLE_SIZE){
        fwrite(block, sizeof(BlockFile), 1, fw);
        num ++;
    }
}

//// DIR
BlockDir* find_dir_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return NULL;

    FILE* fr = fopen(DISK, "rb");
    BlockDir* tmp = (BlockDir *)malloc(sizeof(BlockDir));
    fseek(fr, DATA_TABLE_BEGIN + sizeof(BlockDir)*no, 0);

    if(fread(tmp, sizeof(BlockDir), 1, fr) == 0 || tmp->delete == DELETED)
    return NULL;

    fclose(fr);

    return tmp;
}

int delete_dir_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return 0;

    BlockDir* dir = find_dir_block(no);
    if(dir == NULL)
        return 0;

    dir->delete = DELETED;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, DATA_TABLE_SIZE + sizeof(BlockDir)*no, 0);

    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

BlockDir* create_dir_block(const char name[FILE_NAME_MAX]){
    FILE *fw = fopen(DISK, "rb+");

    int pos = find_empty_block();

    BlockDir* dir = (BlockDir *)malloc(sizeof(BlockDir));
    dir->no = pos;
    dir->flag = TYPE_DIR;
    strcpy(dir->name, name);
    dir->delete = NOT_DELETED;

    // 初始化 Pair
    int i = 0;
    while(i < FILE_SIZE_MAX){
        Pair* pair = dir->file_info + i;
        pair->delete = DELETED;
        i ++;
    }

    fseek(fw, sizeof(BlockDir)*pos + DATA_TABLE_BEGIN, 1);
    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return dir;
}

int update_dir_block(BlockDir* new){
    if(new == NULL)
        return 0;

    FILE *fw = fopen(DISK, "rb+");

    fseek(fw, sizeof(BlockDir)*new->no + DATA_TABLE_BEGIN, 1);
    fwrite(new, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

void print_dir_block(BlockDir* dir){
    printf("DIR ==> no:%ld, flag: %d, dir_name: %s, delete: %d, file: \n", dir->no, dir->flag, dir->name, dir->delete);
    print_pair(dir->file_info);
    printf("\n");
}






//// FILE
BlockFile* find_file_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return NULL;

    FILE* fr = fopen(DISK, "rb");
    BlockFile* tmp = (BlockFile *)malloc(sizeof(BlockFile));
    fseek(fr, DATA_TABLE_BEGIN + sizeof(BlockFile)*no, 0);

    if(fread(tmp, sizeof(BlockFile), 1, fr) == 0)
        return NULL;
    if(tmp->delete == DELETED)
    return NULL;

    fclose(fr);

    return tmp;
}

//FINISH
BlockFile* create_empty_file_block(){
    FILE *fw = fopen(DISK, "rb+");

    int pos = find_empty_block();

    BlockFile* file = (BlockFile *)malloc(sizeof(BlockFile));
    file->no = pos;
    file->flag = TYPE_FILE;
    file->next = DATA_TABLE_SIZE;
    file->delete = NOT_DELETED;

    fseek(fw, sizeof(BlockFile)*pos + DATA_TABLE_BEGIN, 0);
    fwrite(file, sizeof(BlockFile), 1, fw);
    fflush(fw);

    return file;
}

void delete_file_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return;

    BlockFile* file = find_file_block(no);
    if(file == NULL)
        return;

    file->delete = DELETED;

    FILE* fw = fopen(DISK, "rb+");
    fseek(fw, DATA_TABLE_BEGIN + sizeof(BlockFile)*no, 0);

    fwrite(file, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);
}

int update_file_block(BlockFile* new){
    if(new == NULL)
        return 0;

    FILE *fw = fopen(DISK, "rb+");

    fseek(fw, sizeof(BlockFile)*new->no + DATA_TABLE_BEGIN, 0);
    fwrite(new, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

void print_file_block(BlockFile* file){
    BlockFile* block = file;
    while(block != NULL){
        printf("no: %ld, flag: %d, next: %ld, delete: %d\n", file->no, file->flag, file->next, file->delete);
        block = find_file_block(block->next);
    }
    printf("file print finish\n");

}

//FINISH
Inode* create_dir(const char name[FILE_NAME_MAX], const char owner[USR_NAME_MAX], int power, Inode *father) {
    Inode *inode = create_inode(TYPE_DIR, owner, power);
    BlockDir *dir = create_dir_block(name); //FUCK
    if (inode == NULL || dir == NULL)
        return NULL;

    inode->data_begin = dir->no;

    if (update_inode(inode) == 0)
        return NULL;

    if (father != NULL) {
        BlockDir *father_dir = find_dir_block(father->no);

        Pair *pair = (Pair *) malloc(sizeof(Pair));
        pair->delete = NOT_DELETED;
        pair->inode_no = inode->no;
        strcpy(pair->file_name, name);

        if (0 == update_dir_pair(father_dir, pair))
            return NULL;

        Pair *f_pair = (Pair *) malloc(sizeof(Pair));
        f_pair->delete = NOT_DELETED;
        f_pair->inode_no = father->no;
        strcpy(f_pair->file_name, FATHER_DIR_NAME);

        if (0 == update_dir_pair(dir, f_pair))
            return NULL;
    } else {
        Pair *f_pair = (Pair *) malloc(sizeof(Pair));
        f_pair->delete = NOT_DELETED;
        f_pair->inode_no = inode->no;
        strcpy(f_pair->file_name, FATHER_DIR_NAME);

        if (0 == update_dir_pair(dir, f_pair))
            return NULL;
    }

    Pair *this_pair = (Pair *) malloc(sizeof(Pair));
    this_pair->delete = NOT_DELETED;
    this_pair->inode_no = inode->no;
    strcpy(this_pair->file_name, THIS_DIR_NAME);

    if (0 == update_dir_pair(dir, this_pair))
        return NULL;

    select_inode_bitmap(inode->no);
    select_data_bitmap(dir->no);

    print_inode(inode);
    print_dir_block(dir);

    return inode;
}

//FINISH
Inode* create_file(const char name[FILE_NAME_MAX], Inode *father) {
    if (father == NULL)
        return NULL;

    Inode *inode = create_inode(TYPE_FILE, DEFAULT_USR, 0);
    BlockFile *file = create_empty_file_block();
    if (inode == NULL || file == NULL)
        return NULL;
    inode->data_begin = file->no;
    if (0 == update_inode(inode))
        return NULL;
    // 更新目录
    BlockDir *f_block = find_dir_block(father->no);
    Pair *pair = (Pair *) malloc(sizeof(Pair));
    pair->delete = NOT_DELETED;
    strcpy(pair->file_name, name);
    pair->inode_no = inode->no;
    if (0 == update_dir_pair(f_block, pair))
        return NULL;

    // 读取文件数据
    FILE *fr = fopen(name, "rb");

    long pioneer_no = file->no;
    int time = 0;
    long len = 0;
    int block_num = 0;
    byte *b = (byte *) malloc(1);
    while (!feof(fr)) {
        BlockFile *f;
        if (time == 0) {
            f = file;
            time++;
        } else {
            f = create_empty_file_block();
            if (f == NULL) {
                fclose(fr);
                return NULL;
            }

            //更新父 data block
            BlockFile *pioneer = find_file_block(pioneer_no);
            if(pioneer == NULL)
                return NULL;
            pioneer->next = f->no;
            if (update_file_block(pioneer) == 0) {
                fclose(fr);
                return NULL;
            }
        }

        //存储数据
        int size = 0;
        byte* data = f->data;
        while (size < 1024 && fread(b, 1, 1, fr) != 0) {
            data[size] = b[0];
            size++;
            len++;
        }
        if (update_file_block(f) == 0) {
            fclose(fr);
            return NULL;
        }

        select_data_bitmap(f->no);
        pioneer_no = f->no;
        block_num ++;
    }

    fclose(fr);

    inode->size = len;
    inode->data_num = block_num;
    if(update_inode(inode) == 0)
        return NULL;

    select_inode_bitmap(inode->no);

    printf("success store file: size : %ld\n", len);
    return inode;
}

//FINISH
Inode* create_empty_file(const char name[FILE_NAME_MAX], Inode* father){
    if(father == NULL)
        return NULL;

    Inode *inode = create_inode(TYPE_FILE, DEFAULT_USR, 0);
    BlockFile *file = create_empty_file_block();
    if (inode == NULL || file == NULL)
        return NULL;
    inode->data_begin = file->no;
    if (0 == update_inode(inode))
        return NULL;
    // 更新目录
    BlockDir *f_block = find_dir_block(father->no);
    Pair *pair = (Pair *) malloc(sizeof(Pair));
    pair->delete = NOT_DELETED;
    strcpy(pair->file_name, name);
    pair->inode_no = inode->no;
    if (0 == update_dir_pair(f_block, pair))
        return NULL;

    return inode;
}


int delete_file(Inode* inode){
    if(inode == NULL)
        return 0;
    BlockFile* file = find_file_block(inode->no);
    BlockFile* tmp = file;
    while(tmp != NULL){
        tmp->delete = DELETED;
        tmp = find_file_block(tmp->next);
    }

    return delete_inode(inode->no);
}

int delete_dir(Inode* inode){
    if(inode == NULL)
        return 0;

    printf("123123\n");

    BlockDir* dir = find_dir_block(inode->data_begin);
    if(dir == NULL)
        return 0;
    Pair* pair = dir->file_info;
    int i;
    for(i = 2 ;i < FILE_SIZE_MAX;i ++){
        if(pair[i].delete == DELETED)
        break;
        Inode* node = find_inode(pair[i].inode_no);
        print_inode(node);
        if(node->flag == TYPE_FILE){
            delete_file(node);
        }else{
            delete_dir(node);
        }

        pair[i].delete = DELETED;
    }

    return delete_inode(inode->no);
}

Inode* mount() {
    printf("|||||||||||||||||||| MOUNT||||||||||||||||||||||||||||\n");
    init_super_block();
    init_inode();
    init_data();

    Inode *root = create_dir(ROOT, DEFAULT_USR, 0, NULL);

    if (root == NULL)
        return NULL;

    Inode *usr = create_dir(USR, DEFAULT_USR, 0, root);
    Inode *home = create_dir(HOME, DEFAULT_USR, 0, root);
    if (usr == NULL || home == NULL)
        return NULL;

    print_super_block();

    printf("|||||||||||||||||||| MOUNT||||||||||||||||||||||||||||\n");

    return root;
}

void ls(int no){
    Inode* inode = find_inode(no);
    if(inode->flag == TYPE_DIR){
        BlockDir *dir = find_dir_block(inode->data_begin);
        if(dir == NULL)
            return;
        print_pair(dir->file_info);
    }
}

// ERROR 无法创建多个文件夹
int mkdir1(const char name[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL || create_dir(name, DEFAULT_USR, 0, father) == NULL)
        return 0;
    return 1;

}

// ERROR 判断重名
int touch(const char name[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL || NULL == create_empty_file(name, father))
        return 0;
    return 1;
}

int rm(int no, int father){
    Inode* inode = find_inode(father);
    if(inode == NULL)
        return 0;

    BlockDir* dir1 = find_dir_block(inode->data_begin);
    if(dir1 == NULL)
        return 0;

    Pair* pair = dir1->file_info;
    int i;
    for(i = 0;i < FILE_SIZE_MAX;i ++){

        if(pair[i].inode_no == no){
            pair[i].delete =DELETED;
        }
    }
    if(update_dir_block(dir1) == 0)
        return 0;

    if(inode->flag == TYPE_FILE){
        return delete_file(inode);
    }else{
        return delete_dir(inode);
    }
}

int cd(const char path[FILE_NAME_MAX], int no){
    Inode* inode= find_inode(no);
    if(inode->flag == TYPE_DIR){
        BlockDir *dir = find_dir_block(inode->data_begin);
        if(dir == NULL)
            return -1;
        return find_pair(path, dir->file_info);
    }
    return -1;
}

int mv_in(const char file[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL)
        return 0;
    Inode* inode = create_file(file, father);
    if(inode == NULL)
        return 0;

    return inode->no;
}

int mv_out(int no, const char file[FILE_NAME_MAX]){
    Inode* inode = find_inode(no);
    if(inode == NULL)
        return 0;

    BlockFile* head = find_file_block(inode->no);
    if(head == NULL)
        return 0;

    FILE* fw = fopen(file, "wb");

    while(head != NULL){
        fwrite(head->data, 1024, 1, fw);
        fflush(fw);
        printf("%ld %ld 1\n", head->no, head->next);
        head = find_file_block(head->next);
    }

    fclose(fw);
    return 1;

}


/*
 * Class:     jni_FileJNI
 * Method:    mount
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jni_FileJNI_mount(JNIEnv *env, jobject obj){
    mount();
}

/*
 * Class:     jni_FileJNI
 * Method:    mkdir
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mkdir(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = mkdir1(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    touch
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_touch(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = touch(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    rm
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_rm(JNIEnv *env, jobject obj, jint no, jint father){
    return rm(no, father);
}

/*
 * Class:     jni_FileJNI
 * Method:    ls
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_jni_FileJNI_ls(JNIEnv *env, jobject obj, jint no){
    jclass list_clz = (*env)->FindClass(env, "Ljava/util/ArrayList;");
    if(list_clz == NULL) return NULL;

    jmethodID list_construct = (*env)->GetMethodID(env, list_clz, "<init>", "()V");
    if(list_construct == NULL) return NULL;

    jobject list_obj = (*env)->NewObject(env, list_clz, list_construct);

    jmethodID list_add = (*env)->GetMethodID(env, list_clz, "add", "(Ljava/lang/Object;)Z");
    if(list_add == NULL) return NULL;

    jclass sub_clz = (*env)->FindClass(env, "Lbase/model/Sub;");
    jmethodID sub_construct = (*env)->GetMethodID(env, sub_clz , "<init>", "(Ljava/lang/String;I)V");
    if(sub_construct == NULL) return NULL;

    Inode* inode = find_inode(no);
        if(inode == NULL || inode->flag == TYPE_FILE)
            return NULL;

    BlockDir *dir = find_dir_block(inode->data_begin);
    if(dir == NULL)
        return NULL;
    Pair* pair = dir->file_info;
    if(pair == NULL)
        return NULL;
    int i;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
            break;
        printf("FILE => no:%d, file_name: %s, inode_no: %d, delete: %d\n", i, p.file_name, p.inode_no, p.delete);
        jstring name = (*env)->NewStringUTF(env, p.file_name);
        jobject sub_obj = (*env)->NewObject(env, sub_clz, sub_construct, name, p.inode_no, NULL);
        (*env)->CallBooleanMethod(env, list_obj, list_add, sub_obj);
    }
    return list_obj;
}

/*
 * Class:     jni_FileJNI
 * Method:    cd
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cd(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = cd(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    mv_in
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv_1in(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = mv_in(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    mv_out
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv_1out(JNIEnv *env, jobject obj, jint no, jstring name){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = mv_out(no, c_name);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}
