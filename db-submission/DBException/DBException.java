package DBException;

public abstract class DBException extends Exception {

    public DBException()
    {
    }

    public String toString() {
        return this.getClass().getName();
    }
}
