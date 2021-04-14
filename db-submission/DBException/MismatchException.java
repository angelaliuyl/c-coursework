package DBException;

public class MismatchException extends DBException {
    private String field1;
    private String field2;

    public MismatchException(String field1, String field2) {
        this.field1 = field1;
        this.field2 = field2;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": " + field1 + " does not match " + field2;
    }
}
