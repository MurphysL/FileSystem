package jni;

import base.model.Usr;

import java.util.ArrayList;

/**
 * Created by MurphySL on 2017/12/12 19:34
 */
public class DiskJNI {

    static {
        System.loadLibrary("DiskJNI");
    }

    public native void insertUsr(Usr usr);

    public native void deleteUsr(Usr usr);

    public native void updateUsr(Usr usr);

    //confirm user info
    public native int selectUsr(Usr usr);

    public native ArrayList<Usr> selectAllUsr();

    public static void main(String[] args){
        Usr usr = new Usr("123", "123", (short)1);
        new DiskJNI().insertUsr(usr);
    }
}
