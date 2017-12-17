//
// Created by MurphySL on 2017/12/17.
//
#ifndef FILESYSTEM_INODE_H
#define FILESYSTEM_INODE_H

#include <time.h>
#include "base.h"



// 52 bytes
// 72 bytes inode 控制 block 太多
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


void print_inode(Inode* inode);

int find_empty_inode();

Inode* find_inode(int no);

Inode* create_inode(int flag, int size, char ower[USR_NAME_MAX], int power);

void delete_inode(int no);

int update_inode(Inode* new);




#endif //FILESYSTEM_INODE_H
