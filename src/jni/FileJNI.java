package jni;

import base.model.Inode;
import base.model.Usr;

/**
 * Created by MurphySL on 2017/12/18 20:46
 */
public class FileJNI {

    static {
        System.loadLibrary("DiskJNI");
    }

    public native Inode mount();

    public native int useradd(String name, String psw);
    public native int usermod(Usr usr);
    public native int userdel(String name, String psw);

    public native void ls();


    public static void main(String[] args){

    }
}
