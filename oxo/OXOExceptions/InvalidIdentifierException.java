package OXOExceptions;

public class InvalidIdentifierException extends CellDoesNotExistException {
    public InvalidIdentifierException()
    {
    }

    public String toString()
    {
        return this.getClass().getName() + ": Invalid Identifier Exception.";
    }
}
