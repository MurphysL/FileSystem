//
// Created by MurphySL on 2017/12/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h/inode.h"

void print_inode(Inode* inode){
    struct tm *tmp_create = NULL;
    struct tm *tmp_update = NULL;
    struct tm *tmp_read = NULL;

    time_t t_create = inode->create_time;
    time_t t_update = inode->update_time;
    time_t t_read = inode->read_time;

    tmp_create = gmtime(&t_create);
    tmp_update = gmtime(&t_update);
    tmp_read = gmtime(&t_read);

    if(inode->flag == TYPE_FILE){
        printf("type: %s, no: %d, size: %ld, data_num: %ld, data_begin: %ld, owener: %s, power: %d, delete: %d, create_time: %d.%d.%d %d:%d:%d",
               "file",
               inode->no,
               inode->size,
               inode->data_num,
               inode->data_begin,
               inode->owner,
               inode->power,
               inode->delete,
               (1900+tmp_create->tm_year), (1+tmp_create->tm_mon), (tmp_create->tm_mday),tmp_create->tm_hour, tmp_create->tm_min, tmp_create->tm_sec);
    }else{
        printf("type: %s, no:%d, data_begin: %ld, owner: %s, power: %d, delete: %d, create_time:  %d.%d.%d %d:%d:%d",
               "文件",
               inode->no,
               inode->data_begin,
               inode->owner,
               inode->power,
               inode->delete,
               (1900+tmp_create->tm_year), (1+tmp_create->tm_mon), (tmp_create->tm_mday),tmp_create->tm_hour, tmp_create->tm_min, tmp_create->tm_sec);
    }
}

int find_empty_inode(){
    FILE *fr = fopen(DISK, "r");
    int block_num = 0;

    Inode* tmp = (Inode *)malloc(sizeof(Inode));
    fseek(fr, INODE_TABLE_BEGIN, 0);
    while(block_num < INODE_TABLE_SIZE && fread(tmp, sizeof(Inode), 1, fr) != 0){
        if(tmp->delete == DELETED)
            return block_num;
        block_num ++;
    }

    fclose(fr);
    return block_num;
}

Inode* find_inode(int no){
    if(no >= INODE_TABLE_SIZE)
        return  NULL;

    FILE* fr = fopen(DISK, "r");
    Inode* tmp = (Inode*)malloc(sizeof(Inode));

    fseek(fr, INODE_TABLE_BEGIN + sizeof(Inode)*no, 0);
    if(fread(tmp, sizeof(Inode), 1, fr) == 0 || tmp->delete == DELETED)
        return NULL;

    fclose(fr);

    return tmp;
}

Inode* create_inode(int flag, int size, char ower[USR_NAME_MAX], int power){
    FILE* fw = fopen(DISK, "r+");
    int pos = find_empty_inode();
    printf("find empty inode position: %d\n", pos);
    Inode* inode = (Inode *)malloc(sizeof(Inode));
    inode->no = pos;
    inode->delete = NOT_DELETED;

    if(flag == TYPE_DIR){
        inode->flag = TYPE_DIR;
        inode->data_num = 1;
    }else{
        inode->flag = TYPE_FILE;
        int data_num ;
        data_num = size / 1024;
        if(size % 1024 != 0)
            data_num += 1;
        inode->data_num = data_num;
    }

    strcpy(inode->owner, ower);
    inode->power = power;
    //获取当前时间
    time_t now;
    time(&now);
    inode->create_time = now;
    inode->update_time = now;
    inode->read_time = now;

    fseek(fw, sizeof(Inode)*pos + INODE_TABLE_BEGIN, 1);
    fwrite(inode, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);

    return inode;
}

void delete_inode(int no){
    if(no >= INODE_TABLE_SIZE)
        return;

    Inode* inode = find_inode(no);
    if(inode == NULL)
        return;

    inode->delete = DELETED;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, sizeof(Inode) * no + INODE_TABLE_BEGIN, 0);
    fwrite(inode, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
}

int update_inode(Inode* new){
    if(new == NULL)
        return 0;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, INODE_TABLE_BEGIN + sizeof(Inode) * new->no, 0);
    fwrite(new, sizeof(Inode), 1, fw);
    fflush(fw);
    fclose(fw);
    return 1;
}

