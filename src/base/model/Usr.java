package base.model;

/**
 * Created by MurphySL on 2017/12/13 10:02
 */
public class Usr {

    private String name = "";
    private String psw = "";
    private short su;

    public Usr(){}

    public Usr(String name, String psw, short su) {
        this.name = name;
        this.psw = psw;
        this.su = su;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPsw() {
        return psw;
    }

    public void setPsw(String psw) {
        this.psw = psw;
    }

    public short getSu() {
        return su;
    }

    public void setSu(short su) {
        this.su = su;
    }

    @Override
    public String toString() {
        return "Usr{" +
                "name='" + name + '\'' +
                ", psw='" + psw + '\'' +
                ", su=" + su +
                '}';
    }
}
