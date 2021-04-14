import DBException.*;
import java.util.*;

public class DBTable {

    private List<String> columnHeadings = new ArrayList<>();
    private List<DBEntity> tableContent = new ArrayList<>();
    private String tableName;
    private String databaseName;
    private int numberOfRows;
    private int nextId;
    private int numberOfColumns;


    public DBTable(String databaseName, String tableName, String[] columnHeadings) {
        this.tableName = tableName;
        this.numberOfRows = 0;
        this.nextId = 1;
        this.numberOfColumns = columnHeadings.length;
        this.databaseName = databaseName;
        Collections.addAll(this.columnHeadings, columnHeadings);
    }

    public String getTableName() {
        return tableName;
    }

    public String getDatabaseName() {
        return databaseName;
    }

    public void addNewRow(String[] rowContents) throws DBException {
        if (rowContents.length != numberOfColumns) {
            throw new MismatchException("width of new row", "width of table");
        }
        this.numberOfRows++;
        this.nextId++;
        DBEntity newRow = new DBEntity(this.numberOfColumns);
        for (int i = 0; i < this.numberOfColumns; i++) {
            newRow.setColumnValue(i, rowContents[i]);
        }
        tableContent.add(newRow);
    }

    public int getRowIndexById(int id) throws DBException {
        for (int i = 0; i < this.numberOfRows; i++) {
            DBEntity row = this.tableContent.get(i);
            if (row.getId() == id) {
                return i;
            }
        }
        throw new DoesNotExistException("row with id " + id);
    }

    public void deleteRowById(int id) throws DBException {
        int rowIndex = getRowIndexById(id);
        tableContent.remove(rowIndex);
        this.numberOfRows--;
    }

    public int getNextId() {
        return this.nextId;
    }

    public void setValue(String columnHeading, int rowNumber, String value) throws DBException {
        int columnIndex = getColumnIndex(columnHeading);
        try {
            DBEntity tempRow = tableContent.get(rowNumber);
            tempRow.setColumnValue(columnIndex, value);
            tableContent.set(rowNumber, tempRow);
        } catch (IndexOutOfBoundsException ioobe) {
            throw new DoesNotExistException("Row number " + rowNumber);
        }
    }

    public String getValue(String columnHeading, int rowNumber) throws DBException {
        int columnIndex = getColumnIndex(columnHeading);
        try {
            return tableContent.get(rowNumber).getColumnValue(columnIndex);
        } catch (IndexOutOfBoundsException ioobe) {
            throw new DoesNotExistException("Row number " + rowNumber);
        }
    }

    public DBEntity getRow(int rowNumber) throws DBException {
        try {
            return tableContent.get(rowNumber); //to get index of table

        } catch (IndexOutOfBoundsException ioobe) {
            throw new DoesNotExistException("Row number " + rowNumber);
        }
    }

    public String toString() {
        StringBuilder formattedString = new StringBuilder();
        for (int i = 0; i < numberOfColumns; i++) {
            if (i != 0) {
                formattedString.append("\t|");
            }
            formattedString.append(columnHeadings.get(i));
        }
        formattedString.append("\n");
        for (int j = 0; j < numberOfRows; j++) {
            formattedString.append((tableContent.get(j)).toString());
        }

        return formattedString.toString();
    }

    public String toFileOutput() {
        StringBuilder tabulatedString = new StringBuilder();
        for (int i = 0; i < numberOfColumns; i++) {
            if (i != 0) {
                tabulatedString.append("\t");
            }
            tabulatedString.append(columnHeadings.get(i));
        }
        tabulatedString.append("\n");
        for (int j = 0; j < numberOfRows; j++) {
            tabulatedString.append((tableContent.get(j)).toFileOutput());
        }

        return tabulatedString.toString();
    }

    public int getNumberOfColumns() {
        return numberOfColumns; //to account for the id column
    }

    public int getNumberOfRows() {
        return numberOfRows;
    }

    public int getColumnIndex(String columnHeading) throws DBException {
        int columnIndex = this.columnHeadings.indexOf(columnHeading);
        if (columnIndex == -1) {
            throw new DoesNotExistException("column " + columnHeading);
        }
        return columnIndex;
    }

    public void addNewColumn(String columnHeading) {
        this.columnHeadings.add(columnHeading);
        this.numberOfColumns++;
        for (DBEntity row : tableContent) {
            row.addColumn();
        }
    }

    public void dropColumn(String columnHeading) throws DBException {
        try {
            int columnIndex = getColumnIndex(columnHeading);
            this.columnHeadings.remove(columnIndex);
            for (DBEntity row : tableContent) {
                row.dropColumn(columnIndex);
            }
            this.numberOfColumns--;
        } catch (IndexOutOfBoundsException ioobe) {
            throw new DoesNotExistException("column " + columnHeading);
        }
    }

    public List<String> getColumnHeadings() {
        return this.columnHeadings;
    }

    public List<String> getRowContentById(int id) throws DBException {
        int index = getRowIndexById(id);
        return tableContent.get(index).getRowContent();
    }

    public List<Integer> getRowIndicesByValue(int columnIndex, String value) {
        List<Integer> rowIds = new ArrayList<>();
        for (DBEntity row : this.tableContent) {
            if (row.getColumnValue(columnIndex).equals(value)) {
                rowIds.add(row.getId());
            }
        }
        return rowIds;
    }

    public DBTable joinWithTable(DBTable table2, String attribute1, String attribute2) throws DBException {
        int table1RefIndex = this.getColumnIndex(attribute1);
        int table2RefIndex = table2.getColumnIndex(attribute2);
        List<String> newColumnHeadings = new ArrayList<>();
        newColumnHeadings.add("id");
        newColumnHeadings.addAll(this.columnHeadings.subList(1, columnHeadings.size()));
        newColumnHeadings.addAll(table2.getColumnHeadings().subList(1, table2.getNumberOfColumns()));
        for (int i = 1; i < 1 + this.numberOfColumns; i++) {
            String heading = newColumnHeadings.get(i);
            newColumnHeadings.set(i, this.tableName + "." + heading);
        }
        for (int i = 1 + this.numberOfColumns;
             i < newColumnHeadings.size(); i++) {
            String heading = newColumnHeadings.get(i);
            newColumnHeadings.set(i, table2.getTableName() + "." + heading);
        }

        DBTable jointTable = new DBTable(null, null, newColumnHeadings.toArray(new String[0]));
        int id = 1;
        int i = 0;
        for (DBEntity row : this.tableContent) {
            List<String> table1Portion = this.tableContent.get(i).getRowContent().subList(1, columnHeadings.size());
            String crossRefValue = row.getColumnValue(table1RefIndex);
            List<Integer> matchedRowsIds = table2.getRowIndicesByValue(table2RefIndex, crossRefValue);
            for (int rowId : matchedRowsIds) {
                List<String> table2Portion = table2.getRowContentById(rowId).subList(1, table2.getNumberOfColumns());
                List<String> rowContent = new ArrayList<>();
                rowContent.add(String.valueOf(id));
                rowContent.addAll(table1Portion);
                rowContent.addAll(table2Portion);
                jointTable.addNewRow(rowContent.toArray(new String[0]));
                id++;
            }
            i++;
        }
        return jointTable;
    }
}
