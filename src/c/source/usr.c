//
// Created by MurphySL on 2017/12/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../h/usr.h"

void read_all_usr(){
    Usr* head = (Usr *)malloc(sizeof(Usr));
    FILE *file = fopen(DISK, "r");
    int area_num = 0;

    while(area_num < USR_SIZE && fread(head, sizeof(Usr), 1, file) != 0){
        printf("read : %ld area_num : %d\n", ftell(file), area_num);
        area_num ++;
        if(head->flag != 0)
            printf("name : %s psw : %s flag : %d\n", head->name, head->psw, head->flag);
    }
    fclose(file);
}

int write_usr(Usr* usr){
    Usr* head = (Usr *)malloc(sizeof(Usr));
    int area_num = 0;
    int white_area = -1;
    FILE *fr = fopen(DISK, "r+");
    FILE *fw = fopen(DISK, "r+");

    while(area_num < USR_SIZE && fread(head, sizeof(Usr), 1, fr) != 0){
        if(head->flag == 0 && white_area == -1)
            white_area = area_num;

        if(0 == strcmp(usr->name, head->name)) return 0;

        area_num ++;
    }

    if(white_area != -1){
        fseek(fw, sizeof(Usr)*white_area, 0);
    }else{
        fseek(fw, ftell(fr), 0);
    }

    printf("write before : %ld white_area : %ld\n", ftell(fw), white_area* sizeof(Usr));
    fwrite(usr, sizeof(Usr), 1, fw);
    printf("write after : %ld\n", ftell(fw));
    fflush(fw);
    fclose(fw);
    fclose(fr);

    return 1;
}

void delete_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r+");
    FILE* fw = fopen(DISK, "r+");
    int area_num = 0;

    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr)){
        if(strcmp(usr->name, tmp->name) == 0 && tmp->flag == 1){
            tmp->flag = 0;
            fseek(fw, area_num * sizeof(Usr), 1);
            fwrite(tmp, sizeof(Usr), 1, fw);
            fflush(fw);
            fclose(fr);
            fclose(fw);
            return;
        }
        area_num ++;
    }
}

int update_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r");
    FILE* fw = fopen(DISK, "r+");
    int area_num = 0;

    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr)){
        if(0 == strcmp(usr->name, tmp->name) && tmp->flag == 1) {
            strcpy(tmp->psw, usr->psw);
            fwrite(tmp, sizeof(Usr), 1, fw);
            fflush(fw);
            fclose(fr);
            fclose(fw);
            return 1;
        }
        area_num ++;
    }
    fclose(fr);
    fclose(fw);
    return 0;
}

int find_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r");
    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    int area_num = 0;
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr) != 0){
        if(0 == strcmp(usr->name, tmp->name) && 0 == strcmp(usr->psw, tmp->psw) && tmp->flag == 1)
            return 1;
        area_num ++;
    }

    return 0;
}



