package base.presenter

import jni.FileJNI

/**
 *
 * Created by MurphySL on 2017/12/13 19:29
 */
abstract class Presenter<in T>{
    protected val jni : FileJNI = FileJNI()

    abstract fun insert(t: T) : Int

    abstract fun delete(t: T)

    abstract fun update(t: T) : Int

    abstract fun select(t: T) : Int
}