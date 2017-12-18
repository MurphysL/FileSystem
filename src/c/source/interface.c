//
// Created by MurphySL on 2017/12/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "../h/interface.h"

////1 .用户类型

// 用户权限
int useradd(char name[USR_NAME_MAX], char psw[USR_PSW_MAX]){
    Usr* usr = (Usr*)malloc(sizeof(Usr));
    strcpy(usr->name, name);
    strcpy(usr->psw, psw);
    usr->flag = NOT_DELETED;

    // home 下新建 用户文件夹
    //mkdir(usr->name, )

    return write_usr(usr);
}

int usermod(Usr* usr){
    return update_usr(usr);
}

int userdel(char name[USR_NAME_MAX], char psw[USR_PSW_MAX]){
    Usr* usr = (Usr*)malloc(sizeof(Usr));
    strcpy(usr->name, name);
    strcpy(usr->psw, psw);
    if(find_usr(usr) == 0)
        return 0;
    delete_usr(usr);
    return 1;
}

int su(char name[USR_NAME_MAX]){

}

void ls(BlockDir* dir){
    if(dir == NULL)
        return;
    print_pair(dir->file_info);
}


Inode* mount() {
    init_super_block();
    Inode *root = create_dir(ROOT, DEFAULT_USR, 0, NULL);

    if (root == NULL)
        return NULL;

    Inode *usr = create_dir(USR, DEFAULT_USR, 0, root);
    Inode *home = create_dir(HOME, DEFAULT_USR, 0, root);
    if (usr == NULL || home == NULL)
        return root;

    update_remain_block();

    print_super_block();

    return root;
}

int mkdir1(char name[FILE_NAME_MAX], Inode* father){
    if(create_dir(name, DEFAULT_USR, 0, father) == NULL){
        return 0;
    }else{
        return 1;
    }
}

int touch(char name[FILE_NAME_MAX], Inode* father){
    if(access(name, 0))
        return 0;
    if(NULL == create_file(name, father))
        return 0;
    return 1;
}

int rm(){}

Inode* cd(char path[1000], Inode* father){

}

