import java.io.File
import java.io.FileOutputStream

/**
 *
 * Created by MurphySL on 2017/12/11 15:56
 */

fun main(args: Array<String>) {
    val file = File("buffer")
    val os  = FileOutputStream(file)
    val b = ByteArray(1024 * 1024)
    os.write(b)
}