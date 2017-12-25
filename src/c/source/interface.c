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






Inode* mount() {
    init_super_block();
    init_inode();
    init_data();

    Inode *root = create_dir(ROOT, DEFAULT_USR, 0, NULL);

    print_super_block();

    if (root == NULL)
        return NULL;

    Inode *usr = create_dir(USR, DEFAULT_USR, 0, root);
    Inode *home = create_dir(HOME, DEFAULT_USR, 0, root);
    if (usr == NULL || home == NULL)
        return NULL;

    print_super_block();

    return root;
}

void ls(int no){
    Inode* inode = find_inode(no);
    if(inode->flag == TYPE_DIR){
        BlockDir *dir = find_dir_block(inode->data_begin);
        if(dir == NULL)
            return;
        print_pair(dir->file_info);
    }
}

int mkdir1(char name[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL || create_dir(name, DEFAULT_USR, 0, father) == NULL){
        return 0;
    }else{
        return 1;
    }
}

int touch(char name[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL || access(name, 0))
        return 0;
    if(NULL == create_file(name, father))
        return 0;
    return 1;
}

int rm(int no){
    Inode* inode = find_inode(no);
    if(inode == NULL)
        return 0;

    if(inode->flag == TYPE_FILE){
        return delete_file(inode);
    }else{
        return delete_dir(inode);
    }
}

int cd(char path[FILE_NAME_MAX], int no){
    Inode* inode= find_inode(no);
    if(inode->flag == TYPE_DIR){
        BlockDir *dir = find_dir_block(inode->data_begin);
        if(dir == NULL)
            return -1;
        return find_pair(path, dir->file_info);
    }
    return -1;
}

int mv_in(char file[FILE_NAME_MAX], int no){
    Inode* father = find_inode(no);
    if(father == NULL)
        return 0;
    Inode* inode = create_file(file, father);
    if(inode == NULL)
        return 0;

    return inode->no;
}

int mv_out(int no, char file[FILE_NAME_MAX]){
    Inode* inode = find_inode(no);
    if(inode == NULL)
        return 0;

    BlockFile* head = find_file_block(inode->no);
    if(head == NULL)
        return 0;

    FILE* fw = fopen(file, "wb");

    while(head != NULL){
        fwrite(head->data, 1024, 1, fw);
        fflush(fw);
        printf("%ld %ld 1\n", head->no, head->next);
        head = find_file_block(head->next);
    }
    printf("2345\n");

    fclose(fw);
    return 1;

}




