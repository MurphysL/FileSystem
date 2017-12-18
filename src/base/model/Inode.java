package base.model;

/**
 * Created by MurphySL on 2017/12/11 10:40
 * Updated by MurphySL on 2017/12/18 21:29
 */
public class Inode{
    private int no;
    private long size;
    private int flag;
    private long data_num;
    private long data_begin;
    private String owner;
    private int power;
    private long create_time;
    private long update_time;
    private long read_time;
    private int block_num;
    private int block_start;

    public Inode(){}

    public Inode(int no, long size, int flag, long data_num, long data_begin, String owner, int power,
                 long create_time, long update_time, long read_time, int block_num, int block_start) {
        this.no = no;
        this.size = size;
        this.flag = flag;
        this.data_num = data_num;
        this.data_begin = data_begin;
        this.owner = owner;
        this.power = power;
        this.create_time = create_time;
        this.update_time = update_time;
        this.read_time = read_time;
        this.block_num = block_num;
        this.block_start = block_start;
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

    public int getFlag() {
        return flag;
    }

    public void setFlag(int flag) {
        this.flag = flag;
    }

    public long getData_num() {
        return data_num;
    }

    public void setData_num(long data_num) {
        this.data_num = data_num;
    }

    public long getData_begin() {
        return data_begin;
    }

    public void setData_begin(long data_begin) {
        this.data_begin = data_begin;
    }

    public String getOwner() {
        return owner;
    }

    public void setOwner(String owener) {
        this.owner = owener;
    }

    public int getPower() {
        return power;
    }

    public void setPower(int power) {
        this.power = power;
    }

    public long getCreate_time() {
        return create_time;
    }

    public void setCreate_time(long create_time) {
        this.create_time = create_time;
    }

    public long getUpdate_time() {
        return update_time;
    }

    public void setUpdate_time(long update_time) {
        this.update_time = update_time;
    }

    public long getRead_time() {
        return read_time;
    }

    public void setRead_time(long read_time) {
        this.read_time = read_time;
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

    @Override
    public String toString() {
        return "Inode{" +
                "no=" + no +
                ", size=" + size +
                ", flag=" + flag +
                ", data_num=" + data_num +
                ", data_begin=" + data_begin +
                ", owener='" + owner + '\'' +
                ", power=" + power +
                ", create_time=" + create_time +
                ", update_time=" + update_time +
                ", read_time=" + read_time +
                ", block_num=" + block_num +
                ", block_start=" + block_start +
                '}';
    }
}
