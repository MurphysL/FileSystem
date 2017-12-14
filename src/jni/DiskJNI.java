package jni;

import base.model.Block;
import base.model.Inode;
import base.model.SuperBlock;
import base.model.Usr;

import java.util.ArrayList;

/**
 * Created by MurphySL on 2017/12/12 19:34
 */
public class DiskJNI {

    static {
        System.loadLibrary("DiskJNI");
    }

    // USR
    public native int insertUsr(Usr usr);
    public native void deleteUsr(Usr usr);
    public native int updateUsr(Usr usr);
    public native int selectUsr(Usr usr);
    public native ArrayList<Usr> selectAllUsr();

    // Inode
    public native int insertInode(Inode inode);
    public native void deleteInode(Inode inode);
    public native int updateInode(Inode inode);
    public native int selectInode(Inode inode);

    //SUPERBLOCK
    public native int insertSuper(SuperBlock superBlock);
    public native void deleteSuper(SuperBlock superBlock);
    public native int updateSuper(SuperBlock superBlock);
    public native int selectSuper(SuperBlock superBlock);

    //BLOCK
    public native int insertBlock(Block block);
    public native void deleteBlock(Block block);
    public native int updateBlock(Block block);
    public native int selectBlock(Block block);

    public static void main(String[] args){

    }
}
