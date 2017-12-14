package base.model;

/**
 * Created by MurphySL on 2017/12/11 14:30
 */
public class FileBlock extends Block {

    private int nextBlock;

    public FileBlock(){}

    public FileBlock(int nextBlock) {
        this.nextBlock = nextBlock;
    }

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
