package struts;

import java.io.Serializable;

/**
 * Created by MurphySL on 2017/12/11 13:28
 */
public class User implements Serializable{
    private static final long serialVersionUID = -3868922001742601629L;

    private String username;
    private String password;

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    @Override
    public String toString() {
        return "User{" +
                "username='" + username + '\'' +
                ", password='" + password + '\'' +
                '}';
    }
}
