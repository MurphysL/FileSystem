//
// Created by MurphySL on 2017/12/18.
//

#include "base.h"

#ifndef FILESYSTEM_OP_H
#define FILESYSTEM_OP_H

Inode* create_dir(char name[FILE_NAME_MAX], char owner[USR_NAME_MAX], int power, Inode *father);

Inode* create_file(char name[FILE_NAME_MAX], Inode *father); // 移入外部文件

Inode* create_empty_file(char name[FILE_NAME_MAX], Inode* father); // 创建新文件

int delete_file(Inode*);

int delete_dir(Inode*);

#endif //FILESYSTEM_OP_H
