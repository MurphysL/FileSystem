package base.presenter

import jni.DiskJNI

/**
 *
 * Created by MurphySL on 2017/12/13 19:29
 */
abstract class Presenter<in T>{
    protected val jni : DiskJNI = DiskJNI()

    abstract fun insert(t: T) : Int

    abstract fun delete(t: T)

    abstract fun update(t: T) : Int

    abstract fun select(t: T) : Int
}