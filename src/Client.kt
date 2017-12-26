import base.Commond

/**
 *
 * Created by MurphySL on 2017/12/26 19:20
 */
fun main(args: Array<String>) {
    val commond = Commond()
    commond.mount()

    commond.mkdir("123", 0)
    val files = commond.ls(0)

    files.forEach(::println)
}