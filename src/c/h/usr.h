//
// Created by MurphySL on 2017/12/17.
// Finished by MurphySL on 2017/12/18
//
// 待修改 => /etc/passwd 文件
//



#ifndef FILESYSTEM_USR_H
#define FILESYSTEM_USR_H

#include "base.h"

// 40 bytes
typedef struct Usr{
    char name[USR_NAME_MAX];
    char psw[USR_PSW_MAX];
    short flag; // 是否删除
    short su;   // 用户类型
    struct Usr* next;
}Usr;

void read_all_usr();

int write_usr(Usr* usr);

void delete_usr(Usr* usr);

int update_usr(Usr* usr);

int find_usr(Usr* usr);

#endif //FILESYSTEM_USR_H
