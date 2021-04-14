package DBException;

public class FileIOException extends DBException {
    private String errorMessage;

    public FileIOException(String errorMessage) {
        this.errorMessage = errorMessage;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": " + errorMessage;
    }
}
