package base.presenter

import base.model.Usr

/**
 *
 * Created by MurphySL on 2017/12/13 19:31
 */
class UsrPresenter : Presenter<Usr>() {

    override fun insert(t: Usr): Int = 1

    override fun delete(t: Usr) = Unit

    override fun update(t: Usr) : Int = 1

    override fun select(t: Usr): Int = 1

    fun selectAll() : ArrayList<Usr> = ArrayList()

}