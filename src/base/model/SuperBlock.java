package base.model;

/**
 * Created by MurphySL on 2017/12/11 10:27
 */
public class SuperBlock{

    private int inodeNum;
    private int blockNum;
    private long update_time;

    public SuperBlock(){}

    public SuperBlock(int inodeNum, int blockNum, long update_time) {
        this.inodeNum = inodeNum;
        this.blockNum = blockNum;
        this.update_time = update_time;
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

    @Override
    public String toString() {
        return "SuperBlock{" +
                "inodeNum=" + inodeNum +
                ", blockNum=" + blockNum +
                ", update_time=" + update_time +
                '}';
    }
}
