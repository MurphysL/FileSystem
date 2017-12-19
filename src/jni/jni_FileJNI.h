/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jni_FileJNI */

#ifndef _Included_jni_FileJNI
#define _Included_jni_FileJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     jni_FileJNI
 * Method:    mount
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jni_FileJNI_mount
  (JNIEnv *, jobject);

/*
 * Class:     jni_FileJNI
 * Method:    mkdir
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_mkdir
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     jni_FileJNI
 * Method:    touch
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_touch
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     jni_FileJNI
 * Method:    rm
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_rm
  (JNIEnv *, jobject, jint);

/*
 * Class:     jni_FileJNI
 * Method:    ls
 * Signature: (I)Ljava/util/List;
 */
JNIEXPORT jobject JNICALL Java_jni_FileJNI_ls
  (JNIEnv *, jobject, jint);

/*
 * Class:     jni_FileJNI
 * Method:    cd
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_jni_FileJNI_cd
  (JNIEnv *, jobject, jstring, jint);

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

#ifdef __cplusplus
}
#endif
#endif