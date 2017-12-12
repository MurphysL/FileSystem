package struts;

import java.io.Serializable;

/**
 * Created by MurphySL on 2017/12/11 10:40
 */
public abstract class Block implements Serializable{
    private static final long serialVersionUID = -5921279516964974128L;

    protected int no;
    protected byte[] block = new byte[1024 * 1024];
    protected Block next;

    public Block getNext() {
        return next;
    }

    public void setNext(Block next) {
        this.next = next;
    }

    public int getNo() {
        return no;
    }

    public void setNo(int no) {
        this.no = no;
    }

    public byte[] getBlock() {
        return block;
    }

    public void setBlock(byte[] block) {
        this.block = block;
    }
}
