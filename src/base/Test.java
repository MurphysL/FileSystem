package base;

import base.model.Usr;
import base.presenter.UsrPresenter;

import java.util.ArrayList;

/**
 * Created by MurphySL on 2017/12/14 14:41
 */
public class Test {
    public static void main(String[] args){
        UsrPresenter usrPresenter = new UsrPresenter();
        Usr usr = new Usr("MurphySL", "yq578797");
        usrPresenter.insert(usr);

        Usr usr2 = new Usr("Name", "HelloWorld");
        usrPresenter.insert(usr2);

        System.out.println(usrPresenter.select(usr));

        ArrayList<Usr> list = usrPresenter.selectAll();
        for(Usr u : list){
            System.out.println(u.toString());
        }
    }
}
