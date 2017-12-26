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

int mkdir1(const char[FILE_NAME_MAX], int);

int touch(const char[FILE_NAME_MAX], int);

int rm(int no, int father);

int cd(const char[FILE_NAME_MAX], int);

int mv_in(const char[FILE_NAME_MAX], int); // 移入

int mv_out(int, const char[FILE_NAME_MAX]); // 移除



#endif //FILESYSTEM_INTERFACE_H
