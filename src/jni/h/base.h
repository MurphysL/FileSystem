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