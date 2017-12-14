package struts;

import base.model.DirBlock;
import base.model.Inode;

/**
 * Created by MurphySL on 2017/12/11 16:54
 */
public class Dir {
    private Inode inode;
    private DirBlock block;

    public Dir(Inode inode, DirBlock block) {
        this.inode = inode;
        this.block = block;
    }

    public Inode getInode() {
        return inode;
    }

    public void setInode(Inode inode) {
        this.inode = inode;
    }

    public DirBlock getBlock() {
        return block;
    }

    public void setBlock(DirBlock block) {
        this.block = block;
    }
}
