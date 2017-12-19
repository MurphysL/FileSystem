package base.model;

/**
 * Created by MurphySL on 2017/12/19 17:15
 */
public class Sub {
    private String name;
    private int no;

    public Sub(){}

    public Sub(String name, int no) {
        this.name = name;
        this.no = no;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getNo() {
        return no;
    }

    public void setNo(int no) {
        this.no = no;
    }

    @Override
    public String toString() {
        return "Sub{" +
                "name='" + name + '\'' +
                ", no=" + no +
                '}';
    }
}
