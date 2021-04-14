import DBException.*;

public class CmdDropTable extends DBCmd {
    private final String tableName;

    public CmdDropTable(String tableName) {
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
            DBFileIO.deleteTable(tableName, databaseName);
            return "[OK]";
        } catch (DBException dbe) {
            throw new QueryException("DROP TABLE", dbe.toString());
        }
    }

    @Override
    public String toString() {
        return "Command = " + this.getClass().getName() +
                "\nTable name = " + this.tableName;
    }
}
