//
// Created by MurphySL on 2017/12/17.
//
#include "time.h"

#ifndef FILESYSTEM_BASE_H
#define FILESYSTEM_BASE_H

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

int find_pair(char[FILE_NAME_MAX], Pair* pair);





////// INODE
void print_inode(Inode* inode);

int find_empty_inode();

Inode* find_inode(int no);

Inode* create_inode(int flag, char ower[USR_NAME_MAX], int power);

int delete_inode(int no);

int update_inode(Inode* new);

void init_inode();


////DATA
void init_data();


//// DIR
void print_dir_block(BlockDir* dir);

void print_dir_file(BlockDir* dir);

BlockDir* create_dir_block(char name[FILE_NAME_MAX]);

int delete_dir_block(long no);

int update_dir_block(BlockDir* new);

BlockDir* find_dir_block(long no);

BlockDir* get_root_block();



//// FILE
void print_file_block(BlockFile* file);

BlockFile* create_empty_file_block();

BlockFile* create_file_block(const byte data[1024]);

void delete_file_block(long no);

int update_file_block(BlockFile* file);

BlockFile* find_file_block(long no);

#endif //FILESYSTEM_BASE_H
