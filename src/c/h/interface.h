//
// Created by MurphySL on 2017/12/18.
//


#ifndef FILESYSTEM_INTERFACE_H
#define FILESYSTEM_INTERFACE_H

#include "op.h"
#include "usr.h"

int useradd(char[USR_NAME_MAX], char[USR_PSW_MAX]);

int usermod(Usr*);

int userdel(char[USR_NAME_MAX], char[USR_PSW_MAX]);

int su(char[USR_NAME_MAX]);



void ls(int);

Inode* mount();

int mkdir1(char[FILE_NAME_MAX], int);

int touch(char[FILE_NAME_MAX], int);

int rm(int no);

int cd(char[FILE_NAME_MAX], int);

int mv_in(char[FILE_NAME_MAX], int); // 移入

int mv_out(int, char[FILE_NAME_MAX]); // 移除



#endif //FILESYSTEM_INTERFACE_H
