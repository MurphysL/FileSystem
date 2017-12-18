//
// Created by MurphySL on 2017/12/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h/op.h"

// WARN 中途错误 删除前面已写入数据 dir inode


Inode* create_dir(char name[FILE_NAME_MAX], char owner[USR_NAME_MAX], int power, Inode *father) {
    Inode *inode = create_inode(TYPE_DIR, owner, power);
    BlockDir *dir = create_dir_block(name);

    if (inode == NULL || dir == NULL)
        return NULL;

    inode->data_begin = dir->no;

    if (update_inode(inode) == 0)
        return NULL;

    if (father != NULL) {
        BlockDir *father_dir = find_dir_block(father->no);

        Pair *pair = (Pair *) malloc(sizeof(Pair));
        pair->delete = NOT_DELETED;
        pair->inode_no = inode->no;
        strcpy(pair->file_name, name);

        if (0 == update_dir_pair(father_dir, pair))
            return NULL;

        Pair *f_pair = (Pair *) malloc(sizeof(Pair));
        f_pair->delete = NOT_DELETED;
        f_pair->inode_no = father->no;
        strcpy(f_pair->file_name, FATHER_DIR_NAME);

        if (0 == update_dir_pair(dir, f_pair))
            return NULL;
    } else {
        Pair *f_pair = (Pair *) malloc(sizeof(Pair));
        f_pair->delete = NOT_DELETED;
        f_pair->inode_no = inode->no;
        strcpy(f_pair->file_name, FATHER_DIR_NAME);

        if (0 == update_dir_pair(dir, f_pair))
            return NULL;
    }

    Pair *this_pair = (Pair *) malloc(sizeof(Pair));
    this_pair->delete = NOT_DELETED;
    this_pair->inode_no = inode->no;
    strcpy(this_pair->file_name, THIS_DIR_NAME);

    if (0 == update_dir_pair(dir, this_pair))
        return NULL;

    select_inode_bitmap(inode->no);
    select_data_bitmap(dir->no);

    print_inode(inode);
    print_dir_block(dir);

    return inode;
}

Inode* create_file(char name[FILE_NAME_MAX], Inode *father) {
    if (father == NULL)
        return NULL;

    Inode *inode = create_inode(TYPE_FILE, DEFAULT_USR, 0);
    BlockFile *file = create_empty_file_block();

    if (inode == NULL || file == NULL)
        return NULL;

    inode->data_begin = file->no;

    if (0 == update_inode(inode))
        return NULL;

    BlockDir *f_block = find_dir_block(father->no);
    Pair *pair = (Pair *) malloc(sizeof(Pair));
    pair->delete = NOT_DELETED;
    strcpy(pair->file_name, name);
    pair->inode_no = inode->no;

    if (0 == update_dir_pair(f_block, pair))
        return NULL;

    FILE *fr = fopen(name, "rb");
    long pioneer_no = file->no;
    int time = 0;
    long len = 0;
    byte *b = (byte *) malloc(1);
    while (!feof(fr)) {
        BlockFile *f;
        printf("123");
        if (time == 0) {
            f = file;
            time++;
        } else {
            f = create_empty_file_block();
            if (f == NULL) {
                fclose(fr);
                return NULL;
            }

            BlockFile *pioneer = find_file_block(pioneer_no);
            pioneer->next = f->no;
            if (update_file_block(pioneer) == 0) {
                fclose(fr);
                return NULL;
            }
        }

        int size = 0;
        byte* data = f->data;
        while (size < 1024 && fread(b, 1, 1, fr) != 0) {
            data[size] = b[0];
            size++;
            len++;
        }

        if (update_file_block(f) == 0) {
            fclose(fr);
            return NULL;
        }
        pioneer_no = f->no;
    }

    fclose(fr);

    inode->size = len;
    int data_block_num = 0;
    if(len % 10 != 0)
        data_block_num += 1;
    inode->data_num = data_block_num;
    if(update_inode(inode) == 0)
        return NULL;

    printf("success file");
    return inode;
}

SuperBlock* update_remain_block(){
    SuperBlock* sb = read_super_block();

    sb->remain_inode_num -= get_inode_bitmap_num();
    sb->remain_data_num -= get_data_bitmap_num();
    time_t now;
    time(&now);
    sb->last_write_time = now;

    FILE* fw = fopen(DISK, "r+");
    fseek(fw, SUPER_BEGIN, 0);
    int time = 0;
    while(time < SUPER_SIZE){
        fwrite(sb, sizeof(SuperBlock), 1, fw);
        fflush(fw);
        time ++;
    }
    fclose(fw);
}