#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jni_FileJNI.h"
#include "h/interface.h"

/*
 * Class:     jni_FileJNI
 * Method:    mount
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jni_FileJNI_mount(JNIEnv *env, jobject obj){
    mount();
}

/*
 * Class:     jni_FileJNI
 * Method:    mkdir
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mkdir(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = mkdir1(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    touch
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_touch(JNIEnv *env, jobject obj, jstring name, jint no){
     const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
     int result = touch(c_name, no);
     (*env)->ReleaseStringUTFChars(env, name, c_name);
     return result;
}

/*
 * Class:     jni_FileJNI
 * Method:    rm
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_rm(JNIEnv *env, jobject obj, jint no){
    return rm(no);
}

/*
 * Class:     jni_FileJNI
 * Method:    ls
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_jni_FileJNI_ls(JNIEnv *env, jobject obj, jint no){
    jclass list_clz = (*env)->FindClass(env, "Ljava/util/ArrayList;");
    if(list_clz == NULL) return NULL;

    jmethodID list_construct = (*env)->GetMethodID(env, list_clz, "<init>", "()V");
    if(list_construct == NULL) return NULL;

    jobject list_obj = (*env)->NewObject(env, list_clz, list_construct);

    jmethodID list_add = (*env)->GetMethodID(env, list_clz, "add", "(Ljava/lang/Object;)Z");
    if(list_add == NULL) return NULL;

    jclass sub_clz = (*env)->FindClass(env, "Lbase/model/Sub;");
    jmethodID sub_construct = (*env)->GetMethodID(env, sub_clz , "<init>", "(Ljava/lang/String;I)V");
    if(sub_construct == NULL) return NULL;

    Inode* inode = find_inode(no);
    if(inode == NULL || inode->false == TYPE_FILE)
        return NULL;

    BlockDir *dir = find_dir_block(inode->data_begin);
    if(dir == NULL)
        return NULL;
    Pair* pair = dir->file_info;
    if(pair == NULL)
        return NULL;
    int i;
    for(i = 0 ;i < FILE_SIZE_MAX;i ++){
        Pair p = pair[i];
        if(pair[i].delete == DELETED)
            return;
        printf("FILE => no:%d, file_name: %s, inode_no: %d, delete: %d\n", i, p.file_name, p.inode_no, p.delete);
        jstring name = (*env)->NewStringUTF(env, p.file_name);
        jobject sub_obj = (*env)->NewObject(env, sub->clz, sub->sub_construct, name, p.inode_no, NULL);
        (*env)->CallBooleanMethod(env, list_obj, list_add, sub_obj);
    }
    return list_obj;
}

/*
 * Class:     jni_FileJNI
 * Method:    cd
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cd(JNIEnv *env, jobject obj, jstring name, jint no){
    const char* c_name = (*env)->GetStringUTFChars(env, name, NULL);
    int result = cd(c_name, no);
    (*env)->ReleaseStringUTFChars(env, name, c_name);
    return result;
}


/*
 * Class:     jni_FileJNI
 * Method:    mv
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv__Ljava_lang_String_2I
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     jni_FileJNI
 * Method:    mv
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     jni_FileJNI
 * Method:    mv
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv__Ljava_lang_String_2Ljava_lang_String_2
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     jni_FileJNI
 * Method:    mv
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mv__ILjava_lang_String_2
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     jni_FileJNI
 * Method:    cp
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cp__Ljava_lang_String_2I
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     jni_FileJNI
 * Method:    cp
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cp__II
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     jni_FileJNI
 * Method:    cp
 * Signature: (ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cp__ILjava_lang_String_2
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     jni_FileJNI
 * Method:    find
 * Signature: (Ljava/lang/String;)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_jni_FileJNI_find
  (JNIEnv *, jobject, jstring);