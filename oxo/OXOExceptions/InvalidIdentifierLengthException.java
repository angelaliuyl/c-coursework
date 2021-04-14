package OXOExceptions;

public class InvalidIdentifierLengthException extends InvalidIdentifierException {
    private int length;

    public InvalidIdentifierLengthException(int length)
    {
        this.length = length;
    }

    public String toString()
    {
        return this.getClass().getName() + ": input needs to be length 2 but has length " + this.length;
    }
}
