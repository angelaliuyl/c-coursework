import DBException.*;
import java.util.*;

public class CmdSelect extends DBCmd {
    private final String tableName;

    public CmdSelect(String tableName) {
        this.tableName = tableName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            String databaseName = server.getCurrentDatabase();
            String tableName = this.tableName;
            if (!DBFileIO.tableExists(databaseName,
                    tableName)) {
                throw new DoesNotExistException("table");
            }
            DBTable table = DBFileIO.getTableFromFile(databaseName, tableName);
            List<Integer> visibleColumn = new ArrayList<>();

            if (this.attributeNames.size() == 0) {
                for (int i = 0; i < table.getNumberOfColumns(); i++) {
                    visibleColumn.add(i);
                }
            }
            else {
                for (String attributeName : this.attributeNames) {
                    visibleColumn.add(table.getColumnIndex(attributeName));
                }
            }
            StringBuilder returnTable = new StringBuilder("[OK]\n");
            List<String> columnHeadings = table.getColumnHeadings();
            for (int columnNumber : visibleColumn) {
                returnTable.append(columnHeadings.get(columnNumber)).append("\t");
            }
            returnTable.append("\n");
            for (int i = 0; i < table.getNumberOfRows(); i++) {
                StringBuilder rowString = new StringBuilder();
                DBEntity currentRow = table.getRow(i);
                if (this.condition == null || this.condition.evaluateCondition(table, i)) {
                    for (int columnNumber : visibleColumn) {
                        rowString.append(currentRow.getColumnValue(columnNumber)).append("\t");
                    }
                    rowString.append("\n");
                }
                returnTable.append(rowString);
            }
            return returnTable.toString();
        } catch (DBException dbe) {
            throw new QueryException("SELECT", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
