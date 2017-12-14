import base.model.DirBlock;
import base.model.Inode;
import base.model.SuperBlock;
import manager.BlockManger;
import manager.SuperBlockManager;
import struts.*;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by MurphySL on 2017/12/11 13:43
 */
public class Client {
    private static final int SUPER_BLOCK_SIZE = 524288;

    private static final int INODE_NUM = 102400;
    private static final int INODE_SIZE = 1307200;

    private static final int BLOCK_NUM = 811008;
    private static final int BLOCK_SIZE = 103809024;

    private static SuperBlock superBlock ;
    private static SuperBlockManager superBlockManager = new SuperBlockManager();

    private static BlockManger blockManger = new BlockManger();

    public static void main(String[] args){
        try {
            File file = new File("disk");
            if(!file.exists())
                file.createNewFile();
            FileOutputStream fileOutputStream = new FileOutputStream("disk");
            fileOutputStream.write(new byte[1024 * 1024 * 100]);
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        init();

       printInitInfo();
    }

    private static void printInitInfo() {
        /*SuperBlock superBlock = superBlockManager.deserialize();
        System.out.println("空闲 inode 数 / 总 inode 数 : " + superBlock.getInodeNum() + "/" + superBlock.getInodeNum());
        System.out.println("空闲 block 数 / 总 block 数 : " + superBlock.getBlockNum() + "/" + superBlock.getBlockNum());
        Inode node = superBlock.getNode();
        while(node != null){
            System.out.println("是否文件：" + node.isIs_file());
            if(!node.isIs_file()){
                int block_no = node.getBlock_start();
                DirBlock block = (DirBlock) superBlock.getBlock();
                while(block != null && block.getNo() != block_no){
                    block = (DirBlock) block.getNext();
                    Map<String, Integer> map = block.getFiles();
                    for(Map.Entry<String, Integer> entry : map.entrySet()){
                        System.out.println(entry.getKey());
                    }
                }
                if(block != null)
                    System.out.println(block.getDirName());
            }
            node = node.getNext();
        }*/
    }

    private static void init(){
        superBlockManager = new SuperBlockManager();
        superBlock = superBlockManager.deserialize();

        if(superBlock == null)
            mount();
    }

    private static void mount(){
        System.out.println("mount");
        superBlock = new SuperBlock();
        superBlock.setInodeNum(INODE_NUM);
        superBlock.setBlockNum(BLOCK_NUM);
        superBlock.setUpdate_time(System.currentTimeMillis());

        mountDirs();

        superBlockManager.serialize(superBlock);
        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static void mountDirs() {
        // root
        Dir root = blockManger.initDir(superBlock, "/", 0);
        // usr
        Dir usr = blockManger.initDir(superBlock, "usr", root.getInode().getNo());
        // home
        Dir home = blockManger.initDir(superBlock, "home", root.getInode().getNo());
        // local
        Dir local = blockManger.initDir(superBlock, "local", root.getInode().getNo());

        HashMap<String , Integer> root_sub = new HashMap<>();
        root_sub.put(usr.getBlock().getDirName(), usr.getInode().getNo());
        root_sub.put(home.getBlock().getDirName(), home.getInode().getNo());
        blockManger.addSubFiles(root.getBlock(), root_sub);

        HashMap<String, Integer> usr_sub = new HashMap<>();
        usr_sub.put(local.getBlock().getDirName(), local.getInode().getNo());
        blockManger.addSubFiles(usr.getBlock(), usr_sub);

    }


}
