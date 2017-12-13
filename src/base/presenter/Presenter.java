package base.presenter;

import jni.DiskJNI;

import java.util.ArrayList;

/**
 * Created by MurphySL on 2017/12/13 10:09
 */
public abstract class Presenter<T> {
    protected DiskJNI jni = new DiskJNI();

    public abstract void insert(T t);

    public abstract void delete(T t);

    public abstract int select(T t);

    public abstract ArrayList<T> selectAll();

    public abstract void update(T t);
}
