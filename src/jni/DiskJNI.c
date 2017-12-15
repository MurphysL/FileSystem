#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jni_DiskJNI.h"

#define DISK "disk"

#define USR_AREA_SIZE 1024
#define USR_AREA_BEGIN 0
#define USR_SIZE 25

#define SUPER_AREA_SIZE 512
#define SUPER_BEGIN 1024
#define SUPER_SIZE 9

#define INODE_BITMAP_AREA_SIZE 1024
#define INODE_BITMAP_BEGIN 2048
#define INODE_BITMAP_SIZE 1024

#define DATA_BITMAP_AREA_SIZE 10240
#define DATA_BITMAP_BEGIN 3096
#define DATA_BITMAP_SIZE 10240

#define INODE_TABLE_AREA_SIZE 51200
#define INODE_TABLE_BEGIN 13336
#define INODE_TABLE_SIZE 984

#define DATA_TABLE_AREA_SIZE 10421760
#define DATA_TABLE_BEGIN 64536
#define DATA_TABLE_SIZE 9578

#define FILE_NAME_MAX 56

typedef unsigned char byte;

////
// WRAN:
// 3.用户权限
// 4.用户修改
// 5.用户区读取边界
// 6.用户一次性存储 520bytes 后异常
///

// 60 bytes
typedef struct Pair{
    char file_name[FILE_NAME_MAX];
    long inode_no;
}Pair;

// 40 bytes, largest 25 usr
typedef struct Usr{
    char name[12];
    char psw[12];
    short flag; // 是否删除
    short su;   // 用户类型
    struct Usr* next;
}Usr;

// 56 bytes
typedef struct SuperBlock{
    int mount;
    long ver;
    long block_size;
    long inode_size;
    long inode_begin;
    long data_begin;
    long data_size;
    long inode_num;
    long data_num;
    long remain_inode_num;
    long remain_data_num;
    long mount_time;
    long last_write_time;
    int valid;
}SuperBlock;

// 52 bytes
typedef struct Inode{
    int no;
    long size;
    int flag;
    long data_num;
    long data_begin;
    char owner[12];
    int power;
    long create_time;
    long update_time;
    long read_time;
    int delete;
}Inode;

// 1088 bytes
typedef struct BlockFile{
    long no;
    int flag;
    byte data[1024];
    long next;
    byte fill[48]; // 填充位
    int delete;
}BlockFile;

// 1088 bytes
typedef struct BlockDir{
    long no;
    int flag;
    char name[FILE_NAME_MAX];
    Pair file_info[17];
    int delete;
}BlockDir;

int write_usr(Usr* usr){
    Usr* head = (Usr *)malloc(sizeof(Usr));
    int area_num = 0;
    int white_area = -1;
    FILE *fr = fopen(DISK, "r+");
    FILE *fw = fopen(DISK, "r+");

    while(area_num < USR_SIZE && fread(head, sizeof(Usr), 1, fr) != 0){
        if(head->flag == 0 && white_area == -1)
            white_area = area_num;

        if(!strcmp(usr->name, head->name) || area_num >= USR_SIZE) {
            fclose(fw);
            fclose(fr);
            return 0;
        }

        area_num ++;
    }

    if(white_area != -1)
        fseek(fw, sizeof(Usr)*white_area, 0);
    else
        fseek(fw, ftell(fr), 0);
    fwrite(usr, sizeof(Usr), 1, fw);
    fflush(fw);
    fclose(fw);
    fclose(fr);

    printf("jni insert: finish ");

    return 1;
}

void delete_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r+");
    FILE* fw = fopen(DISK, "r+");
    int area_num = 0;

    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr) != 0){
        if(strcmp(usr->name, tmp->name) == 0 && strcmp(usr->psw, tmp->psw) == 0 && tmp->flag == 1){
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
    fclose(fr);
    fclose(fw);
}

int find_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r");
    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    int area_num = 0;
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr) != 0)
        if(0 == strcmp(usr->name, tmp->name) && 0 == strcmp(usr->psw, tmp->psw) && tmp->flag == 1)
            return 1;

    fclose(fr);
    return 0;
}

int update_usr(Usr* usr){
    FILE* fr = fopen(DISK, "r");
    FILE* fw = fopen(DISK, "r+");
    Usr* tmp = (Usr*)malloc(sizeof(Usr));
    int area_num = 0;
    while(area_num < USR_SIZE && fread(tmp, sizeof(Usr), 1, fr) != 0)
        if(0 == strcmp(usr->name, tmp->name) && tmp->flag == 1) {
            strcpy(tmp->psw, usr->psw);
            fwrite(tmp, sizeof(Usr), 1, fw);
            fflush(fw);
            fclose(fr);
            fclose(fw);
            return 1;
        }
    fclose(fr);
    fclose(fw);
    return 0;
}

/*
 * Class:     jni_DiskJNI
 * Method:    insertUsr
 * Signature: (Lbase/model/Usr;)I
 */
JNIEXPORT jint JNICALL Java_jni_DiskJNI_insertUsr (JNIEnv *env, jobject obj, jobject obj_usr){
    jclass clz = (*env)->GetObjectClass(env, obj_usr);
    if(clz == NULL) return 0;

    jfieldID suFieldID = (*env)->GetFieldID(env, clz, "su", "S");
    if(suFieldID == NULL)  return 0;
    jfieldID pswFieldID = (*env)->GetFieldID(env, clz, "psw", "Ljava/lang/String;");
    if(pswFieldID == NULL) return 0;
    jfieldID nameFieldID = (*env)->GetFieldID(env, clz, "name", "Ljava/lang/String;");
    if(nameFieldID == NULL) return 0;

    jstring name = (*env)->GetObjectField(env, obj_usr, nameFieldID);
    const char *c_name = (*env)->GetStringUTFChars(env, name, NULL);

    jstring psw = (*env)->GetObjectField(env, obj_usr, pswFieldID);
    const char *c_psw = (*env)->GetStringUTFChars(env, psw, NULL);

    jshort su = (*env)->GetShortField(env, obj_usr, suFieldID);

    printf("jni insert: su:%d name:%s psw:%s\n", su, c_name, c_psw);

    Usr usr;
    strcpy(usr.name, c_name);
    strcpy(usr.psw, c_psw);
    usr.su = su;
    usr.flag = 1;
    usr.next = NULL;

    (*env)->ReleaseStringUTFChars(env, name, c_name);
    (*env)->ReleaseStringUTFChars(env, psw, c_psw);

    return write_usr(&usr);
}

/*
 * Class:     jni_DiskJNI
 * Method:    deleteUsr
 * Signature: (Lbase/model/Usr;)V
 */
JNIEXPORT void JNICALL Java_jni_DiskJNI_deleteUsr (JNIEnv *env, jobject obj, jobject obj_usr){
    jclass clz = (*env)->GetObjectClass(env, obj_usr);
    if(clz == NULL) return;

    jfieldID pswFieldID = (*env)->GetFieldID(env, clz, "psw", "Ljava/lang/String;");
    if(pswFieldID == NULL) return;
    jfieldID nameFieldID = (*env)->GetFieldID(env, clz, "name", "Ljava/lang/String;");
    if(nameFieldID == NULL) return;

    jstring name = (*env)->GetObjectField(env, obj_usr, nameFieldID);
    const char *c_name = (*env)->GetStringUTFChars(env, name, NULL);

    jstring psw = (*env)->GetObjectField(env, obj_usr, pswFieldID);
    const char *c_psw = (*env)->GetStringUTFChars(env, psw, NULL);

    Usr usr;
    strcpy(usr.name, c_name);
    strcpy(usr.psw, c_psw);
    usr.next = NULL;

    (*env)->ReleaseStringUTFChars(env, name, c_name);
    (*env)->ReleaseStringUTFChars(env, psw, c_psw);

    return delete_usr(&usr);
}

/*
 * Class:     jni_DiskJNI
 * Method:    updateUsr
 * Signature: (Lbase/model/Usr;)I
 */
JNIEXPORT jint JNICALL Java_jni_DiskJNI_updateUsr (JNIEnv *env, jobject obj, jobject obj_usr){
    jclass clz = (*env)->GetObjectClass(env, obj_usr);
    if(clz == NULL) return 0;

    jfieldID pswFieldID = (*env)->GetFieldID(env, clz, "psw", "Ljava/lang/String;");
    if(pswFieldID == NULL) return 0;
    jfieldID nameFieldID = (*env)->GetFieldID(env, clz, "name", "Ljava/lang/String;");
    if(nameFieldID == NULL) return 0;

    jstring name = (*env)->GetObjectField(env, obj_usr, nameFieldID);
    const char *c_name = (*env)->GetStringUTFChars(env, name, NULL);

    jstring psw = (*env)->GetObjectField(env, obj_usr, pswFieldID);
    const char *c_psw = (*env)->GetStringUTFChars(env, psw, NULL);

    Usr usr;
    strcpy(usr.name, c_name);
    strcpy(usr.psw, c_psw);
    usr.next = NULL;

    (*env)->ReleaseStringUTFChars(env, name, c_name);
    (*env)->ReleaseStringUTFChars(env, psw, c_psw);

    return update_usr(&usr);
}


/*
 * Class:     jni_DiskJNI
 * Method:    selectUsr
 * Signature: (Lbase/model/Usr;)I
 */
JNIEXPORT jint JNICALL Java_jni_DiskJNI_selectUsr (JNIEnv *env, jobject obj, jobject obj_usr){
    jclass clz = (*env)->GetObjectClass(env, obj_usr);
    if(clz == NULL) return 0;

    jfieldID pswFieldID = (*env)->GetFieldID(env, clz, "psw", "Ljava/lang/String;");
    if(pswFieldID == NULL) return 0;
    jfieldID nameFieldID = (*env)->GetFieldID(env, clz, "name", "Ljava/lang/String;");
    if(nameFieldID == NULL) return 0;

    jstring name = (*env)->GetObjectField(env, obj_usr, nameFieldID);
    const char *c_name = (*env)->GetStringUTFChars(env, name, NULL);

    jstring psw = (*env)->GetObjectField(env, obj_usr, pswFieldID);
    const char *c_psw = (*env)->GetStringUTFChars(env, psw, NULL);

    Usr usr;
    strcpy(usr.name, c_name);
    strcpy(usr.psw, c_psw);
    usr.next = NULL;

    (*env)->ReleaseStringUTFChars(env, name, c_name);
    (*env)->ReleaseStringUTFChars(env, psw, c_psw);

    return find_usr(&usr);
}


/*
 * Class:     jni_DiskJNI
 * Method:    selectAllUsr
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_jni_DiskJNI_selectAllUsr (JNIEnv *env, jobject obj){
    jclass list_clz = (*env)->FindClass(env, "Ljava/util/ArrayList;");
    if(list_clz == NULL) return NULL;

    jmethodID list_construct = (*env)->GetMethodID(env, list_clz, "<init>", "()V");
    if(list_construct == NULL) return NULL;

    jobject list_obj = (*env)->NewObject(env, list_clz, list_construct);

    jmethodID list_add = (*env)->GetMethodID(env, list_clz, "add", "(Ljava/lang/Object;)Z");
    if(list_add == NULL) return NULL;

    jclass usr_clz = (*env)->FindClass(env, "Lbase/model/Usr;");
    jmethodID usr_construct = (*env)->GetMethodID(env, usr_clz , "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if(usr_construct == NULL) return NULL;

    Usr* head = (Usr *)malloc(sizeof(Usr));
    FILE *file = fopen(DISK, "r");
    int area_num = 0;

    while(area_num < USR_SIZE && fread(head, sizeof(Usr), 1, file) != 0){
        area_num ++;
        if(head->flag != 0){
            printf("name : %s psw : %s flag : %d\n", head->name, head->psw, head->flag);
            jstring name = (*env)->NewStringUTF(env, head->name);
            jstring psw = (*env)->NewStringUTF(env, head->psw);
            jobject usr_obj = (*env)->NewObject(env, usr_clz, usr_construct, name, psw, NULL);
            (*env)->CallBooleanMethod(env, list_obj, list_add, usr_obj);
        }
    }
    fclose(file);
    printf("file area num : %d\n", area_num);

    return list_obj;
}















