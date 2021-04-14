package DBException;

public class CommandParseException extends DBException {
    private String commandType;
    private String errorMessage;

    public CommandParseException(String commandType, String errorMessage) {
        this.commandType = commandType;
        this.errorMessage = errorMessage;
    }

    @Override
    public String toString() {
        return this.getClass().getName() +
                ": Error in parsing " + commandType + " command - " + errorMessage;
    }
}
