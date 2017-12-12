package struts;

/**
 * Created by MurphySL on 2017/12/11 14:30
 */
public class FileBlock extends Block {
    private static final long serialVersionUID = -7725171461800247231L;

    private int nextBlock;

    public int getNextBlock() {
        return nextBlock;
    }

    public void setNextBlock(int nextBlock) {
        this.nextBlock = nextBlock;
    }

    @Override
    public String toString() {
        return "FileBlock{" +
                "nextBlock=" + nextBlock +
                ", no=" + no +
                '}';
    }
}
