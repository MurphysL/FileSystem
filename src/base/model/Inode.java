package base.model;

/**
 * Created by MurphySL on 2017/12/11 10:40
 */
public class Inode{

    private int no;
    private boolean is_file;
    private long size;
    private int block_num;
    private int block_start;
    private String owner;
    private String power;

    public Inode(){}

    public Inode(int no, boolean is_file, long size, int block_num, int block_start, String owner, String power) {
        this.no = no;
        this.is_file = is_file;
        this.size = size;
        this.block_num = block_num;
        this.block_start = block_start;
        this.owner = owner;
        this.power = power;
    }

    public int getNo() {
        return no;
    }

    public void setNo(int no) {
        this.no = no;
    }

    public long getSize() {
        return size;
    }

    public void setSize(long size) {
        this.size = size;
    }

    public int getBlock_num() {
        return block_num;
    }

    public void setBlock_num(int block_num) {
        this.block_num = block_num;
    }

    public int getBlock_start() {
        return block_start;
    }

    public void setBlock_start(int block_start) {
        this.block_start = block_start;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owner) {
        this.owner = owner;
    }

    public String getPower() {
        return power;
    }

    public void setPower(String power) {
        this.power = power;
    }

    public boolean isIs_file() {
        return is_file;
    }

    public void setIs_file(boolean is_file) {
        this.is_file = is_file;
    }

    @Override
    public String toString() {
        return "InodeJNI{" +
                "no=" + no +
                ", is_file=" + is_file +
                ", size=" + size +
                ", block_num=" + block_num +
                ", block_start=" + block_start +
                ", owner=" + owner +
                ", power='" + power + '\'' +
                '}';
    }
}
