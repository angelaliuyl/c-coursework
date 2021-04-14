package OXOExceptions;

import static OXOExceptions.RowOrColumn.ROW;

public class OutsideCellRangeException extends CellDoesNotExistException {
    private int position;
    private RowOrColumn type;

    public OutsideCellRangeException(int pos, RowOrColumn type) {
        this.position = pos;
        this.type = type;
    }

    public String toString() {
        if (this.type == ROW) {
            return this.getClass().getName() + ": Cell does not exist, row " + (char) (position + 'A') + " is out of bounds";
        } else {
            return this.getClass().getName() + ": Cell does not exist, col " + (position + 1) + " is out of bounds";
        }
    }

}
