package manager;

import base.model.Block;
import base.model.DirBlock;
import base.model.Inode;
import base.model.SuperBlock;
import struts.*;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by MurphySL on 2017/12/11 15:13
 */
public class BlockManger implements Manager<Block>{

    @Override
    public Block deserialize() {
        return null;
    }

    @Override
    public void serialize(Block block) {

    }

    public Dir initDir(SuperBlock superBlock, String dirName, int fatherInodeNo){
        int inodeNum = superBlock.getInodeNum();
        int blockNum = superBlock.getBlockNum();

        HashMap<String, Integer> sub = new HashMap<>();
        sub.put(".", inodeNum);
        sub.put("..", fatherInodeNo);
        DirBlock dir = new DirBlock();
        dir.setNo(blockNum);
        dir.setDirName(dirName);
        dir.setFiles(sub);

        Inode inode = new Inode();
        inode.setNo(inodeNum);
        inode.setBlock_num(1);
        inode.setBlock_start(dir.getNo());

        superBlock.setInodeNum(-- inodeNum);
        superBlock.setBlockNum(-- blockNum);

       /* if(superBlock.getNode() == null)
            superBlock.setNode(inode);
        else{
            Inode inode1 = superBlock.getNode();
            while(inode1.getNext() != null){
                inode1 = inode1.getNext();
            }
            inode1.setNext(inode);
        }

        if(superBlock.getBlock() == null)
            superBlock.setBlock(dir);
        else{
            Block dir1 = superBlock.getBlock();
            while(dir1.getNext() != null){
                dir1 = dir1.getNext();
            }
            dir1.setNext(dir);
        }*/

        return new Dir(inode, dir);
    }

    public void addSubFiles(DirBlock dir, HashMap<String, Integer> map){
        if(dir == null){
            return;
        }
        Map<String, Integer> sub = dir.getFiles();
        for(Map.Entry<String, Integer> m : map.entrySet()){
            sub.put(m.getKey(), m.getValue());
        }
    }
}
