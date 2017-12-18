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

void ls(BlockDir*);

Inode* mount();

int mkdir1(char[FILE_NAME_MAX], Inode*);

int touch(char[FILE_NAME_MAX], Inode*);

int rm();

Inode* cd(char[1000], Inode*);



#endif //FILESYSTEM_INTERFACE_H
