package struts;

import java.util.Map;

/**
 * Created by MurphySL on 2017/12/11 14:33
 */
public class DirBlock extends Block{
    private static final long serialVersionUID = -2282146984490222174L;

    private String dirName;
    private Map<String, Integer> files;

    public Map<String, Integer> getFiles() {
        return files;
    }

    public void setFiles(Map<String, Integer> files) {
        this.files = files;
    }

    public String getDirName() {
        return dirName;
    }

    public void setDirName(String dirName) {
        this.dirName = dirName;
    }

    @Override
    public String toString() {
        return "DirBlock{" +
                "dirName='" + dirName + '\'' +
                ", files=" + files +
                ", no=" + no +
                '}';
    }
}
