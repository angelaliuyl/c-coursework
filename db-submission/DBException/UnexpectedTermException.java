package DBException;

public class UnexpectedTermException extends DBException {
    private final String expectedContent;
    private final String actualContent;

    public UnexpectedTermException(String expectedContent, String actualContent) {
        this.expectedContent = expectedContent;
        this.actualContent = actualContent;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": Expecting " + expectedContent +
                " instead of " + actualContent;
    }
}
