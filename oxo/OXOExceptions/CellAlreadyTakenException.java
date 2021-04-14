package OXOExceptions;

public class CellAlreadyTakenException extends OXOMoveException {
    public CellAlreadyTakenException(int row, int column) {
        super(row, column);
    }

    public String toString() {
        return this.getClass().getName() + ": cell " + (char) (super.getColumn() + 'A') + (super.getRow() + 1) + " is already taken";
    }

}
