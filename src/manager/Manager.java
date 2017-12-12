package manager;

/**
 * Created by MurphySL on 2017/12/11 13:39
 */
public interface Manager<T> {

    T deserialize();

    void serialize(T t);

}
