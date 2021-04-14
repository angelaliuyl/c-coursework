import DBException.*;

public class CmdAlterDrop extends DBCmd {
    private final String tableName;
    private final String attributeName;

    public CmdAlterDrop(String tableName, String attributeName) {
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
            table.dropColumn(this.attributeName);
            DBFileIO.saveTableToFile(table);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("ALTER DROP", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
