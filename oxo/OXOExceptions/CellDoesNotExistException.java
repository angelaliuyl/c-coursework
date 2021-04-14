package OXOExceptions;

public class CellDoesNotExistException extends OXOMoveException {
    public CellDoesNotExistException()
    {

    }
    public String toString()
    {
        return this.getClass().getName() + ": Cell does not exist.";
    }

}
