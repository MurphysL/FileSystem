package jni;

import base.model.Sub;

import java.util.List;

/**
 * Created by MurphySL on 2017/12/18 20:46
 */
public class FileJNI {

    static {
        System.loadLibrary("FileJNI");
    }

    public native void mount();

    public native int mkdir(String name, int no);

    public native int touch(String name, int no);

    public native int rm(int no, int father);

    public native List<Sub> ls(int no);

    // - 1 未找到
    public native int cd(String dir, int no);

    public native int mv_in(String file, int no); //移入

    //public native int mv(int old, int now); // 移动

    //public native int mv(String old, String now); // 重命名

    public native int mv_out(int no, String file); // 移出

    //public native int cp(String file, int no); // 复制入

    //public native int cp(int old, int now); // 复制

    //public native int cp(int no, String file); // 复制出

    //public native List<String> find(String name);


    public static void main(String[] args){

    }
}
