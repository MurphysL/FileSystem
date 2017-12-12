package manager;

import struts.SuperBlock;

import java.io.*;

/**
 * Created by MurphySL on 2017/12/11 13:59
 */
public class SuperBlockManager implements Manager<SuperBlock> {

    @Override
    public SuperBlock deserialize() {
        System.out.println(123);
        SuperBlock superBlock = null;
        try {
            ObjectInputStream ois = new ObjectInputStream(new FileInputStream("disk"));
            superBlock = (SuperBlock) ois.readObject();
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }

        return superBlock;
    }

    @Override
    public void serialize(SuperBlock superBlock) {
        try {
            ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("disk"));
            oos.writeObject(superBlock);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
