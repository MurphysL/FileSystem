package util

import java.io.File
import java.io.FileOutputStream

/**
 *
 * Created by MurphySL on 2017/12/14 22:25
 */
fun main(args: Array<String>) {
    val file = File("disk")
    val os = FileOutputStream(file)
    val b = ByteArray(1024*10)
    os.write(b)
}