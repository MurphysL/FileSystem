package util;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by MurphySL on 2017/12/25 13:21
 */
public class FileSystem {
    private Map<Integer, Block> table = new HashMap<>();
    private int current = 0;
    private int no = 0;

    private class Dir extends Block{
        public HashMap<String, Integer> sub = new HashMap<>();

        public Dir(String name, int no) {
            super(name, no);
        }
    }

    private class File extends Block{
        public File(String name, int no) {
            super(name, no);
        }
    }

    private class Block{
        protected String name;
        private int no;

        Block(String name, int no) {
            this.name = name;
            this.no = no;
        }
    }

    public FileSystem(){
        Dir dir = new Dir("/", no);
        table.put(no ++, dir);
    }

    public boolean touch(String name){
        Dir father = (Dir) table.get(current);
        for(String s : ls()){
            if (name.equals(s))
                return false;
        }
        File file = new File(name, no);
        table.put(no, file);
        father.sub.put(name, no++);
        return true;
    }

    public boolean mkdir(String name){
        Dir father = (Dir) table.get(current);
        for(String s : ls()){
            if (name.equals(s))
                return false;
        }
        Dir dir = new Dir(name, no);
        table.put(no, dir);
        father.sub.put(name, no++);
        return true;
    }

    public boolean rm(String name){
        Dir dir = (Dir) table.get(current);
        for(String s : ls()){
            if (name.equals(s)){
                int no = dir.sub.get(name);
                dir.sub.remove(name);
                table.remove(no);
                return true;
            }
        }
        return false;
    }

    public String[] ls(){
        List<String> list = new ArrayList<>();
        Dir dir = (Dir) table.get(current);
        for (Object o : dir.sub.entrySet()) {
            Map.Entry<String, Integer> entry = (Map.Entry<String, Integer>) o;
            list.add(entry.getKey());
        }
        return list.toArray(new String[0]);
    }

    public void cd(String path){
        String[] dirs =  path.split("\\\\");
        if(dirs.length <= 1)
            return;

        Dir root = (Dir) table.get(0);
        for(String dir : dirs){
            for(Object o : root.sub.entrySet()){
                Map.Entry<String, Integer> e = (Map.Entry<String, Integer>) o;
                String s = e.getKey();
                if(s.equals(dir)){
                    if(table.get(e.getValue()) instanceof Dir){
                        root = (Dir) table.get(e.getValue());
                        current = e.getValue();
                    }
                }
            }
        }
    }

    public static void main(String[] args){
        FileSystem fs = new FileSystem();
        fs.mkdir("123");
        for(String s :fs.ls()){
            System.out.println(s);
        }
        fs.cd("\\123\\");
        fs.touch("234");
        for(String s :fs.ls()){
            System.out.println(s);
        }
        fs.rm("234");
        for(String s :fs.ls()){
            System.out.println(s);
        }
    }
}
