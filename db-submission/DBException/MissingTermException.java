package DBException;

public class MissingTermException extends DBException {
    private String expectedContent;

    public MissingTermException(String expectedContent) {
        this.expectedContent = expectedContent;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": Expecting " + expectedContent;
    }
}
