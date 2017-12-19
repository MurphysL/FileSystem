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
void select_inode_bitmap(int no);

void select_data_bitmap(long no);

void delete_inode_bitmap(int no);

void delete_data_bitmap(long no);

long get_inode_bitmap_num();

long get_data_bitmap_num();


///// COMMON
void copyPair(Pair* new, Pair* origin);

int find_empty_block();

void print_pair(Pair*);

int update_dir_pair(BlockDir* block, Pair* pair);