import java.util.*;

public class DBEntity {

    private List<String> rowContent = new ArrayList<>();
    private int numberOfColumns;

    public DBEntity(int numberOfColumns) {
        this.numberOfColumns = numberOfColumns;
        for (int i = 0; i < numberOfColumns; i++) {
            rowContent.add(null);
        }
        //System.out.println(rowContent.size());
    }

    public int getId() {
        return Integer.parseInt(this.rowContent.get(0));
    }

    public String getColumnValue(int columnNumber) {
        return rowContent.get(columnNumber);
    }
    
    public void setColumnValue(int columnNumber, String value) {
        rowContent.set(columnNumber, value);
    }

    public void addColumn() {
        this.numberOfColumns++;
        this.rowContent.add(" ");
    }

    public void dropColumn(int columnIndex) throws IndexOutOfBoundsException {
        this.numberOfColumns--;
        this.rowContent.remove(columnIndex);
    }

    public List<String> getRowContent() {
        return this.rowContent;
    }

    public String toString() {
        StringBuilder formattedString = new StringBuilder();
        for (int i = 0; i < numberOfColumns; i++) {
            if (i != 0) {
                formattedString.append("\t|");
            }
            formattedString.append(getColumnValue(i));
        }
        formattedString.append("\n");

        return formattedString.toString();
    }

    public String toFileOutput() {
        StringBuilder tabulatedString = new StringBuilder();
        for (int i = 0; i < numberOfColumns; i++) {
            if (i != 0) {
                tabulatedString.append("\t");
            }
            tabulatedString.append(getColumnValue(i));
        }
        tabulatedString.append("\n");

        return tabulatedString.toString();
    }
}
