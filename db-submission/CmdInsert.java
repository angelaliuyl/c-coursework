import DBException.*;

import java.util.*;

public class CmdInsert extends DBCmd {
    private final String tableName;
    private final List<String> values;

    public CmdInsert(String tableName, List<String> values) throws DBException {
        this.tableName = tableName;
        this.values = values;
        if (values.size() < 1) {
            throw new UnexpectedTermException("at least 1 value", "no values");
        }
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
            if (this.values.size() != table.getNumberOfColumns() - 1) {
                throw new MismatchException("number of values", "number of attributes");
            }
            String id = String.valueOf(table.getNextId());
            this.values.add(0, id);
            String[] valueArray = this.values.toArray(new String[0]);
            table.addNewRow(valueArray);
            DBFileIO.saveTableToFile(table);
            return "[OK] New row inserted";
        } catch (DBException dbe) {
            throw new QueryException("INSERT", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
