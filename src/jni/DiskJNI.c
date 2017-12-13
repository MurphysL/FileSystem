#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jni_DiskJNI.h"

#define DISK "disk"
#define USR_AREA_SIZE 40
#define USR_SIZE 25

////
// WRAN:
// 1.未对最大人数越界反馈
// 2.重名限制
// 3.用户权限
///

typedef struct Usr{
    char name[20];
    char psw[20];
    short su;
    struct Usr* next;
}Usr;

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
    jmethodID usr_construct = (*env)->GetMethodID(env, usr_clz , "<init>", "(ILjava/lang/String;)V");
    if(usr_construct == NULL) return NULL;

    Usr* head = (Usr *)malloc(sizeof(Usr));
    FILE *file = fopen(DISK, "r");

    while(fread(head, sizeof(Usr), 1, file) != 0){
        jstring name = (*env)->NewStringUTF(env, head->name);
        jstring psw = (*env)->NewStringUTF(env, head->psw);
        jobject usr_obj = (*env)->NewObject(env, usr_clz, usr_construct, name, psw, NULL);
        (*env)->CallBooleanMethod(env, list_obj, list_add, usr_obj);
    }

    return list_obj;
}

/*
 * Class:     jni_DiskJNI
 * Method:    insertUsr
 * Signature: (Lbase/model/Usr;)V
 */
JNIEXPORT void JNICALL Java_jni_DiskJNI_insertUsr (JNIEnv *env, jobject obj, jobject obj_usr){
    jclass clz = (*env)->GetObjectClass(env, obj_usr);
    if(clz == NULL) return;

    jfieldID suFieldID = (*env)->GetFieldID(env, clz, "su", "S");
    if(suFieldID == NULL)  return;
    jfieldID pswFieldID = (*env)->GetFieldID(env, clz, "psw", "Ljava/lang/String;");
    if(pswFieldID == NULL) return;
    jfieldID nameFieldID = (*env)->GetFieldID(env, clz, "name", "Ljava/lang/String;");
    if(nameFieldID == NULL) return;

    jstring name = (*env)->GetObjectField(env, obj_usr, nameFieldID);
    const char *c_name = (*env)->GetStringUTFChars(env, name, NULL);

    jstring psw = (*env)->GetObjectField(env, obj_usr, pswFieldID);
    const char *c_psw = (*env)->GetStringUTFChars(env, psw, NULL);

    jshort su = (*env)->GetShortField(env, obj_usr, suFieldID);

    printf("jni: su:%d name:%s psw:%s", su, c_name, c_psw);

    Usr usr;
    strcpy(usr.name, c_name);
    strcpy(usr.psw, c_psw);
    usr.su = su;
    usr.next = NULL;

    (*env)->ReleaseStringUTFChars(env, name, c_name);
    (*env)->ReleaseStringUTFChars(env, psw, c_psw);

    Usr* head = (Usr *)malloc(sizeof(Usr));
    int file_num = 0;
    FILE *fr = fopen(DISK, "r+");
    FILE *fw = fopen(DISK, "r+");

    if(fread(head, sizeof(Usr), 1, fr) == 0){
        fwrite(&usr, sizeof(Usr), 1, fw);
        fflush(fw);
        return;
    }else{
        file_num ++;
    }

    while(fread(head, sizeof(Usr), 1, fr) != 0){
        file_num ++;
        if(file_num >= USR_SIZE)
            return;
    }

    //find pioneer
    fseek(fr, sizeof(Usr)*(file_num - 1), 0);
    fseek(fw, sizeof(Usr)*(file_num - 1), 0);
    fread(head, sizeof(Usr), 1, fr);
    head->next = &usr;

    fwrite(head, sizeof(Usr), 1, fw);
    fflush(fw);

    fwrite(&usr, sizeof(Usr), 1, fw);
    fflush(fw);

    fclose(fr);
    fclose(fw);
}
















