import DBException.DBException;
import DBException.DoesNotExistException;
import DBException.QueryException;

public class CmdAlterAdd extends DBCmd {

    private final String tableName;
    private final String attributeName;

    public CmdAlterAdd(String tableName, String attributeName) throws DBException {
        this.tableName = tableName;
        this.attributeName = attributeName;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            String databaseName = server.getCurrentDatabase();
            if (!DBFileIO.tableExists(databaseName,
                    this.tableName)) {
                throw new DoesNotExistException("table");
            }
            DBTable table = DBFileIO.getTableFromFile(databaseName, this.tableName);
            table.addNewColumn(this.attributeName);
            DBFileIO.saveTableToFile(table);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("ALTER ADD", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
