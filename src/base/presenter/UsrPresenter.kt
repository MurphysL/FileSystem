package base.presenter

import base.model.Usr

/**
 *
 * Created by MurphySL on 2017/12/13 19:31
 */
class UsrPresenter : Presenter<Usr>() {

    override fun insert(t: Usr): Int = jni.insertUsr(t)

    override fun delete(t: Usr) = jni.deleteUsr(t)

    override fun update(t: Usr) : Int = jni.updateUsr(t)

    override fun select(t: Usr) = jni.selectUsr(t)

    fun selectAll() : ArrayList<Usr> = jni.selectAllUsr()

}