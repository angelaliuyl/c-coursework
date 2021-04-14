package OXOExceptions;

import static OXOExceptions.RowOrColumn.ROW;

public class InvalidIdentifierCharacterException extends InvalidIdentifierException {
    private char character;
    private RowOrColumn type;

    public InvalidIdentifierCharacterException(char character, RowOrColumn type)
    {
        this.character = character;
        this.type = type;
    }

    public String toString() {
        if (this.type == ROW) {
            return this.getClass().getName() + ": " + this.character +" is not a valid character for row.";
        }
        else {
            return this.getClass().getName() + ": " + this.character +" is not a valid character for column.";
        }
    }
}
