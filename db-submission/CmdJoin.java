import DBException.DBException;
import DBException.DoesNotExistException;
import DBException.QueryException;

public class CmdJoin extends DBCmd {

    private final String tableName1;
    private final String tableName2;
    private final String attribute1;
    private final String attribute2;

    public CmdJoin(String tableName1, String tableName2,
                   String attribute1, String attribute2) {
        this.tableName1 = tableName1;
        this.tableName2 = tableName2;
        this.attribute1 = attribute1;
        this.attribute2 = attribute2;
    }

    @Override
    public String query(DBServer server) throws DBException {
        try {
            String databaseName = server.getCurrentDatabase();
            if (!DBFileIO.tableExists(databaseName,
                    tableName1)) {
                throw new DoesNotExistException("table 1 " + tableName1);
            }
            if (!DBFileIO.tableExists(databaseName,
                    tableName2)) {
                throw new DoesNotExistException("table 2 " + tableName2);
            }
            DBTable table1 = DBFileIO.getTableFromFile(databaseName, tableName1);
            DBTable table2 = DBFileIO.getTableFromFile(databaseName, tableName2);
            DBTable jointTable = table1.joinWithTable(table2, attribute1, attribute2);
            return "[OK]\n" + jointTable.toFileOutput();

        } catch (DBException dbe) {
            throw new QueryException("JOIN", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable 1 name = " + this.tableName1 +
                "\nTable 2 name = " + this.tableName2 +
                "\nAttribute 1 = " + this.attribute1 +
                "\nAttribute 2 = " + this.attribute2;
    }
}
