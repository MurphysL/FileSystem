package base.presenter;

import base.model.Usr;

import java.util.ArrayList;

/**
 * Created by MurphySL on 2017/12/13 10:08
 */
public class UsrPresenter extends Presenter<Usr> {

    @Override
    public void insert(Usr usr) {
        jni.insertUsr(usr);
    }

    @Override
    public void delete(Usr usr) {
        jni.deleteUsr(usr);
    }

    @Override
    public int select(Usr usr) {
        return jni.selectUsr(usr);
    }

    @Override
    public ArrayList<Usr> selectAll() {
        return jni.selectAllUsr();
    }

    @Override
    public void update(Usr usr) {
        jni.updateUsr(usr);
    }
}
