//
// Created by MurphySL on 2017/12/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h/block.h"

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
    if(sb != NULL)
        printf("block_size : %ld, ver: %ld, inode_area_size: %ld, data_area_size: %ld, inode_begin: %ld, data_begin: %ld\n"
                       "inode_size:%ld, data_size: %ld, remain_inode: %ld, remain_data: %ld",
               sb->block_size, sb->ver, sb->inode_area_size, sb->data_area_size, sb->inode_begin, sb->data_begin,
        sb->inode_size, sb->data_size, sb->remain_inode_num, sb->remain_data_num);
}

int update_super_block(SuperBlock* sb){
    if(sb == NULL)
        return 0;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, sizeof(SuperBlock), 0);
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
    FILE *fr = fopen(DISK, "r");
    fseek(fr, DATA_BITMAP_SIZE, 0);
    int i;
    for(i = 0 ;i < DATA_BITMAP_SIZE;i ++){
        if(i % 10 == 0)
            printf("\n");
        printf("%c", fgetc(fr));
    }
    fclose(fr);
}

void print_inode_bitmap(){
    FILE *fr = fopen(DISK, "r");
    fseek(fr, INODE_BITMAP_SIZE, 0);
    int i;
    for(i = 0 ;i < INODE_BITMAP_SIZE;i ++){
        if(i % 10 == 0)
            printf("\n");
        printf("%c", fgetc(fr));
    }
    fclose(fr);
}

void change_inode_bitmap(long no){
    if(no >= INODE_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");
    FILE *fr = fopen(DISK, "r");

    byte flag;
    fseek(fr, sizeof(byte), 1);
    flag = (byte) fgetc(fw);
    fseek(fw, sizeof(byte), 1);

    if(flag == '0')
        flag = '1';
    else
        flag = '0';

    fputc(flag, fw);
    fflush(fw);
    fclose(fr);
    fclose(fw);
}

void change_data_bitmap(long no){
    if(no >= DATA_BITMAP_SIZE)
        return;
    FILE *fw = fopen(DISK, "r+");
    FILE *fr = fopen(DISK, "r");

    byte flag;
    fseek(fr, sizeof(byte), 1);
    flag = (byte) fgetc(fw);
    fseek(fw, sizeof(byte), 1);

    if(flag == '0')
        flag = '1';
    else
        flag = '0';

    fputc(flag, fw);
    fflush(fw);
    fclose(fr);
    fclose(fw);
}



///// COMMON

void copyPair(Pair* new, Pair* origin){
    int i;
    for(i = 0; i < FILE_SIZE_MAX;i ++){
        new->inode_no = origin->inode_no;
        strcpy(new->file_name, origin->file_name);
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
    printf("no: %ld, flag: %d, dir_name: %s, delete: %d, file: ", dir->no, dir->flag, dir->name, dir->delete);
    int i;
    Pair* pair = dir->file_info;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
            return;
        printf("file%d: file_name: %s, inode_no: %d", i, p.file_name, p.inode_no);
    }
    printf("\n");
}

//// BLOCK

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

BlockFile* create_file_block(byte data[1024]){
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




