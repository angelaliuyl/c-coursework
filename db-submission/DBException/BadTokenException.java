package DBException;

public class BadTokenException extends DBException {
    private String tokenContent;

    public BadTokenException(String tokenContent) {
        this.tokenContent = tokenContent;
    }

    @Override
    public String toString() {
        return super.toString() +
                ": token content " + this.tokenContent +
                " cannot be tokenized";
    }
}
