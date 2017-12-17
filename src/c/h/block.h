//
// Created by MurphySL on 2017/12/17.
//


#ifndef FILESYSTEM_DATA_H
#define FILESYSTEM_DATA_H

#include "base.h"

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
    long no;
    int flag;
    byte data[1024];
    long next;
    byte fill[44]; // 填充位
    int delete;
}BlockFile;

// 1084 bytes
typedef struct BlockDir{
    long no;
    int flag;
    char name[FILE_NAME_MAX];
    Pair file_info[FILE_SIZE_MAX];
    int delete;
}BlockDir;


////// SUPER
void init_super_block();

SuperBlock* read_super_block();

void print_super_block();



////// BITMAP
int update_super_block(SuperBlock* sb);

void change_inode_bitmap(long no);

void change_data_bitmap(long no);

void print_inode_bitmap();

void print_data_bitmap();


///// COMMON
void copyPair(Pair* new, Pair* origin);

int find_empty_block();



//// DIR
void print_dir_block(BlockDir* dir);

BlockDir* create_dir_block(char name[FILE_NAME_MAX]);

void delete_dir_block(long no);

int update_dir_block(BlockDir* new);

BlockDir* find_dir_block(long no);



//// FILE
void print_file_block(BlockFile* file);

BlockFile* create_file_block(byte data[1024]);

void delete_file_block(long no);

int update_file_block(BlockFile* file);

BlockFile* find_file_block(long no);


#endif //FILESYSTEM_DATA_H
