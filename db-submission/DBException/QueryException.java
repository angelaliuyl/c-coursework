package DBException;

public class QueryException extends DBException {
    String commandType;
    private String errorMessage;

    public QueryException(String commandType, String errorMessage) {
        this.commandType = commandType;
        this.errorMessage = errorMessage;
    }

    public String toString() {
        return this.getClass().getName() +
                ": Error in executing " + commandType + " command - " + errorMessage;
    }
}
