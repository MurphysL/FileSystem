package base

import base.model.Sub
import jni.FileJNI

/**
 *
 * Created by MurphySL on 2017/12/19 14:45
 */
class Commond{

    private val jni : FileJNI = FileJNI()

    fun mount() = jni.mount()

    fun ls(no : Int) : List<Sub> = jni.ls(no)

    fun cd(path : String, no : Int) : Int{
        var aim = no
        val dirs = path.split("\\")

        for(dir in dirs){
            aim = jni.cd(dir, aim)
            if(aim == -1)
                return aim
        }

        return aim
    }

    fun mkdir(name : String, no : Int) = jni.mkdir(name, no)

    fun touch(name : String, no : Int) = jni.touch(name, no)

    fun rm(no : Int, father : Int) = jni.rm(no, father)

}