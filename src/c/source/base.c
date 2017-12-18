//
// Created by MurphySL on 2017/12/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h/base.h"


//// 修改前驱序号
//// SUPER BLOCK 校验


////// SUPRE FINISH
void init_super_block(){
    FILE* fr = fopen(DISK, "r");
    fseek(fr, SUPER_BEGIN, 0);

    SuperBlock* tmp = (SuperBlock*)malloc(sizeof(SuperBlock));

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
    tmp->mount = NOT_MOUNTED;

    time_t now;
    time(&now);
    tmp->last_write_time = now;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, SUPER_BEGIN, 0);
    int time = 0;
    while(time < SUPER_SIZE){
        fwrite(tmp, sizeof(SuperBlock), 1, fw);
        fflush(fw);
        time ++;
    }

    fclose(fw);
    fclose(fr);
}

SuperBlock* read_super_block(){
    FILE* fr = fopen(DISK, "r");
    fseek(fr, SUPER_BEGIN, 0);
    SuperBlock* sb = (SuperBlock*)malloc(sizeof(SuperBlock));
    if(0 == fread(sb, sizeof(SuperBlock), 1, fr))
        return NULL;

    fclose(fr);
    return sb;
}

void print_super_block(){
    SuperBlock* sb = read_super_block();
    if(sb == NULL)
        return;

    printf("SUPER_BLOCK =>\n block_size : %ld b, ver: %ld, inode_area_size: %.2ld kb, data_area_size: %.2ld kb,\n "
                   "inode_begin: %.2ld, data_begin: %.2ld, inode_size:%ld, data_size: %ld, remain_inode: %ld, remain_data: %ld\n\n",
           sb->block_size, sb->ver, (sb->inode_area_size / 1024), (sb->data_area_size / 1024), (sb->inode_begin/ 1024), (sb->data_begin/ 1024),
           sb->inode_size, sb->data_size, sb->remain_inode_num, sb->remain_data_num);
}

int update_super_block(SuperBlock* sb){
    if(sb == NULL)
        return 0;

    FILE* fw = fopen(DISK, "r+");
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

void select_inode_bitmap(long no){
    if(no >= INODE_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");

    byte flag = SELECT_BIT;
    fseek(fw, INODE_BITMAP_BEGIN + sizeof(byte)*no, 0);

    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void select_data_bitmap(long no){
    if(no >= DATA_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");

    byte flag = SELECT_BIT;
    fseek(fw, DATA_BITMAP_BEGIN + sizeof(byte)*no, 0);

    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void delete_inode_bitmap(long no){
    if(no >= INODE_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");

    byte flag = DESELECT_BIT;
    fseek(fw, sizeof(byte)*no + INODE_BITMAP_BEGIN, 0);

    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

void delete_data_bitmap(long no){
    if(no >= DATA_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");

    byte flag = DESELECT_BIT;
    fseek(fw, sizeof(byte)*no + DATA_BITMAP_BEGIN, 0);
    fputc(flag, fw);
    fflush(fw);
    fclose(fw);
}

int find_inode_bitmap(long no){
    FILE *fr = fopen(DISK, "r");
    fseek(fr, sizeof(byte)*no + INODE_BITMAP_BEGIN, 0);
    byte flag = (byte) fgetc(fr);
    fclose(fr);

    if(flag == SELECT_BIT)
        return 1;

    return 0;
}

int find_data_bitmap(long no){
    FILE *fr = fopen(DISK, "r");
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
    fseek(fr, INODE_BITMAP_SIZE, 0);
    int i;
    for(i = 0 ;i < INODE_BITMAP_SIZE;i ++){

        if(fgetc(fr) == '1')
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
    for(i = 0 ;i < DATA_BITMAP_BEGIN;i ++){

        if(fgetc(fr) == '1')
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
    FILE *fr = fopen(DISK, "r");
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

void print_pair(Pair* pair){
    if(pair == NULL)
        return;
    int i;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
            return;
        printf("FILE => no:%d, file_name: %s, inode_no: %d, delete: %d\n", i, p.file_name, p.inode_no, p.delete);
    }
}

int update_dir_pair(BlockDir* dir, Pair* pair){
    if(dir == NULL || pair == NULL)
        return 0;

    int i = 0;
    while(i < FILE_SIZE_MAX){
        Pair* p = dir->file_info + i;
        if(strcmp(p->file_name, pair->file_name) == 0)
            return 0;
        if(p->delete == DELETED){
            copyPair(p, pair);
            break;
        }
        i ++;
    }

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, DATA_TABLE_BEGIN + sizeof(BlockDir)*dir->no, 0);

    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}




//// INODE
void print_inode(Inode* inode){
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
    FILE *fr = fopen(DISK, "r");
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

    FILE* fr = fopen(DISK, "r");
    Inode* tmp = (Inode*)malloc(sizeof(Inode));

    fseek(fr, INODE_TABLE_BEGIN + sizeof(Inode)*no, 0);
    if(fread(tmp, sizeof(Inode), 1, fr) == 0 || tmp->delete == DELETED)
        return NULL;

    fclose(fr);

    return tmp;
}

Inode* create_inode(int flag, char ower[USR_NAME_MAX], int power){
    FILE* fw = fopen(DISK, "r+");
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

void delete_inode(int no){
    if(no >= INODE_TABLE_SIZE)
        return;

    Inode* inode = find_inode(no);
    if(inode == NULL)
        return;

    inode->delete = DELETED;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, sizeof(Inode) * no + INODE_TABLE_BEGIN, 0);
    fwrite(inode, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
}

int update_inode(Inode* new){
    if(new == NULL)
        return 0;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, INODE_TABLE_BEGIN + sizeof(Inode) * new->no, 0);
    fwrite(new, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
    return 1;
}



//// DIR
BlockDir* find_dir_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return NULL;

    FILE* fr = fopen(DISK, "r");
    BlockDir* tmp = (BlockDir *)malloc(sizeof(BlockDir));
    fseek(fr, DATA_TABLE_BEGIN + sizeof(BlockDir)*no, 0);

    if(fread(tmp, sizeof(BlockDir), 1, fr) == 0 || tmp->delete == DELETED)
        return NULL;

    fclose(fr);

    return tmp;
}

void delete_dir_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return;

    BlockDir* dir = find_dir_block(no);
    if(dir == NULL)
        return;

    dir->delete = DELETED;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, DATA_TABLE_SIZE + sizeof(BlockDir)*no, 0);

    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);
}

BlockDir* create_dir_block(char name[FILE_NAME_MAX]){
    FILE *fw = fopen(DISK, "r+");

    int pos = find_empty_block();

    BlockDir* dir = (BlockDir *)malloc(sizeof(BlockDir));
    dir->no = pos;
    dir->flag = TYPE_DIR;
    strcpy(dir->name, name);
    dir->delete = NOT_DELETED;

    int i = 0;
    while(i < FILE_SIZE_MAX){
        Pair* pair = dir->file_info + i;
        pair->delete = DELETED;
        i ++;
    }

    fseek(fw, sizeof(BlockDir)*pos + DATA_TABLE_SIZE, 1);
    fwrite(dir, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return dir;
}

int update_dir_block(BlockDir* new){
    if(new == NULL)
        return 0;

    FILE *fw = fopen(DISK, "r+");

    fseek(fw, sizeof(BlockDir)*new->no + DATA_TABLE_SIZE, 1);
    fwrite(new, sizeof(BlockDir), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

void print_dir_block(BlockDir* dir){
    printf("DIR==> no: %ld, flag: %d, dir_name: %s, delete: %d, file: \n", dir->no, dir->flag, dir->name, dir->delete);
    print_pair(dir->file_info);
    printf("\n");
}






//// FILE
BlockFile* find_file_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return NULL;

    FILE* fr = fopen(DISK, "r");
    BlockFile* tmp = (BlockFile *)malloc(sizeof(BlockFile));
    fseek(fr, DATA_TABLE_BEGIN + sizeof(BlockFile)*no, 0);

    if(fread(tmp, sizeof(BlockFile), 1, fr) == 0 || tmp->delete == DELETED)
        return NULL;

    fclose(fr);

    return tmp;
}

BlockFile* create_empty_file_block(){
    FILE *fw = fopen(DISK, "r+");

    int pos = find_empty_block();

    BlockFile* file = (BlockFile *)malloc(sizeof(BlockFile));
    file->no = pos;
    file->flag = TYPE_FILE;
    file->delete = NOT_DELETED;

    fseek(fw, sizeof(BlockFile)*pos + DATA_TABLE_SIZE, 1);
    fwrite(file, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);

    return file;
}

BlockFile* create_file_block(const byte data[1024]){
    FILE *fw = fopen(DISK, "r+");

    int pos = find_empty_block();

    BlockFile* file = (BlockFile *)malloc(sizeof(BlockFile));
    file->no = pos;
    file->flag = TYPE_FILE;
    file->delete = NOT_DELETED;

    int len = 0;
    byte* d = file->data;
    while(len < 1024){
        *(d+len) = data[len];
        len ++;
    }

    fseek(fw, sizeof(BlockFile)*pos + DATA_TABLE_SIZE, 1);
    fwrite(file, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);

    return file;
}

void delete_file_block(long no){
    if(no >= DATA_TABLE_SIZE)
        return;

    BlockFile* file = find_file_block(no);
    if(file == NULL)
        return;

    file->delete = DELETED;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, DATA_TABLE_SIZE + sizeof(BlockFile)*no, 0);

    fwrite(file, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);
}

int update_file_block(BlockFile* new){
    if(new == NULL)
        return 0;

    FILE *fw = fopen(DISK, "r+");

    fseek(fw, sizeof(BlockFile)*new->no + DATA_TABLE_SIZE, 1);
    fwrite(new, sizeof(BlockFile), 1, fw);
    fflush(fw);
    fclose(fw);

    return 1;
}

void print_file_block(BlockFile* file){
    printf("no: %ld, flag: %d, new: %ld, delete: %d, file: ", file->no, file->flag, file->next, file->delete);
}
