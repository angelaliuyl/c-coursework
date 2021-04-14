package DBException;

public class UnexpectedEndException extends DBException {
    @Override
    public String toString() {
        return super.toString() + ": command ended unexpectedly";
    }
}
