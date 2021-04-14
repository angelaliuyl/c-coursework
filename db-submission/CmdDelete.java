import DBException.*;
import java.util.*;

public class CmdDelete extends DBCmd {
    private final String tableName;
    private final DBCondition condition;

    public CmdDelete(String tableName, DBCondition condition) {
        this.tableName = tableName;
        this.condition = condition;
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
            int deleteCount = 0;
            List<Integer> rowsToDelete = new ArrayList<>();
            for (int i = 0; i < table.getNumberOfRows(); i++) {
                DBEntity currentRow = table.getRow(i);
                //System.out.println(currentRow);
                if (this.condition.evaluateCondition(table, i)) {
                    rowsToDelete.add(currentRow.getId());
                    deleteCount++;
                }
            }
            for (int row : rowsToDelete) {
                table.deleteRowById(row);
            }
            DBFileIO.saveTableToFile(table);
            return "[OK] deleted " + deleteCount + " entry(s).";
        } catch (DBException dbe) {
            throw new QueryException("DELETE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
