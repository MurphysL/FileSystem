package struts;

import sun.dc.pr.PRError;

import java.io.Serializable;

/**
 * Created by MurphySL on 2017/12/11 10:27
 */
public class SuperBlock implements Serializable{
    private static final long serialVersionUID = 6644812098746905862L;

    private int inodeNum;
    private int blockNum;
    private long update_time;
    private Inode node;
    private Block block;

    public static long getSerialVersionUID() {
        return serialVersionUID;
    }

    public int getInodeNum() {
        return inodeNum;
    }

    public void setInodeNum(int inodeNum) {
        this.inodeNum = inodeNum;
    }

    public int getBlockNum() {
        return blockNum;
    }

    public void setBlockNum(int blockNum) {
        this.blockNum = blockNum;
    }

    public long getUpdate_time() {
        return update_time;
    }

    public void setUpdate_time(long update_time) {
        this.update_time = update_time;
    }

    public Inode getNode() {
        return node;
    }

    public void setNode(Inode node) {
        this.node = node;
    }

    public Block getBlock() {
        return block;
    }

    public void setBlock(Block block) {
        this.block = block;
    }

    @Override
    public String toString() {
        return "SuperBlock{" +
                "inodeNum=" + inodeNum +
                ", blockNum=" + blockNum +
                ", update_time=" + update_time +
                '}';
    }
}
