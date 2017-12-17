//
// Created by MurphySL on 2017/12/17.
//

#include "base.h"
#include "inode.h"

#ifndef FILESYSTEM_DATA_H
#define FILESYSTEM_DATA_H

int mount();

int create_dir(char name[FILE_NAME_MAX], Inode* father);

int create_file(char name[FILE_NAME_MAX], Inode* father);


#endif //FILESYSTEM_DATA_H
